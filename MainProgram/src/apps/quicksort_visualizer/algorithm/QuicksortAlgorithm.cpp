// ============================================================================
// File: src/apps/quicksort_visualizer/algorithm/QuicksortAlgorithm.cpp
// ============================================================================

/**
 * @file QuicksortAlgorithm.cpp
 * @brief Implementation of the QuicksortAlgorithm class.
 */

#include "apps/quicksort_visualizer/algorithm/QuicksortAlgorithm.h"
#include <algorithm>
#include <cmath>

namespace quicksort {
    namespace algorithm {

        // ============================================================================
        // Construction
        // ============================================================================

        QuicksortAlgorithm::QuicksortAlgorithm(data::SwapQueue& swapQueue)
            : m_swapQueue(swapQueue)
            , m_cancelled(false)
            , m_swapCount(0)
            , m_estimatedSwaps(0)
        {
        }

        // ============================================================================
        // Sorting
        // ============================================================================

        void QuicksortAlgorithm::execute(std::vector<double>& values) {
            // Reset state
            m_cancelled.store(false);
            m_swapCount.store(0);

            // Estimate swaps for sound scaling
            m_estimatedSwaps = estimateSwaps(static_cast<uint32_t>(values.size()));
            m_swapQueue.setTotalSwaps(m_estimatedSwaps);

            // Execute quicksort
            if (!values.empty()) {
                quicksort(values, 0, static_cast<int>(values.size()) - 1);
            }

            // Mark as complete
            m_swapQueue.markComplete();
        }

        void QuicksortAlgorithm::requestCancel() {
            m_cancelled.store(true);
        }

        bool QuicksortAlgorithm::isCancelled() const {
            return m_cancelled.load();
        }

        uint32_t QuicksortAlgorithm::getSwapCount() const {
            return m_swapCount.load();
        }

        // ============================================================================
        // Internal Sorting Methods
        // ============================================================================

        void QuicksortAlgorithm::quicksort(std::vector<double>& values, int low, int high) {
            // Check for cancellation
            if (m_cancelled.load()) {
                return;
            }

            if (low < high) {
                // Partition and get pivot index
                int pivotIndex = partition(values, low, high);

                // Recursively sort sub-arrays
                quicksort(values, low, pivotIndex - 1);
                quicksort(values, pivotIndex + 1, high);
            }
        }

        int QuicksortAlgorithm::partition(std::vector<double>& values, int low, int high) {
            // Use last element as pivot
            double pivot = values[high];
            int i = low - 1;

            for (int j = low; j < high; ++j) {
                // Check for cancellation periodically
                if (m_cancelled.load()) {
                    return low;
                }

                if (values[j] <= pivot) {
                    ++i;
                    if (i != j) {
                        recordSwap(values, static_cast<uint32_t>(i), static_cast<uint32_t>(j));
                    }
                }
            }

            // Place pivot in correct position
            if (i + 1 != high) {
                recordSwap(values, static_cast<uint32_t>(i + 1), static_cast<uint32_t>(high));
            }

            return i + 1;
        }

        void QuicksortAlgorithm::recordSwap(std::vector<double>& values, uint32_t indexA, uint32_t indexB) {
            // Increment swap count
            uint32_t currentSwap = m_swapCount.fetch_add(1) + 1;

            // Perform the actual swap on the data
            std::swap(values[indexA], values[indexB]);

            // Record the swap for visualization
            data::SwapOperation swap(indexA, indexB, currentSwap, m_estimatedSwaps);
            m_swapQueue.push(swap);
        }

        uint32_t QuicksortAlgorithm::estimateSwaps(uint32_t n) const {
            // Average case: O(n log n) comparisons, roughly n/2 swaps per partition level
            // Estimate: n * log2(n) / 4 swaps on average
            if (n <= 1) return 0;

            double logN = std::log2(static_cast<double>(n));
            uint32_t estimate = static_cast<uint32_t>(n * logN / 4.0);

            // Ensure at least n swaps estimated (for sound scaling purposes)
            return std::max(estimate, n);
        }

    } // namespace algorithm
} // namespace quicksort