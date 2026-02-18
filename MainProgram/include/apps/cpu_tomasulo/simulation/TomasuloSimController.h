#pragma once

#include "apps/cpu_tomasulo/simulation/TomasuloCPU.h"

#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

// ── Task / Result DTOs ──────────────────────────────────────────

struct SimTask {
    enum class Type {
        Compile, LoadBinary, Reset, ResetRAM,
        Step, StepUntil, InfiniteStep, InfiniteStepMS, StopInfinite
    };

    Type        type;
    std::string source;
    std::string filePath;
    int         stepCount = 1;
};

struct SimTaskResult {
    SimTask::Type type;
    bool          success = false;
    std::string   message;
    uint64_t      cycleCount = 0;
};

// ── Controller ──────────────────────────────────────────────────

class TomasuloSimController {
public:
    TomasuloSimController() = default;
    ~TomasuloSimController();

    TomasuloSimController(const TomasuloSimController&) = delete;
    TomasuloSimController& operator=(const TomasuloSimController&) = delete;

    void start();
    void stop();

    void requestCompile(const std::string& source);
    void requestLoadBinary(const std::string& filePath);
    void requestReset();
    void requestResetRAM();
    void requestStep();
    void requestStepUntil(int count);
    void requestInfiniteStep();
    void requestInfiniteStepMS(int delayMs);
    void requestStopInfinite();

    bool hasResult() const;
    SimTaskResult consumeResult();
    bool isBusy() const;
    bool isRunningInfinite() const { return m_runningInfinite.load(std::memory_order_relaxed); }

    std::mutex& mutex();
    TomasuloCPU& cpu();
    const TomasuloCPU& cpu() const;

private:
    void workerLoop();
    void processTask(const SimTask& task);

    void pushResult(SimTask::Type type, bool success,
        const std::string& msg, uint64_t cycles = 0);

    // ── Configuration ───────────────────────────────────────────
    // How many cycles to batch before yielding the lock to the UI.
    // Higher = faster simulation, lower = more responsive UI.
    static constexpr int kCyclesPerBatch = 64;

    // ── Simulation ──────────────────────────────────────────────
    TomasuloCPU m_cpu;
    std::mutex  m_simMutex;

    // ── Task queue ──────────────────────────────────────────────
    std::queue<SimTask>     m_taskQueue;
    std::mutex              m_queueMutex;
    std::condition_variable m_queueCV;

    // ── Results ─────────────────────────────────────────────────
    SimTaskResult       m_lastResult;
    std::mutex          m_resultMutex;
    std::atomic<bool>   m_hasResult{ false };
    std::atomic<bool>   m_busy{ false };

    // ── Thread control ──────────────────────────────────────────
    std::atomic<bool>   m_running{ false };
    std::thread         m_worker;

    // ── InfiniteStep control ────────────────────────────────────
    std::atomic<bool>   m_stopInfinite{ false };
    std::atomic<bool>   m_runningInfinite{ false };
};