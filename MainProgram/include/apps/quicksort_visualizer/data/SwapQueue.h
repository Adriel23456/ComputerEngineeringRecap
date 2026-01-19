// ============================================================================
// File: include/apps/quicksort_visualizer/data/SwapQueue.h
// ============================================================================

#pragma once

/**
 * @file SwapQueue.h
 * @brief Thread-safe queue for swap operations.
 *
 * Provides a producer-consumer queue for communication between
 * the sorting thread (producer) and main/render thread (consumer).
 *
 * @note Design Principles:
 *   - SRP: Only handles queue operations
 *   - Thread-safe with mutex protection
 */

#include "apps/quicksort_visualizer/data/SwapOperation.h"
#include <queue>
#include <mutex>
#include <atomic>
#include <optional>

namespace quicksort {
    namespace data {

        /**
         * @class SwapQueue
         * @brief Thread-safe queue for swap operations.
         *
         * Used for communication between:
         * - Logic thread: pushes swap operations
         * - Main thread: pops and animates swaps
         */
        class SwapQueue {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs an empty swap queue.
             */
            SwapQueue();

            /**
             * @brief Destructor.
             */
            ~SwapQueue() = default;

            // Non-copyable
            SwapQueue(const SwapQueue&) = delete;
            SwapQueue& operator=(const SwapQueue&) = delete;

            // ========================================================================
            // Producer Operations (Logic Thread)
            // ========================================================================

            /**
             * @brief Pushes a swap operation to the queue.
             * @param swap The swap operation to add.
             */
            void push(const SwapOperation& swap);

            /**
             * @brief Marks the sorting as complete.
             *
             * Called by logic thread when sorting finishes.
             */
            void markComplete();

            /**
             * @brief Sets the total expected swap count (for progress calculation).
             * @param total Estimated total swaps.
             */
            void setTotalSwaps(uint32_t total);

            // ========================================================================
            // Consumer Operations (Main Thread)
            // ========================================================================

            /**
             * @brief Tries to pop a swap operation from the queue.
             * @return The swap operation if available, std::nullopt otherwise.
             */
            std::optional<SwapOperation> tryPop();

            /**
             * @brief Checks if the queue is empty.
             * @return true if no swaps in queue.
             */
            bool isEmpty() const;

            /**
             * @brief Gets the current queue size.
             * @return Number of pending swaps.
             */
            size_t size() const;

            /**
             * @brief Checks if sorting is complete AND queue is empty.
             * @return true if all swaps have been processed.
             */
            bool isFullyProcessed() const;

            /**
             * @brief Checks if sorting algorithm has finished.
             * @return true if sorting is complete (queue may still have items).
             */
            bool isSortingComplete() const;

            // ========================================================================
            // Reset
            // ========================================================================

            /**
             * @brief Clears the queue and resets completion flag.
             */
            void reset();

        private:
            // ========================================================================
            // Data Members
            // ========================================================================

            std::queue<SwapOperation> m_queue;  ///< The swap queue
            mutable std::mutex m_mutex;          ///< Mutex for thread safety
            std::atomic<bool> m_sortingComplete; ///< Flag set when sorting finishes
            std::atomic<uint32_t> m_totalSwaps;  ///< Total expected swaps
            std::atomic<uint32_t> m_swapCount;   ///< Current swap count
        };

    } // namespace data
} // namespace quicksort