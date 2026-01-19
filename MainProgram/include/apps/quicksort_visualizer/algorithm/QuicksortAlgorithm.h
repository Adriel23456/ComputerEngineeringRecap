// ============================================================================
// File: include/apps/quicksort_visualizer/algorithm/QuicksortAlgorithm.h
// ============================================================================

#pragma once

/**
 * @file QuicksortAlgorithm.h
 * @brief Quicksort algorithm implementation for visualization.
 *
 * Implements the quicksort algorithm that records swap operations
 * into a thread-safe queue for visualization.
 *
 * @note Design Principles:
 *   - SRP: Only implements sorting logic
 *   - DIP: Depends on SwapQueue abstraction
 *   - Runs on logic thread (OpenMP)
 */

#include "apps/quicksort_visualizer/data/SwapQueue.h"
#include <vector>
#include <atomic>
#include <cstdint>

namespace quicksort {
    namespace algorithm {

        /**
         * @class QuicksortAlgorithm
         * @brief Executes quicksort and records swaps for visualization.
         */
        class QuicksortAlgorithm {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs the algorithm with a swap queue.
             * @param swapQueue Reference to the queue for recording swaps.
             */
            explicit QuicksortAlgorithm(data::SwapQueue& swapQueue);

            /**
             * @brief Destructor.
             */
            ~QuicksortAlgorithm() = default;

            // Non-copyable
            QuicksortAlgorithm(const QuicksortAlgorithm&) = delete;
            QuicksortAlgorithm& operator=(const QuicksortAlgorithm&) = delete;

            // ========================================================================
            // Sorting
            // ========================================================================

            /**
             * @brief Executes quicksort on the given values.
             *
             * This method runs on the logic thread and records all swaps
             * to the swap queue for visualization.
             *
             * @param values Vector of values to sort.
             */
            void execute(std::vector<double>& values);

            /**
             * @brief Requests cancellation of the sorting.
             */
            void requestCancel();

            /**
             * @brief Checks if sorting was cancelled.
             * @return true if cancelled.
             */
            bool isCancelled() const;

            /**
             * @brief Gets the total number of swaps performed.
             * @return Swap count.
             */
            uint32_t getSwapCount() const;

        private:
            // ========================================================================
            // Internal Sorting Methods
            // ========================================================================

            /**
             * @brief Recursive quicksort implementation.
             * @param values The array to sort.
             * @param low Starting index.
             * @param high Ending index.
             */
            void quicksort(std::vector<double>& values, int low, int high);

            /**
             * @brief Partition function for quicksort.
             * @param values The array to partition.
             * @param low Starting index.
             * @param high Ending index.
             * @return Partition index.
             */
            int partition(std::vector<double>& values, int low, int high);

            /**
             * @brief Records and performs a swap.
             * @param values The array.
             * @param indexA First index.
             * @param indexB Second index.
             */
            void recordSwap(std::vector<double>& values, uint32_t indexA, uint32_t indexB);

            /**
             * @brief Estimates total swaps for average case.
             * @param n Number of elements.
             * @return Estimated swap count.
             */
            uint32_t estimateSwaps(uint32_t n) const;

            // ========================================================================
            // Data Members
            // ========================================================================

            data::SwapQueue& m_swapQueue;        ///< Reference to swap queue
            std::atomic<bool> m_cancelled;       ///< Cancellation flag
            std::atomic<uint32_t> m_swapCount;   ///< Current swap count
            uint32_t m_estimatedSwaps;           ///< Estimated total swaps
        };

    } // namespace algorithm
} // namespace quicksort