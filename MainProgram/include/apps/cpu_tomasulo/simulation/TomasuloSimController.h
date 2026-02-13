#pragma once

/**
 * @file TomasuloSimController.h
 * @brief Asynchronous simulation controller for Tomasulo CPU.
 *
 * Owns the TomasuloCPU and processes all mutations on a dedicated
 * worker thread, keeping the UI thread responsive.
 *
 * Thread model:
 *   - UI thread dispatches tasks via request*() methods
 *   - Worker thread processes tasks sequentially
 *   - UI thread polls results via hasResult() / consumeResult()
 *   - UI thread locks mutex() during render to read simulation state
 *
 * @note
 *   - SRP: Only coordinates async task dispatch and result delivery
 *   - OCP: New task types added by extending SimTask::Type
 *   - DIP: UI depends on this controller, not on raw CPU internals
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

 /**
  * @struct SimTask
  * @brief Describes a unit of work for the simulation worker thread.
  */
struct SimTask {
    enum class Type { Compile, LoadBinary, Reset, ResetRAM};

    Type        type;
    std::string source;     ///< Assembly text   (Compile only)
    std::string filePath;   ///< .bin file path   (LoadBinary only)
};

/**
 * @struct SimTaskResult
 * @brief Outcome of a processed SimTask.
 */
struct SimTaskResult {
    SimTask::Type type;
    bool          success = false;
    std::string   message;
};

// ════════════════════════════════════════════════════════════════
// Controller
// ════════════════════════════════════════════════════════════════

/**
 * @class TomasuloSimController
 * @brief Manages the simulation worker thread and task queue.
 */
class TomasuloSimController {
public:
    TomasuloSimController() = default;
    ~TomasuloSimController();

    TomasuloSimController(const TomasuloSimController&) = delete;
    TomasuloSimController& operator=(const TomasuloSimController&) = delete;

    // ── Lifecycle ───────────────────────────────────────────────

    /** @brief Spawns the worker thread. Call once after construction. */
    void start();

    /** @brief Signals shutdown and joins the worker thread. */
    void stop();

    // ── Task dispatch (called from UI thread) ───────────────────

    /** @brief Queue an assembly + load task. */
    void requestCompile(const std::string& source);

    /**
     * @brief Queue a binary-file load into RAM.
     *
     * The worker reads the .bin file (little-endian uint64 words)
     * and writes them into RAM starting at the current UPPER register
     * value, stopping at the end of RAM.
     */
    void requestLoadBinary(const std::string& filePath);

    /** @brief Queue a full CPU reset (RAM + registers). */
    void requestReset();

    /** @brief Queue a RAM-only reset (RAM only). */
    void requestResetRAM();

    // ── Result polling (called from UI thread) ──────────────────

    /** @brief True if a result is waiting to be consumed. */
    bool hasResult() const;

    /** @brief Returns and clears the pending result. */
    SimTaskResult consumeResult();

    /** @brief True if the worker is currently processing a task. */
    bool isBusy() const;

    // ── Thread-safe simulation access ───────────────────────────

    /**
     * @brief Mutex protecting the simulation state.
     *
     * The UI thread MUST hold this lock while reading any data
     * from cpu() (RAM contents, register values, etc.).
     * The worker thread holds this lock during task processing.
     */
    std::mutex& mutex();

    /** @brief Direct access to the simulation object. */
    TomasuloCPU& cpu();
    const TomasuloCPU& cpu() const;

private:
    void workerLoop();
    void processTask(const SimTask& task);

    // ── Simulation ──────────────────────────────────────────────
    TomasuloCPU m_cpu;
    std::mutex  m_simMutex;           ///< Guards m_cpu state

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
};