// ============================================================================
// File: include/apps/quicksort_visualizer/data/SortElement.h
// ============================================================================

#pragma once

/**
 * @file SortElement.h
 * @brief Data structure for a single sortable element.
 *
 * Contains the logical data for one element in the quicksort visualization.
 * This is pure data with no UI dependencies.
 *
 * @note Design Principles:
 *   - SRP: Only holds element data
 *   - OCP: Can be extended with additional states without modification
 */

#include <cstdint>

namespace quicksort {
    namespace data {

        /**
         * @enum ElementState
         * @brief Visual state of an element during sorting.
         *
         * Used to determine the color of the element during visualization.
         */
        enum class ElementState : uint8_t {
            Normal,      ///< Default state - chalk/white color
            Comparing,   ///< Currently being compared - yellow
            Swapping,    ///< Currently being swapped - red
            Pivot,       ///< Current pivot element - green
            Sorted,      ///< Element is in final sorted position - blue
            Active       ///< Element is in the active partition range - orange
        };

        /**
         * @struct SortElement
         * @brief Represents a single element to be sorted.
         *
         * Contains:
         * - The logical value (determines height)
         * - Original index (for tracking during sort)
         * - Current visual state (for coloring)
         */
        struct SortElement {
            uint32_t value;          ///< The value of this element (1 to maxValue)
            uint32_t originalIndex;  ///< Original position before sorting
            ElementState state;      ///< Current visual state

            /**
             * @brief Default constructor.
             */
            SortElement()
                : value(0)
                , originalIndex(0)
                , state(ElementState::Normal)
            {
            }

            /**
             * @brief Constructs an element with a value and index.
             * @param val The element's value.
             * @param index The element's original index.
             */
            SortElement(uint32_t val, uint32_t index)
                : value(val)
                , originalIndex(index)
                , state(ElementState::Normal)
            {
            }

            /**
             * @brief Resets the element state to Normal.
             */
            void resetState() {
                state = ElementState::Normal;
            }
        };

    } // namespace data
} // namespace quicksort