// ============================================================================
// File: include/apps/quicksort_visualizer/data/ElementCollection.h
// ============================================================================

#pragma once

/**
 * @file ElementCollection.h
 * @brief Manages the collection of sortable elements.
 *
 * Handles element generation, storage, and access. This is pure logic
 * with no UI dependencies - the rendering is handled separately.
 *
 * @note Design Principles:
 *   - SRP: Only manages element data and generation
 *   - OCP: New generation strategies can be added
 *   - DIP: No dependencies on UI frameworks
 */

#include "apps/quicksort_visualizer/data/SortElement.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include <vector>
#include <cstdint>
#include <random>

namespace quicksort {
    namespace data {

        /**
         * @class ElementCollection
         * @brief Manages a collection of sortable elements.
         *
         * Provides:
         * - Random value generation
         * - Element access (read-only for main thread)
         * - Element modification (for sorting thread)
         * - Grid size calculation based on element count
         */
        class ElementCollection {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs an empty element collection.
             */
            ElementCollection();

            /**
             * @brief Destructor.
             */
            ~ElementCollection() = default;

            // ========================================================================
            // Element Generation
            // ========================================================================

            /**
             * @brief Generates random elements.
             * @param count Number of elements to generate.
             *
             * Values are generated in range [1, count] and shuffled.
             * This ensures each value is unique.
             */
            void generateRandom(uint32_t count);

            /**
             * @brief Generates elements in sorted order (for testing).
             * @param count Number of elements to generate.
             */
            void generateSorted(uint32_t count);

            /**
             * @brief Generates elements in reverse sorted order (worst case).
             * @param count Number of elements to generate.
             */
            void generateReverseSorted(uint32_t count);

            /**
             * @brief Clears all elements.
             */
            void clear();

            // ========================================================================
            // Element Access (Thread-Safe Read)
            // ========================================================================

            /**
             * @brief Gets the number of elements.
             * @return Element count.
             */
            uint32_t getCount() const;

            /**
             * @brief Checks if collection is empty.
             * @return true if no elements.
             */
            bool isEmpty() const;

            /**
             * @brief Gets a const reference to an element.
             * @param index Element index.
             * @return Const reference to element.
             */
            const SortElement& getElement(uint32_t index) const;

            /**
             * @brief Gets all elements (const).
             * @return Const reference to element vector.
             */
            const std::vector<SortElement>& getElements() const;

            /**
             * @brief Gets the maximum value in the collection.
             * @return Maximum element value.
             */
            uint32_t getMaxValue() const;

            // ========================================================================
            // Element Modification (For Sorting Thread)
            // ========================================================================

            /**
             * @brief Gets a mutable reference to an element.
             * @param index Element index.
             * @return Mutable reference to element.
             */
            SortElement& getElementMutable(uint32_t index);

            /**
             * @brief Swaps two elements by index.
             * @param indexA First element index.
             * @param indexB Second element index.
             */
            void swapElements(uint32_t indexA, uint32_t indexB);

            /**
             * @brief Sets the state of an element.
             * @param index Element index.
             * @param state New state.
             */
            void setElementState(uint32_t index, ElementState state);

            /**
             * @brief Resets all element states to Normal.
             */
            void resetAllStates();

            // ========================================================================
            // Grid Size Calculation
            // ========================================================================

            /**
             * @brief Calculates the required grid width for current elements.
             *
             * Formula: LEFT_PADDING + (count * elementWidth) + ((count - 1) * gap) + RIGHT_PADDING
             *
             * @param elementWidth Width of each element bar.
             * @param elementGap Gap between elements.
             * @return Required grid width.
             */
            float calculateGridWidth(float elementWidth, float elementGap) const;

            /**
             * @brief Gets the recommended element width based on count.
             * @return Recommended width per element.
             */
            float getRecommendedElementWidth() const;

            /**
             * @brief Gets the recommended gap between elements based on count.
             * @return Recommended gap in grid units.
             */
            float getRecommendedElementGap() const;

        private:
            // ========================================================================
            // Data Members
            // ========================================================================

            std::vector<SortElement> m_elements;  ///< The element storage
            uint32_t m_maxValue;                   ///< Maximum value in collection
            std::mt19937 m_rng;                    ///< Random number generator

            // ========================================================================
            // Constants
            // ========================================================================

            static constexpr float BASE_ELEMENT_WIDTH = 20.0f;      ///< Width for small counts
            static constexpr float MIN_ELEMENT_WIDTH = 1.0f;        ///< Minimum width per element
            static constexpr float BASE_ELEMENT_GAP = 2.0f;         ///< Gap for small counts
            static constexpr float MIN_ELEMENT_GAP = 0.5f;          ///< Minimum gap
        };

    } // namespace data
} // namespace quicksort