#pragma once

/**
 * @file TomasuloSimController.h
 * @brief Asynchronous simulation controller with Step / StepUntil / InfiniteStep.
 *
 * Step modes:
 *   - Step:          Execute 1 cycle, then signal completion.
 *   - StepUntil(N):  Execute N cycles sequentially.
 *   - InfiniteStep:  Execute cycles forever until StopInfinite is requested.
 *
 * All step execution happens on the worker thread.
 */

#include "apps/cpu_tomasulo/simulation/TomasuloCPU.h"

#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <omp.h>

 // ════════════════════════════════════════════════════════════════
 // Task / Result DTOs
 // ════════════════════════════════════════════════════════════════

struct SimTask {
    enum class Type {
        Compile, LoadBinary, Reset, ResetRAM,
        Step, StepUntil, InfiniteStep, StopInfinite
    };

    Type        type;
    std::string source;
    std::string filePath;
    int         stepCount = 1;   ///< For StepUntil
};

struct SimTaskResult {
    SimTask::Type type;
    bool          success = false;
    std::string   message;
    uint64_t      cycleCount = 0;  ///< Current cycle after task completes
};

// ════════════════════════════════════════════════════════════════
// Controller
// ════════════════════════════════════════════════════════════════

class TomasuloSimController {
public:
    TomasuloSimController() = default;
    ~TomasuloSimController();

    TomasuloSimController(const TomasuloSimController&) = delete;
    TomasuloSimController& operator=(const TomasuloSimController&) = delete;

    // ── Lifecycle ───────────────────────────────────────────────
    void start();
    void stop();

    // ── Task dispatch (UI thread) ───────────────────────────────
    void requestCompile(const std::string& source);
    void requestLoadBinary(const std::string& filePath);
    void requestReset();
    void requestResetRAM();
    void requestStep();
    void requestStepUntil(int count);
    void requestInfiniteStep();
    void requestStopInfinite();

    // ── Result polling (UI thread) ──────────────────────────────
    bool hasResult() const;
    SimTaskResult consumeResult();
    bool isBusy() const;

    /** @brief True if currently running InfiniteStep. */
    bool isRunningInfinite() const { return m_runningInfinite.load(); }

    // ── Thread-safe simulation access ───────────────────────────
    std::mutex& mutex();
    TomasuloCPU& cpu();
    const TomasuloCPU& cpu() const;

private:
    void workerLoop();
    void processTask(const SimTask& task);

    void pushResult(SimTask::Type type, bool success,
        const std::string& msg, uint64_t cycles = 0);

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