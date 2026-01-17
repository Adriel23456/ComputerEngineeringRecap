// ============================================================================
// File: include/apps/quicksort_visualizer/threading/LogicThreadController.h
// ============================================================================

#pragma once

/**
 * @file LogicThreadController.h
 * @brief Controller for the OpenMP-based logic thread.
 *
 * Manages the lifecycle of the logic thread that will execute the quicksort
 * algorithm. The main thread handles visualization while this controller
 * manages algorithm execution on a separate thread.
 *
 * @note Design Principles:
 *   - SRP: Only manages thread lifecycle and coordination
 *   - OCP: Can be extended for different algorithms without modification
 *   - DIP: Visualization depends on abstract thread interface
 */

#include <atomic>
#include <functional>
#include <string>

namespace quicksort {

    /**
     * @enum ThreadState
     * @brief Represents the current state of the logic thread.
     */
    enum class ThreadState {
        Uninitialized,  ///< Thread has not been created yet
        Idle,           ///< Thread is ready but not executing
        Running,        ///< Thread is actively executing algorithm
        Paused,         ///< Thread execution is paused
        Stopped,        ///< Thread has been stopped
        Error           ///< Thread encountered an error
    };

    /**
     * @class LogicThreadController
     * @brief Manages the OpenMP logic thread for algorithm execution.
     *
     * This class provides:
     * - Thread initialization and verification
     * - Thread-safe state management using atomics
     * - Safe console logging without race conditions
     * - Coordination between main (render) and logic threads
     *
     * Thread Safety:
     * - All public methods are thread-safe
     * - State is managed using std::atomic
     * - Console output uses OpenMP critical sections
     */
    class LogicThreadController {
    public:
        // ========================================================================
        // Construction / Destruction
        // ========================================================================

        /**
         * @brief Constructs the logic thread controller.
         *
         * Does not start the thread - call initialize() to start.
         */
        LogicThreadController();

        /**
         * @brief Destructor - ensures thread is properly stopped.
         */
        ~LogicThreadController();

        // Prevent copying - controller owns thread resources
        LogicThreadController(const LogicThreadController&) = delete;
        LogicThreadController& operator=(const LogicThreadController&) = delete;

        // ========================================================================
        // Thread Lifecycle
        // ========================================================================

        /**
         * @brief Initializes the OpenMP logic thread.
         *
         * Spawns the logic thread and verifies successful initialization.
         * Thread-safe debug messages are printed to console.
         *
         * @return true if thread initialized successfully.
         */
        bool initialize();

        /**
         * @brief Shuts down the logic thread.
         *
         * Signals the thread to stop and waits for completion.
         */
        void shutdown();

        /**
         * @brief Checks if the logic thread is initialized and ready.
         * @return true if thread is initialized and not in error state.
         */
        bool isInitialized() const;

        // ========================================================================
        // State Management
        // ========================================================================

        /**
         * @brief Gets the current thread state.
         * @return Current ThreadState value.
         */
        ThreadState getState() const;

        /**
         * @brief Gets a human-readable string for the current state.
         * @return State description string.
         */
        std::string getStateString() const;

        /**
         * @brief Checks if the thread is currently running an algorithm.
         * @return true if thread state is Running.
         */
        bool isRunning() const;

        // ========================================================================
        // OpenMP Information
        // ========================================================================

        /**
         * @brief Gets the number of available OpenMP threads.
         * @return Number of threads OpenMP can use.
         */
        static int getAvailableThreadCount();

        /**
         * @brief Gets the OpenMP version string.
         * @return OpenMP version information.
         */
        static std::string getOpenMPVersion();

        // ========================================================================
        // Thread-Safe Logging
        // ========================================================================

        /**
         * @brief Logs a message to console in a thread-safe manner.
         *
         * Uses OpenMP critical section to prevent interleaved output.
         *
         * @param message The message to log.
         * @param prefix Optional prefix (default: "[LogicThread]").
         */
        static void threadSafeLog(const std::string& message,
            const std::string& prefix = "[LogicThread]");

    private:
        // ========================================================================
        // Internal State
        // ========================================================================

        /**
         * @brief Sets the thread state atomically.
         * @param newState The new state to set.
         */
        void setState(ThreadState newState);

        /**
         * @brief Performs OpenMP environment verification.
         * @return true if OpenMP is properly configured.
         */
        bool verifyOpenMPEnvironment();

        /**
         * @brief Logs OpenMP configuration details.
         */
        void logOpenMPConfiguration();

        // ========================================================================
        // Data Members
        // ========================================================================

        std::atomic<ThreadState> m_state;       ///< Current thread state (atomic for thread safety)
        std::atomic<bool> m_shutdownRequested;  ///< Flag to signal thread shutdown
        std::atomic<int> m_logicThreadId;       ///< ID of the logic thread (set by OpenMP)
    };

} // namespace quicksort