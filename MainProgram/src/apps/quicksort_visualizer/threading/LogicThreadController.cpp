// ============================================================================
// File: src/apps/quicksort_visualizer/threading/LogicThreadController.cpp
// ============================================================================

/**
 * @file LogicThreadController.cpp
 * @brief Implementation of the LogicThreadController class.
 *
 * Provides OpenMP-based thread management for the quicksort algorithm.
 * All console output is protected by critical sections to ensure
 * thread-safe logging without race conditions.
 */

#include "apps/quicksort_visualizer/threading/LogicThreadController.h"
#include <omp.h>
#include <iostream>
#include <sstream>

namespace quicksort {

    // ============================================================================
    // Construction / Destruction
    // ============================================================================

    LogicThreadController::LogicThreadController()
        : m_state(ThreadState::Uninitialized)
        , m_shutdownRequested(false)
        , m_logicThreadId(-1)
    {
        threadSafeLog("Controller created.", "[LogicThread]");
    }

    LogicThreadController::~LogicThreadController() {
        shutdown();
        threadSafeLog("Controller destroyed.", "[LogicThread]");
    }

    // ============================================================================
    // Thread Lifecycle
    // ============================================================================

    bool LogicThreadController::initialize() {
        // Prevent re-initialization
        if (m_state.load() != ThreadState::Uninitialized) {
            threadSafeLog("WARNING: Attempted to initialize already initialized thread.");
            return false;
        }

        threadSafeLog("========================================");
        threadSafeLog("Initializing OpenMP Logic Thread");
        threadSafeLog("========================================");

        // Verify OpenMP environment first
        if (!verifyOpenMPEnvironment()) {
            setState(ThreadState::Error);
            threadSafeLog("ERROR: OpenMP environment verification failed!");
            return false;
        }

        // Log OpenMP configuration
        logOpenMPConfiguration();

        // Test OpenMP parallel execution
        threadSafeLog("Testing OpenMP parallel execution...");

        bool initSuccess = false;

        // Use OpenMP parallel section to verify threading works
#pragma omp parallel num_threads(2)
        {
            int threadId = omp_get_thread_num();
            int totalThreads = omp_get_num_threads();

#pragma omp critical(console_output)
            {
                std::cout << "[LogicThread] Thread " << threadId
                    << " of " << totalThreads << " reporting." << std::endl;
            }

            // Thread 0 is the main thread, Thread 1 will be our logic thread
            if (threadId == 1) {
#pragma omp critical(console_output)
                {
                    std::cout << "[LogicThread] Logic thread (ID: " << threadId
                        << ") initialized successfully!" << std::endl;
                }
            }

            // Synchronize all threads before continuing
#pragma omp barrier

#pragma omp single
            {
                initSuccess = true;
            }
        }

        if (initSuccess) {
            setState(ThreadState::Idle);
            threadSafeLog("========================================");
            threadSafeLog("OpenMP Logic Thread READY");
            threadSafeLog("State: Idle (waiting for commands)");
            threadSafeLog("========================================");
            return true;
        }
        else {
            setState(ThreadState::Error);
            threadSafeLog("ERROR: Failed to initialize OpenMP parallel region!");
            return false;
        }
    }

    void LogicThreadController::shutdown() {
        ThreadState currentState = m_state.load();

        if (currentState == ThreadState::Uninitialized ||
            currentState == ThreadState::Stopped) {
            return; // Nothing to shutdown
        }

        threadSafeLog("Shutting down logic thread...");
        m_shutdownRequested.store(true);

        // Wait for any running operations to complete
        // (In future steps, this will signal the sorting algorithm to stop)

        setState(ThreadState::Stopped);
        threadSafeLog("Logic thread shutdown complete.");
    }

    bool LogicThreadController::isInitialized() const {
        ThreadState state = m_state.load();
        return state != ThreadState::Uninitialized && state != ThreadState::Error;
    }

    // ============================================================================
    // State Management
    // ============================================================================

    ThreadState LogicThreadController::getState() const {
        return m_state.load();
    }

    std::string LogicThreadController::getStateString() const {
        switch (m_state.load()) {
        case ThreadState::Uninitialized: return "Uninitialized";
        case ThreadState::Idle:          return "Idle";
        case ThreadState::Running:       return "Running";
        case ThreadState::Paused:        return "Paused";
        case ThreadState::Stopped:       return "Stopped";
        case ThreadState::Error:         return "Error";
        default:                         return "Unknown";
        }
    }

    bool LogicThreadController::isRunning() const {
        return m_state.load() == ThreadState::Running;
    }

    void LogicThreadController::setState(ThreadState newState) {
        ThreadState oldState = m_state.exchange(newState);

        if (oldState != newState) {
            std::ostringstream oss;
            oss << "State transition: ";

            // Convert old state to string
            switch (oldState) {
            case ThreadState::Uninitialized: oss << "Uninitialized"; break;
            case ThreadState::Idle:          oss << "Idle"; break;
            case ThreadState::Running:       oss << "Running"; break;
            case ThreadState::Paused:        oss << "Paused"; break;
            case ThreadState::Stopped:       oss << "Stopped"; break;
            case ThreadState::Error:         oss << "Error"; break;
            default:                         oss << "Unknown"; break;
            }

            oss << " -> ";

            // Convert new state to string
            switch (newState) {
            case ThreadState::Uninitialized: oss << "Uninitialized"; break;
            case ThreadState::Idle:          oss << "Idle"; break;
            case ThreadState::Running:       oss << "Running"; break;
            case ThreadState::Paused:        oss << "Paused"; break;
            case ThreadState::Stopped:       oss << "Stopped"; break;
            case ThreadState::Error:         oss << "Error"; break;
            default:                         oss << "Unknown"; break;
            }

            threadSafeLog(oss.str());
        }
    }

    // ============================================================================
    // OpenMP Information
    // ============================================================================

    int LogicThreadController::getAvailableThreadCount() {
        return omp_get_max_threads();
    }

    std::string LogicThreadController::getOpenMPVersion() {
        std::ostringstream oss;

        // OpenMP version is encoded in _OPENMP macro as YYYYMM
#ifdef _OPENMP
        int version = _OPENMP;
        int year = version / 100;
        int month = version % 100;
        oss << "OpenMP " << year << "." << month;

        // Add human-readable version
        if (version >= 201811) {
            oss << " (5.0+)";
        }
        else if (version >= 201511) {
            oss << " (4.5)";
        }
        else if (version >= 201307) {
            oss << " (4.0)";
        }
        else if (version >= 201107) {
            oss << " (3.1)";
        }
        else if (version >= 200805) {
            oss << " (3.0)";
        }
        else if (version >= 200505) {
            oss << " (2.5)";
        }
        else {
            oss << " (2.0 or earlier)";
        }
#else
        oss << "OpenMP not available";
#endif

        return oss.str();
    }

    // ============================================================================
    // Thread-Safe Logging
    // ============================================================================

    void LogicThreadController::threadSafeLog(const std::string& message,
        const std::string& prefix) {
        // Use OpenMP critical section to ensure atomic console output
        // This prevents interleaved output from multiple threads
#pragma omp critical(console_output)
            {
                std::cout << prefix << " " << message << std::endl;
            }
    }

    // ============================================================================
    // Internal Helpers
    // ============================================================================

    bool LogicThreadController::verifyOpenMPEnvironment() {
#ifndef _OPENMP
        threadSafeLog("ERROR: OpenMP is not enabled in this build!");
        threadSafeLog("Please ensure OpenMP is enabled in CMakeLists.txt");
        return false;
#endif

        // Verify we can get thread information
        int maxThreads = omp_get_max_threads();
        if (maxThreads < 1) {
            threadSafeLog("ERROR: OpenMP reports 0 available threads!");
            return false;
        }

        // Verify parallel execution is possible
        bool parallelEnabled = (omp_get_max_threads() > 1) ||
            (omp_get_num_procs() >= 1);

        if (!parallelEnabled) {
            threadSafeLog("WARNING: Parallel execution may not be available.");
        }

        return true;
    }

    void LogicThreadController::logOpenMPConfiguration() {
        threadSafeLog("--- OpenMP Configuration ---");

        std::ostringstream oss;

        // Version
        oss << "Version: " << getOpenMPVersion();
        threadSafeLog(oss.str());
        oss.str("");

        // Max threads
        oss << "Max threads: " << omp_get_max_threads();
        threadSafeLog(oss.str());
        oss.str("");

        // Number of processors
        oss << "Available processors: " << omp_get_num_procs();
        threadSafeLog(oss.str());
        oss.str("");

        // Dynamic adjustment
        oss << "Dynamic threads: " << (omp_get_dynamic() ? "enabled" : "disabled");
        threadSafeLog(oss.str());
        oss.str("");

        // Nested parallelism
        oss << "Nested parallelism: " << (omp_get_nested() ? "enabled" : "disabled");
        threadSafeLog(oss.str());

        threadSafeLog("----------------------------");
    }

} // namespace quicksort