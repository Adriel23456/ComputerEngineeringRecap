// ============================================================================
// File: src/apps/quicksort_visualizer/data/SwapQueue.cpp
// ============================================================================

/**
 * @file SwapQueue.cpp
 * @brief Implementation of the SwapQueue class.
 */

#include "apps/quicksort_visualizer/data/SwapQueue.h"

namespace quicksort {
    namespace data {

        // ============================================================================
        // Construction
        // ============================================================================

        SwapQueue::SwapQueue()
            : m_queue()
            , m_mutex()
            , m_sortingComplete(false)
            , m_totalSwaps(0)
            , m_swapCount(0)
        {
        }

        // ============================================================================
        // Producer Operations
        // ============================================================================

        void SwapQueue::push(const SwapOperation& swap) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(swap);
            m_swapCount++;
        }

        void SwapQueue::markComplete() {
            m_sortingComplete.store(true);
        }

        void SwapQueue::setTotalSwaps(uint32_t total) {
            m_totalSwaps.store(total);
        }

        // ============================================================================
        // Consumer Operations
        // ============================================================================

        std::optional<SwapOperation> SwapQueue::tryPop() {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (m_queue.empty()) {
                return std::nullopt;
            }

            SwapOperation swap = m_queue.front();
            m_queue.pop();
            return swap;
        }

        bool SwapQueue::isEmpty() const {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        size_t SwapQueue::size() const {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.size();
        }

        bool SwapQueue::isFullyProcessed() const {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_sortingComplete.load() && m_queue.empty();
        }

        bool SwapQueue::isSortingComplete() const {
            return m_sortingComplete.load();
        }

        // ============================================================================
        // Reset
        // ============================================================================

        void SwapQueue::reset() {
            std::lock_guard<std::mutex> lock(m_mutex);

            // Clear the queue
            while (!m_queue.empty()) {
                m_queue.pop();
            }

            m_sortingComplete.store(false);
            m_totalSwaps.store(0);
            m_swapCount.store(0);
        }

    } // namespace data
} // namespace quicksort