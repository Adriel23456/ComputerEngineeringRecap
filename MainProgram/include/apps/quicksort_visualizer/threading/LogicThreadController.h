// ============================================================================
// File: include/apps/quicksort_visualizer/threading/LogicThreadController.h
// ============================================================================

#pragma once

/**
 * @file LogicThreadController.h
 * @brief Manages the OpenMP-based logic thread for sorting algorithms.
 *
 * Provides a dedicated thread for running sorting algorithms while
 * the main thread handles UI and rendering.
 *
 * @note Design Principles:
 *   - SRP: Only manages thread lifecycle and task execution
 *   - DIP: Depends on abstractions (SwapQueue, QuicksortAlgorithm)
 */

#include "apps/quicksort_visualizer/data/SwapQueue.h"
#include "apps/quicksort_visualizer/algorithm/QuicksortAlgorithm.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>

namespace quicksort {

    /**
     * @enum ThreadState
     * @brief Current state of the logic thread.
     */
    enum class ThreadState {
        Uninitialized,  ///< Thread not yet created
        Idle,           ///< Thread waiting for work
        Running,        ///< Thread executing sorting
        Completed,      ///< Sorting finished
        ShuttingDown,   ///< Thread shutting down
        Error           ///< Error occurred
    };

    /**
     * @class LogicThreadController
     * @brief Controls the dedicated sorting thread.
     */
    class LogicThreadController {
    public:
        // ========================================================================
        // Construction / Destruction
        // ========================================================================

        LogicThreadController();
        ~LogicThreadController();

        // Non-copyable
        LogicThreadController(const LogicThreadController&) = delete;
        LogicThreadController& operator=(const LogicThreadController&) = delete;

        // ========================================================================
        // Lifecycle Management
        // ========================================================================

        /**
         * @brief Initializes the logic thread.
         * @return true if initialization succeeded.
         */
        bool initialize();

        /**
         * @brief Shuts down the logic thread gracefully.
         */
        void shutdown();

        // ========================================================================
        // Sorting Control
        // ========================================================================

        /**
         * @brief Starts sorting the given values.
         * @param values Vector of values to sort (will be copied).
         * @return true if sorting started successfully.
         */
        bool startSorting(const std::vector<double>& values);

        /**
         * @brief Requests cancellation of current sorting.
         */
        void cancelSorting();

        /**
         * @brief Checks if sorting is currently in progress.
         * @return true if sorting.
         */
        bool isSorting() const;

        /**
         * @brief Checks if sorting has completed.
         * @return true if completed.
         */
        bool isCompleted() const;

        // ========================================================================
        // State Access
        // ========================================================================

        /**
         * @brief Gets the current thread state.
         * @return Current state.
         */
        ThreadState getState() const;

        /**
         * @brief Gets the swap queue for consuming swap operations.
         * @return Reference to swap queue.
         */
        data::SwapQueue& getSwapQueue();

        /**
         * @brief Gets the swap queue (const).
         * @return Const reference to swap queue.
         */
        const data::SwapQueue& getSwapQueue() const;

        /**
         * @brief Resets the controller for a new sorting session.
         */
        void reset();

    private:
        // ========================================================================
        // Internal Methods
        // ========================================================================

        /**
         * @brief Thread worker function.
         */
        void threadWorker();

        /**
         * @brief Executes the sorting task.
         */
        void executeSorting();

        // ========================================================================
        // Data Members
        // ========================================================================

        std::thread m_thread;                    ///< The worker thread
        std::atomic<ThreadState> m_state;        ///< Current state
        std::atomic<bool> m_shouldExit;          ///< Exit signal
        std::atomic<bool> m_hasTask;             ///< Task pending flag

        std::mutex m_mutex;                      ///< Mutex for condition variable
        std::condition_variable m_condition;     ///< Condition for task signaling

        data::SwapQueue m_swapQueue;             ///< Queue for swap operations
        std::unique_ptr<algorithm::QuicksortAlgorithm> m_algorithm; ///< Sorting algorithm
        std::vector<double> m_valuesToSort;      ///< Values to sort (copied)
    };

} // namespace quicksort