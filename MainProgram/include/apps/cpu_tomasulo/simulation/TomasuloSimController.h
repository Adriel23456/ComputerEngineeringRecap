// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/TomasuloSimController.h
// ============================================================================

#pragma once

/**
 * @file TomasuloSimController.h
 * @brief Asynchronous simulation controller for the Tomasulo CPU.
 *
 * Wraps a TomasuloCPU in a dedicated worker thread so that long-running
 * operations (StepUntil, InfiniteStep) never block the render thread.
 *
 * Architecture:
 *   - The UI thread enqueues SimTask requests via requestXxx() methods.
 *   - The worker thread dequeues tasks, acquires m_simMutex, and processes them.
 *   - The UI thread polls hasResult() each frame and calls consumeResult()
 *     when a result is ready.
 *   - For data-dependent panel rendering (RAM, registers, ROB, etc.) the UI
 *     thread acquires m_simMutex via mutex() using try_lock so it never blocks
 *     the worker for a full frame.
 *
 * Batched locking for step loops:
 *   StepUntil and InfiniteStep execute kCyclesPerBatch cycles per lock
 *   acquisition, then release the lock to give the UI thread a render window.
 *
 * Task types:
 *   Compile        — assemble source and load into RAM
 *   LoadBinary     — load a .bin file into RAM at UPPER address
 *   Reset          — full CPU reset
 *   ResetRAM       — RAM-only reset
 *   Step           — single cycle
 *   StepUntil      — N cycles (batched)
 *   InfiniteStep   — run until stopped or SWI (batched, no inter-step delay)
 *   InfiniteStepMS — run until stopped or SWI (one cycle per delay interval)
 *   StopInfinite   — signal flag to terminate an in-progress infinite loop
 *
 * @note
 *   - Thread safety: TomasuloCPU is only ever accessed while holding m_simMutex.
 *   - The result queue is single-item (last write wins) — fine for UI polling.
 */

#include "apps/cpu_tomasulo/simulation/TomasuloCPU.h"

#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

 // ── Task / Result DTOs ───────────────────────────────────────────

 /** @brief Describes a unit of simulation work to be executed by the worker thread. */
struct SimTask {
    enum class Type {
        Compile,
        LoadBinary,
        Reset,
        ResetRAM,
        Step,
        StepUntil,
        InfiniteStep,
        InfiniteStepMS,
        StopInfinite
    };

    Type        type;
    std::string source;      ///< Assembly source text (Compile).
    std::string filePath;    ///< Binary file path (LoadBinary).
    int         stepCount = 1; ///< Cycle count (StepUntil) or delay ms (InfiniteStepMS).
};

/** @brief Result produced by the worker thread and consumed by the UI thread. */
struct SimTaskResult {
    SimTask::Type type;
    bool          success = false;
    std::string   message;
    uint64_t      cycleCount = 0;
};

// ── Controller ───────────────────────────────────────────────────

class TomasuloSimController {
public:
    TomasuloSimController() = default;
    ~TomasuloSimController();

    TomasuloSimController(const TomasuloSimController&) = delete;
    TomasuloSimController& operator=(const TomasuloSimController&) = delete;

    // ── Lifecycle ─────────────────────────────────────────────────
    void start();  ///< Spawns the worker thread. Call once after construction.
    void stop();   ///< Signals shutdown, stops infinite loops, joins the thread.

    // ── Task Submission ───────────────────────────────────────────
    void requestCompile(const std::string& source);
    void requestLoadBinary(const std::string& filePath);
    void requestReset();
    void requestResetRAM();
    void requestStep();
    void requestStepUntil(int count);
    void requestInfiniteStep();
    void requestInfiniteStepMS(int delayMs);
    void requestStopInfinite();

    // ── Result Polling (UI thread) ────────────────────────────────

    /** @brief Returns true if a completed result is waiting to be consumed. */
    bool          hasResult()     const;

    /** @brief Consumes and returns the last result, clearing the ready flag. */
    SimTaskResult consumeResult();

    /** @brief True while the worker is processing a task. */
    bool isBusy() const;

    /** @brief True while an InfiniteStep or InfiniteStepMS loop is running. */
    bool isRunningInfinite() const {
        return m_runningInfinite.load(std::memory_order_relaxed);
    }

    // ── Shared Mutex / CPU Access ─────────────────────────────────

    /**
     * @brief Returns the simulation mutex.
     *
     * The UI thread uses this (via try_lock) to safely read simulation state
     * for data-dependent panels without blocking the render loop.
     */
    std::mutex& mutex();

    /**
     * @brief Returns a reference to the underlying CPU.
     *
     * PRECONDITION: caller holds mutex().
     */
    TomasuloCPU& cpu();
    const TomasuloCPU& cpu() const;

private:
    // ── Worker ────────────────────────────────────────────────────
    void workerLoop();
    void processTask(const SimTask& task);

    void pushResult(SimTask::Type type, bool success,
        const std::string& msg, uint64_t cycles = 0);

    // ── Tuning ───────────────────────────────────────────────────
    /**
     * @brief Number of cycles executed per lock acquisition in batch loops.
     *
     * Higher values increase throughput; lower values improve UI responsiveness.
     */
    static constexpr int kCyclesPerBatch = 64;

    // ── Simulation ────────────────────────────────────────────────
    TomasuloCPU m_cpu;
    std::mutex  m_simMutex;

    // ── Task Queue ────────────────────────────────────────────────
    std::queue<SimTask>     m_taskQueue;
    std::mutex              m_queueMutex;
    std::condition_variable m_queueCV;

    // ── Result ────────────────────────────────────────────────────
    SimTaskResult     m_lastResult;
    std::mutex        m_resultMutex;
    std::atomic<bool> m_hasResult{ false };
    std::atomic<bool> m_busy{ false };

    // ── Thread Control ────────────────────────────────────────────
    std::atomic<bool> m_running{ false };
    std::thread       m_worker;

    // ── Infinite Loop Control ─────────────────────────────────────
    std::atomic<bool> m_stopInfinite{ false };     ///< Set by requestStopInfinite().
    std::atomic<bool> m_runningInfinite{ false };  ///< Set by InfiniteStep handlers.
};