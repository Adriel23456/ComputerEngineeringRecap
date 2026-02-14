/**
 * @file TomasuloSimController.cpp
 * @brief Implementation of TomasuloSimController with step system.
 */

#include "apps/cpu_tomasulo/simulation/TomasuloSimController.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <omp.h>

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
    std::cout << "[SimController] Worker thread started.\n";
}

void TomasuloSimController::stop() {
    if (!m_running.load()) return;
    m_running.store(false);
    m_stopInfinite.store(true);
    m_queueCV.notify_all();
    if (m_worker.joinable()) m_worker.join();
    std::cout << "[SimController] Worker thread stopped.\n";
}

// ============================================================================
// Task Dispatch
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
    std::cout << "[SimController] Step requested.\n";
}

void TomasuloSimController::requestStepUntil(int count) {
    SimTask task; task.type = SimTask::Type::StepUntil; task.stepCount = count;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
    std::cout << "[SimController] StepUntil(" << count << ") requested.\n";
}

void TomasuloSimController::requestInfiniteStep() {
    m_stopInfinite.store(false);
    SimTask task; task.type = SimTask::Type::InfiniteStep;
    { std::lock_guard<std::mutex> lk(m_queueMutex); m_taskQueue.push(std::move(task)); }
    m_queueCV.notify_one();
    std::cout << "[SimController] InfiniteStep requested.\n";
}

void TomasuloSimController::requestStopInfinite() {
    m_stopInfinite.store(true);
    std::cout << "[SimController] Stop requested for InfiniteStep.\n";
}

// ============================================================================
// Result Polling
// ============================================================================

bool TomasuloSimController::hasResult() const { return m_hasResult.load(); }

SimTaskResult TomasuloSimController::consumeResult() {
    std::lock_guard<std::mutex> lk(m_resultMutex);
    m_hasResult.store(false);
    return m_lastResult;
}

bool TomasuloSimController::isBusy() const { return m_busy.load(); }

void TomasuloSimController::pushResult(SimTask::Type type, bool success,
    const std::string& msg, uint64_t cycles) {
    std::lock_guard<std::mutex> lk(m_resultMutex);
    m_lastResult.type = type;
    m_lastResult.success = success;
    m_lastResult.message = msg;
    m_lastResult.cycleCount = cycles;
    m_hasResult.store(true);
}

// ============================================================================
// Thread-safe Accessors
// ============================================================================

std::mutex& TomasuloSimController::mutex() { return m_simMutex; }
TomasuloCPU& TomasuloSimController::cpu() { return m_cpu; }
const TomasuloCPU& TomasuloSimController::cpu() const { return m_cpu; }

// ============================================================================
// Worker Loop
// ============================================================================

void TomasuloSimController::workerLoop() {
    omp_set_num_threads(omp_get_max_threads());

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
        m_busy.store(true);
        processTask(task);
        m_busy.store(false);
    }
}

// ============================================================================
// Task Processing
// ============================================================================

void TomasuloSimController::processTask(const SimTask& task) {

    switch (task.type) {

        // ── Compile ─────────────────────────────────────────────────
    case SimTask::Type::Compile: {
        std::lock_guard<std::mutex> simLk(m_simMutex);
        AssemblyResult asmResult = m_cpu.compile(task.source);
        pushResult(task.type, asmResult.success, asmResult.summary, m_cpu.cycleCount());
        break;
    }

                               // ── Load Binary ─────────────────────────────────────────────
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

#pragma omp parallel for schedule(static)
            for (int i = 0; i < static_cast<int>(totalWords); ++i) {
                size_t offset = static_cast<size_t>(i) * 8;
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

                                  // ── Reset ───────────────────────────────────────────────────
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

                                // ── Step (single cycle) ─────────────────────────────────────
    case SimTask::Type::Step: {
        std::lock_guard<std::mutex> simLk(m_simMutex);
        uint64_t c = m_cpu.step();
        std::cout << "[SimController] Step complete. Cycle = " << c << "\n";
        pushResult(task.type, true, "Step complete.", c);
        break;
    }

                            // ── StepUntil (N cycles) ────────────────────────────────────
    case SimTask::Type::StepUntil: {
        int total = task.stepCount;
        std::cout << "[SimController] StepUntil: executing " << total << " cycles...\n";
        for (int i = 0; i < total; ++i) {
            std::lock_guard<std::mutex> simLk(m_simMutex);
            m_cpu.step();
        }
        uint64_t c;
        {
            std::lock_guard<std::mutex> simLk(m_simMutex);
            c = m_cpu.cycleCount();
        }
        std::ostringstream oss;
        oss << "StepUntil complete. " << total << " cycles executed. Total: " << c;
        pushResult(task.type, true, oss.str(), c);
        std::cout << "[SimController] StepUntil done. Cycle = " << c << "\n";
        break;
    }

                                 // ── InfiniteStep ────────────────────────────────────────────
    case SimTask::Type::InfiniteStep: {
        m_runningInfinite.store(true);
        std::cout << "[SimController] InfiniteStep: running...\n";
        uint64_t c = 0;
        while (!m_stopInfinite.load() && m_running.load()) {
            {
                std::lock_guard<std::mutex> simLk(m_simMutex);
                c = m_cpu.step();
            }
            // Publish intermediate results every 100 cycles for UI
            if (c % 100 == 0) {
                pushResult(SimTask::Type::InfiniteStep, true,
                    "Running... cycle " + std::to_string(c), c);
            }
        }
        m_runningInfinite.store(false);
        {
            std::lock_guard<std::mutex> simLk(m_simMutex);
            c = m_cpu.cycleCount();
        }
        std::ostringstream oss;
        oss << "InfiniteStep stopped at cycle " << c << ".";
        pushResult(task.type, true, oss.str(), c);
        std::cout << "[SimController] InfiniteStep stopped. Cycle = " << c << "\n";
        break;
    }

                                    // ── StopInfinite (handled inline, but just in case) ─────────
    case SimTask::Type::StopInfinite: {
        m_stopInfinite.store(true);
        break;
    }

    } // switch
}