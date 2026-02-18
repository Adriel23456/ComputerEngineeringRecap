#include "apps/cpu_tomasulo/simulation/TomasuloSimController.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>

// ============================================================================
// Lifecycle
// ============================================================================

TomasuloSimController::~TomasuloSimController() {
    stop();
}

void TomasuloSimController::start() {
    if (m_running.load()) return;
    m_running.store(true);
    m_worker = std::thread(&TomasuloSimController::workerLoop, this);
}

void TomasuloSimController::stop() {
    if (!m_running.load()) return;
    m_running.store(false);
    m_stopInfinite.store(true);
    m_queueCV.notify_all();
    if (m_worker.joinable()) m_worker.join();
}

// ============================================================================
// Task Dispatch — no std::cout in any of these
// ============================================================================

void TomasuloSimController::requestCompile(const std::string& source) {
    SimTask task; task.type = SimTask::Type::Compile; task.source = source;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestLoadBinary(const std::string& filePath) {
    SimTask task; task.type = SimTask::Type::LoadBinary; task.filePath = filePath;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestReset() {
    SimTask task; task.type = SimTask::Type::Reset;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestResetRAM() {
    SimTask task; task.type = SimTask::Type::ResetRAM;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestStep() {
    SimTask task; task.type = SimTask::Type::Step;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestStepUntil(int count) {
    SimTask task; task.type = SimTask::Type::StepUntil; task.stepCount = count;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestInfiniteStep() {
    m_stopInfinite.store(false);
    SimTask task; task.type = SimTask::Type::InfiniteStep;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestInfiniteStepMS(int delayMs) {
    m_stopInfinite.store(false);
    SimTask task; task.type = SimTask::Type::InfiniteStepMS;
    task.stepCount = delayMs;  // reuse stepCount to carry the delay
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestStopInfinite() {
    m_stopInfinite.store(true);
}

// ============================================================================
// Result Polling
// ============================================================================

bool TomasuloSimController::hasResult() const { return m_hasResult.load(std::memory_order_acquire); }

SimTaskResult TomasuloSimController::consumeResult() {
    std::lock_guard<std::mutex> lk(m_resultMutex);
    m_hasResult.store(false, std::memory_order_release);
    return m_lastResult;
}

bool TomasuloSimController::isBusy() const { return m_busy.load(std::memory_order_relaxed); }

void TomasuloSimController::pushResult(SimTask::Type type, bool success,
    const std::string& msg, uint64_t cycles) {
    std::lock_guard<std::mutex> lk(m_resultMutex);
    m_lastResult.type = type;
    m_lastResult.success = success;
    m_lastResult.message = msg;
    m_lastResult.cycleCount = cycles;
    m_hasResult.store(true, std::memory_order_release);
}

// ============================================================================
// Thread-safe Accessors
// ============================================================================

std::mutex& TomasuloSimController::mutex() { return m_simMutex; }
TomasuloCPU& TomasuloSimController::cpu() { return m_cpu; }
const TomasuloCPU& TomasuloSimController::cpu() const { return m_cpu; }

// ============================================================================
// Worker Loop — no OMP, clean wait
// ============================================================================

void TomasuloSimController::workerLoop() {
    while (m_running.load()) {
        SimTask task;
        {
            std::unique_lock<std::mutex> lk(m_queueMutex);
            m_queueCV.wait(lk, [this] {
                return !m_taskQueue.empty() || !m_running.load();
                });
            if (!m_running.load() && m_taskQueue.empty()) break;
            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
        }
        m_busy.store(true, std::memory_order_relaxed);
        processTask(task);
        m_busy.store(false, std::memory_order_relaxed);
    }
}

// ============================================================================
// Task Processing — BATCHED locking for step loops
// ============================================================================

void TomasuloSimController::processTask(const SimTask& task) {

    switch (task.type) {

    case SimTask::Type::Compile: {
        std::lock_guard<std::mutex> simLk(m_simMutex);
        AssemblyResult asmResult = m_cpu.compile(task.source);
        pushResult(task.type, asmResult.success, asmResult.summary, m_cpu.cycleCount());
        break;
    }

    case SimTask::Type::LoadBinary: {
        try {
            std::ifstream file(task.filePath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                pushResult(task.type, false, "Error: Could not open file.");
                break;
            }
            auto fileSize = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<uint8_t> rawBytes(static_cast<size_t>(fileSize));
            file.read(reinterpret_cast<char*>(rawBytes.data()), static_cast<std::streamsize>(fileSize));
            file.close();

            size_t totalWords = rawBytes.size() / 8;
            std::vector<uint64_t> words(totalWords, 0);
            for (size_t i = 0; i < totalWords; ++i) {
                size_t offset = i * 8;
                uint64_t w = 0;
                for (int b = 0; b < 8; ++b)
                    w |= static_cast<uint64_t>(rawBytes[offset + b]) << (b * 8);
                words[i] = w;
            }

            std::lock_guard<std::mutex> simLk(m_simMutex);
            uint64_t startAddr = m_cpu.registers().get(TomasuloRegisterFile::UPPER);
            size_t startRow = static_cast<size_t>(startAddr / TomasuloRAM::kStep);
            size_t written = 0;
            for (size_t i = 0; i < words.size(); ++i) {
                size_t row = startRow + i;
                if (row >= TomasuloRAM::kTotalRows) break;
                m_cpu.ram().write(row, words[i]);
                ++written;
            }

            std::ostringstream oss;
            oss << "Success - Loaded " << written << " word(s) from 0x"
                << std::hex << startAddr << std::dec << ".";
            pushResult(task.type, true, oss.str(), m_cpu.cycleCount());
        }
        catch (const std::exception& e) {
            pushResult(task.type, false, std::string("Load error: ") + e.what());
        }
        break;
    }

    case SimTask::Type::Reset: {
        std::lock_guard<std::mutex> simLk(m_simMutex);
        m_cpu.reset();
        pushResult(task.type, true, "Success - CPU reset.", 0);
        break;
    }

    case SimTask::Type::ResetRAM: {
        std::lock_guard<std::mutex> simLk(m_simMutex);
        m_cpu.resetRAMOnly();
        pushResult(task.type, true, "Success - RAM reset.", m_cpu.cycleCount());
        break;
    }

    case SimTask::Type::Step: {
        std::lock_guard<std::mutex> simLk(m_simMutex);
        uint64_t c = m_cpu.step();
        pushResult(task.type, true, "Step complete.", c);
        break;
    }

    // ── StepUntil — BATCHED lock ────────────────────────────────
    case SimTask::Type::StepUntil: {
        int total = task.stepCount;
        int done = 0;
        while (done < total) {
            int batch = std::min(kCyclesPerBatch, total - done);
            {
                std::lock_guard<std::mutex> simLk(m_simMutex);
                for (int i = 0; i < batch; ++i) {
                    m_cpu.step();
                    if (m_cpu.m_halted) { done = total; break; }
                }
            }
            done += batch;
            // Yielding the lock here lets the UI thread grab it for a frame
        }
        uint64_t c;
        {
            std::lock_guard<std::mutex> simLk(m_simMutex);
            c = m_cpu.cycleCount();
        }
        std::string msg = m_cpu.m_halted
            ? "SWI reached at cycle " + std::to_string(c) + ". CPU halted."
            : "StepUntil complete. " + std::to_string(total) + " cycles. Total: " + std::to_string(c);
        pushResult(task.type, true, msg, c);
        break;
    }

    // ── InfiniteStep — BATCHED lock ─────────────────────────────
    case SimTask::Type::InfiniteStep: {
        m_runningInfinite.store(true, std::memory_order_relaxed);
        uint64_t c = 0;
        bool hitSWI = false;

        while (!m_stopInfinite.load(std::memory_order_relaxed)
            && m_running.load(std::memory_order_relaxed)) {
                {
                    std::lock_guard<std::mutex> simLk(m_simMutex);
                    for (int i = 0; i < kCyclesPerBatch; ++i) {
                        c = m_cpu.step();
                        if (m_cpu.m_halted) { hitSWI = true; break; }
                    }
                }
                // Lock released — UI can grab it for this frame's render

                if (hitSWI) break;

                // Publish progress periodically (not every cycle!)
                if ((c & 0x3FF) == 0) {  // every ~1024 cycles
                    pushResult(SimTask::Type::InfiniteStep, true,
                        "Running... cycle " + std::to_string(c), c);
                }
        }

        m_runningInfinite.store(false, std::memory_order_relaxed);
        {
            std::lock_guard<std::mutex> simLk(m_simMutex);
            c = m_cpu.cycleCount();
        }
        std::string msg = hitSWI
            ? "SWI reached at cycle " + std::to_string(c) + ". CPU halted."
            : "InfiniteStep stopped at cycle " + std::to_string(c) + ".";
        pushResult(task.type, true, msg, c);
        break;
    }

    case SimTask::Type::InfiniteStepMS: {
        m_runningInfinite.store(true, std::memory_order_relaxed);
        uint64_t c = 0;
        bool hitSWI = false;
        const int delayMs = task.stepCount > 0 ? task.stepCount : 1;

        while (!m_stopInfinite.load(std::memory_order_relaxed)
            && m_running.load(std::memory_order_relaxed)) {
                {
                    std::lock_guard<std::mutex> simLk(m_simMutex);
                    c = m_cpu.step();
                    if (m_cpu.m_halted) { hitSWI = true; break; }
                }

                pushResult(SimTask::Type::InfiniteStepMS, true,
                    "Running... cycle " + std::to_string(c), c);

                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        m_runningInfinite.store(false, std::memory_order_relaxed);
        {
            std::lock_guard<std::mutex> simLk(m_simMutex);
            c = m_cpu.cycleCount();
        }
        std::string msg = hitSWI
            ? "SWI reached at cycle " + std::to_string(c) + ". CPU halted."
            : "StepMS stopped at cycle " + std::to_string(c) + ".";
        pushResult(task.type, true, msg, c);
        break;
    }
    
    case SimTask::Type::StopInfinite: {
        m_stopInfinite.store(true);
        break;
    }

    } // switch
}