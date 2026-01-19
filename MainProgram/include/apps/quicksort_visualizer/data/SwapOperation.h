// ============================================================================
// File: include/apps/quicksort_visualizer/data/SwapOperation.h
// ============================================================================

#pragma once

/**
 * @file SwapOperation.h
 * @brief Data structure representing a swap operation during sorting.
 *
 * Contains indices of elements to be swapped. This is a pure data structure
 * with no dependencies on UI or threading.
 *
 * @note Design Principles:
 *   - SRP: Only holds swap data
 *   - Immutable after construction for thread safety
 */

#include <cstdint>

namespace quicksort {
    namespace data {

        /**
         * @struct SwapOperation
         * @brief Represents a single swap operation between two elements.
         */
        struct SwapOperation {
            uint32_t indexA;    ///< First element index
            uint32_t indexB;    ///< Second element index
            uint32_t swapNumber; ///< Sequential swap number (for sound pitch)
            uint32_t totalSwaps; ///< Total expected swaps (estimated for sound scaling)

            /**
             * @brief Default constructor.
             */
            SwapOperation()
                : indexA(0)
                , indexB(0)
                , swapNumber(0)
                , totalSwaps(1)
            {
            }

            /**
             * @brief Constructs a swap operation.
             * @param a First element index.
             * @param b Second element index.
             * @param num Sequential swap number.
             * @param total Total expected swaps.
             */
            SwapOperation(uint32_t a, uint32_t b, uint32_t num, uint32_t total)
                : indexA(a)
                , indexB(b)
                , swapNumber(num)
                , totalSwaps(total)
            {
            }

            /**
             * @brief Checks if this is a valid swap (different indices).
             * @return true if indices are different.
             */
            bool isValid() const {
                return indexA != indexB;
            }

            /**
             * @brief Gets the progress ratio (0.0 to 1.0) for sound pitch scaling.
             * @return Progress ratio.
             */
            float getProgressRatio() const {
                if (totalSwaps == 0) return 0.0f;
                return static_cast<float>(swapNumber) / static_cast<float>(totalSwaps);
            }
        };

    } // namespace data
} // namespace quicksort