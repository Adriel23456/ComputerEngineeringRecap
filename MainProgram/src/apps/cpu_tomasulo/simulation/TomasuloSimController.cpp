/**
 * @file TomasuloSimController.cpp
 * @brief Implementation of TomasuloSimController.
 *
 * Worker thread processes tasks sequentially. OpenMP is available
 * within the worker for any parallel computation inside tasks.
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

    std::cout << "[SimController] Worker thread started (thread + OpenMP ready).\n";
}

void TomasuloSimController::stop() {
    if (!m_running.load()) return;

    m_running.store(false);
    m_queueCV.notify_all();

    if (m_worker.joinable()) {
        m_worker.join();
    }

    std::cout << "[SimController] Worker thread stopped.\n";
}

// ============================================================================
// Task Dispatch  (called from UI thread)
// ============================================================================

void TomasuloSimController::requestCompile(const std::string& source) {
    SimTask task;
    task.type = SimTask::Type::Compile;
    task.source = source;

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_taskQueue.push(std::move(task));
    }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestLoadBinary(const std::string& filePath) {
    SimTask task;
    task.type = SimTask::Type::LoadBinary;
    task.filePath = filePath;

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_taskQueue.push(std::move(task));
    }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestReset() {
    SimTask task;
    task.type = SimTask::Type::Reset;

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_taskQueue.push(std::move(task));
    }
    m_queueCV.notify_one();
}

void TomasuloSimController::requestResetRAM() {
    SimTask task;
    task.type = SimTask::Type::ResetRAM;

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_taskQueue.push(std::move(task));
    }
    m_queueCV.notify_one();
}

// ============================================================================
// Result Polling  (called from UI thread)
// ============================================================================

bool TomasuloSimController::hasResult() const {
    return m_hasResult.load();
}

SimTaskResult TomasuloSimController::consumeResult() {
    std::lock_guard<std::mutex> lock(m_resultMutex);
    m_hasResult.store(false);
    return m_lastResult;
}

bool TomasuloSimController::isBusy() const {
    return m_busy.load();
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

    // OpenMP is available inside this thread for parallel work
    omp_set_num_threads(omp_get_max_threads());

    while (m_running.load()) {

        SimTask task;

        // Wait for a task
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_queueCV.wait(lock, [this] {
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

    SimTaskResult result;
    result.type = task.type;

    switch (task.type) {

        // ── Compile ─────────────────────────────────────────────────
    case SimTask::Type::Compile: {
        std::lock_guard<std::mutex> simLock(m_simMutex);

        AssemblyResult asmResult = m_cpu.compile(task.source);
        result.success = asmResult.success;
        result.message = asmResult.summary;
        break;
    }

                               // ── Load Binary ─────────────────────────────────────────────
    case SimTask::Type::LoadBinary: {
        try {
            // 1. Read .bin file
            std::ifstream file(task.filePath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                result.success = false;
                result.message = "Error: Could not open file.";
                break;
            }

            auto fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<uint8_t> rawBytes(static_cast<size_t>(fileSize));
            file.read(reinterpret_cast<char*>(rawBytes.data()),
                static_cast<std::streamsize>(fileSize));
            file.close();

            // 2. Parse little-endian uint64 words
            size_t totalWords = rawBytes.size() / 8;
            std::vector<uint64_t> words(totalWords, 0);

            // OpenMP parallel for: parse bytes → words
            #pragma omp parallel for schedule(static)
            for (int i = 0; i < static_cast<int>(totalWords); ++i) {
                size_t offset = static_cast<size_t>(i) * 8;
                uint64_t w = 0;
                for (int b = 0; b < 8; ++b) {
                    w |= static_cast<uint64_t>(rawBytes[offset + b]) << (b * 8);
                }
                words[i] = w;
            }

            // 3. Lock simulation and write into RAM at UPPER address
            std::lock_guard<std::mutex> simLock(m_simMutex);

            uint64_t startAddr = m_cpu.registers().get(TomasuloRegisterFile::UPPER);
            size_t   startRow = static_cast<size_t>(startAddr / TomasuloRAM::kStep);
            size_t   maxRows = TomasuloRAM::kTotalRows;

            size_t written = 0;
            for (size_t i = 0; i < words.size(); ++i) {
                size_t row = startRow + i;
                if (row >= maxRows) break;   // RAM full — stop
                m_cpu.ram().write(row, words[i]);
                ++written;
            }

            std::ostringstream oss;
            oss << "Success - Loaded " << written << " word(s) from 0x"
                << std::hex << startAddr << std::dec << ".";
            result.success = true;
            result.message = oss.str();

        }
        catch (const std::exception& e) {
            result.success = false;
            result.message = std::string("Load error: ") + e.what();
        }
        break;
    }

    // ── Reset ───────────────────────────────────────────────────
    case SimTask::Type::Reset: {
        std::lock_guard<std::mutex> simLock(m_simMutex);
        m_cpu.reset();
        result.success = true;
        result.message = "Success - CPU reset.";
        break;
    }

    case SimTask::Type::ResetRAM: {
        std::lock_guard<std::mutex> simLock(m_simMutex);
        m_cpu.resetRAMOnly();
        result.success = true;
        result.message = "Success - RAM reset.";
        break;
    }

    } // switch

    // Store result for UI polling
    {
        std::lock_guard<std::mutex> rLock(m_resultMutex);
        m_lastResult = std::move(result);
        m_hasResult.store(true);
    }
}