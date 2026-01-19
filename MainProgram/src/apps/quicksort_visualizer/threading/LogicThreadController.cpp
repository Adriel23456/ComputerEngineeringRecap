// ============================================================================
// File: src/apps/quicksort_visualizer/threading/LogicThreadController.cpp
// ============================================================================

/**
 * @file LogicThreadController.cpp
 * @brief Implementation of the LogicThreadController class.
 */

#include "apps/quicksort_visualizer/threading/LogicThreadController.h"
#include <iostream>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace quicksort {

    // ============================================================================
    // Construction / Destruction
    // ============================================================================

    LogicThreadController::LogicThreadController()
        : m_thread()
        , m_state(ThreadState::Uninitialized)
        , m_shouldExit(false)
        , m_hasTask(false)
        , m_mutex()
        , m_condition()
        , m_swapQueue()
        , m_algorithm(nullptr)
        , m_valuesToSort()
    {
    }

    LogicThreadController::~LogicThreadController() {
        shutdown();
    }

    // ============================================================================
    // Lifecycle Management
    // ============================================================================

    bool LogicThreadController::initialize() {
        if (m_state.load() != ThreadState::Uninitialized) {
            std::cerr << "[LogicThread] Already initialized\n";
            return false;
        }

        try {
            // Create the algorithm instance
            m_algorithm = std::make_unique<algorithm::QuicksortAlgorithm>(m_swapQueue);

            // Start the worker thread
            m_shouldExit.store(false);
            m_hasTask.store(false);
            m_thread = std::thread(&LogicThreadController::threadWorker, this);

            m_state.store(ThreadState::Idle);

#ifdef _OPENMP
            std::cout << "[LogicThread] Initialized with OpenMP support (max threads: "
                << omp_get_max_threads() << ")\n";
#else
            std::cout << "[LogicThread] Initialized (OpenMP not available)\n";
#endif

            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "[LogicThread] Initialization failed: " << e.what() << "\n";
            m_state.store(ThreadState::Error);
            return false;
        }
    }

    void LogicThreadController::shutdown() {
        if (m_state.load() == ThreadState::Uninitialized) {
            return;
        }

        std::cout << "[LogicThread] Shutting down...\n";

        // Signal thread to exit
        m_state.store(ThreadState::ShuttingDown);
        m_shouldExit.store(true);

        // Cancel any running sorting
        if (m_algorithm) {
            m_algorithm->requestCancel();
        }

        // Wake up thread if waiting
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_hasTask.store(true);
        }
        m_condition.notify_one();

        // Wait for thread to finish
        if (m_thread.joinable()) {
            m_thread.join();
        }

        m_algorithm.reset();
        m_state.store(ThreadState::Uninitialized);

        std::cout << "[LogicThread] Shutdown complete\n";
    }

    // ============================================================================
    // Sorting Control
    // ============================================================================

    bool LogicThreadController::startSorting(const std::vector<double>& values) {
        ThreadState currentState = m_state.load();

        if (currentState != ThreadState::Idle && currentState != ThreadState::Completed) {
            std::cerr << "[LogicThread] Cannot start sorting - thread not ready\n";
            return false;
        }

        if (values.empty()) {
            std::cerr << "[LogicThread] Cannot sort empty array\n";
            return false;
        }

        // Reset state
        m_swapQueue.reset();

        // Copy values to sort
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_valuesToSort = values;
            m_hasTask.store(true);
        }

        m_state.store(ThreadState::Running);
        m_condition.notify_one();

        std::cout << "[LogicThread] Started sorting " << values.size() << " elements\n";
        return true;
    }

    void LogicThreadController::cancelSorting() {
        if (m_algorithm) {
            m_algorithm->requestCancel();
        }
    }

    bool LogicThreadController::isSorting() const {
        return m_state.load() == ThreadState::Running;
    }

    bool LogicThreadController::isCompleted() const {
        return m_state.load() == ThreadState::Completed;
    }

    // ============================================================================
    // State Access
    // ============================================================================

    ThreadState LogicThreadController::getState() const {
        return m_state.load();
    }

    data::SwapQueue& LogicThreadController::getSwapQueue() {
        return m_swapQueue;
    }

    const data::SwapQueue& LogicThreadController::getSwapQueue() const {
        return m_swapQueue;
    }

    void LogicThreadController::reset() {
        // Cancel any running sorting first
        cancelSorting();

        // Wait for completion if running
        while (m_state.load() == ThreadState::Running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // Reset queue
        m_swapQueue.reset();

        // Set back to idle
        if (m_state.load() == ThreadState::Completed) {
            m_state.store(ThreadState::Idle);
        }
    }

    // ============================================================================
    // Internal Methods
    // ============================================================================

    void LogicThreadController::threadWorker() {
        std::cout << "[LogicThread] Worker thread started\n";

        while (!m_shouldExit.load()) {
            // Wait for task
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_condition.wait(lock, [this] {
                    return m_hasTask.load() || m_shouldExit.load();
                    });
            }

            if (m_shouldExit.load()) {
                break;
            }

            if (m_hasTask.load()) {
                m_hasTask.store(false);
                executeSorting();
            }
        }

        std::cout << "[LogicThread] Worker thread exiting\n";
    }

    void LogicThreadController::executeSorting() {
        if (!m_algorithm) {
            m_state.store(ThreadState::Error);
            return;
        }

        try {
            // Execute quicksort
            m_algorithm->execute(m_valuesToSort);

            // Check if cancelled
            if (m_algorithm->isCancelled()) {
                std::cout << "[LogicThread] Sorting cancelled after "
                    << m_algorithm->getSwapCount() << " swaps\n";
            }
            else {
                std::cout << "[LogicThread] Sorting completed with "
                    << m_algorithm->getSwapCount() << " swaps\n";
            }

            m_state.store(ThreadState::Completed);
        }
        catch (const std::exception& e) {
            std::cerr << "[LogicThread] Sorting error: " << e.what() << "\n";
            m_state.store(ThreadState::Error);
        }
    }

} // namespace quicksort