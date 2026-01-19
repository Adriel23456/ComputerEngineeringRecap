// ============================================================================
// File: src/apps/quicksort_visualizer/data/ElementCollection.cpp
// ============================================================================

/**
 * @file ElementCollection.cpp
 * @brief Implementation of the ElementCollection class.
 */

#include "apps/quicksort_visualizer/data/ElementCollection.h"
#include <algorithm>
#include <numeric>
#include <chrono>

namespace quicksort {
    namespace data {

        // ============================================================================
        // Construction
        // ============================================================================

        ElementCollection::ElementCollection()
            : m_elements()
            , m_maxValue(0)
            , m_rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
        {
        }

        // ============================================================================
        // Element Generation
        // ============================================================================

        void ElementCollection::generateRandom(uint32_t count) {
            // Clamp count to valid range
            count = std::clamp(count,
                config::GridConfig::MIN_ELEMENT_COUNT,
                config::GridConfig::MAX_ELEMENT_COUNT);

            // Clear and resize
            m_elements.clear();
            m_elements.reserve(count);

            // Generate sequential values
            for (uint32_t i = 0; i < count; ++i) {
                m_elements.emplace_back(i + 1, i);  // Values from 1 to count
            }

            // Shuffle the elements
            std::shuffle(m_elements.begin(), m_elements.end(), m_rng);

            // Update original indices after shuffle
            for (uint32_t i = 0; i < count; ++i) {
                m_elements[i].originalIndex = i;
            }

            m_maxValue = count;
        }

        void ElementCollection::generateSorted(uint32_t count) {
            // Clamp count to valid range
            count = std::clamp(count,
                config::GridConfig::MIN_ELEMENT_COUNT,
                config::GridConfig::MAX_ELEMENT_COUNT);

            // Clear and resize
            m_elements.clear();
            m_elements.reserve(count);

            // Generate sorted values
            for (uint32_t i = 0; i < count; ++i) {
                m_elements.emplace_back(i + 1, i);
            }

            m_maxValue = count;
        }

        void ElementCollection::generateReverseSorted(uint32_t count) {
            // Clamp count to valid range
            count = std::clamp(count,
                config::GridConfig::MIN_ELEMENT_COUNT,
                config::GridConfig::MAX_ELEMENT_COUNT);

            // Clear and resize
            m_elements.clear();
            m_elements.reserve(count);

            // Generate reverse sorted values
            for (uint32_t i = 0; i < count; ++i) {
                m_elements.emplace_back(count - i, i);
            }

            m_maxValue = count;
        }

        void ElementCollection::clear() {
            m_elements.clear();
            m_maxValue = 0;
        }

        // ============================================================================
        // Element Access
        // ============================================================================

        uint32_t ElementCollection::getCount() const {
            return static_cast<uint32_t>(m_elements.size());
        }

        bool ElementCollection::isEmpty() const {
            return m_elements.empty();
        }

        const SortElement& ElementCollection::getElement(uint32_t index) const {
            return m_elements[index];
        }

        const std::vector<SortElement>& ElementCollection::getElements() const {
            return m_elements;
        }

        uint32_t ElementCollection::getMaxValue() const {
            return m_maxValue;
        }

        // ============================================================================
        // Element Modification
        // ============================================================================

        SortElement& ElementCollection::getElementMutable(uint32_t index) {
            return m_elements[index];
        }

        void ElementCollection::swapElements(uint32_t indexA, uint32_t indexB) {
            if (indexA < m_elements.size() && indexB < m_elements.size()) {
                std::swap(m_elements[indexA], m_elements[indexB]);
            }
        }

        void ElementCollection::setElementState(uint32_t index, ElementState state) {
            if (index < m_elements.size()) {
                m_elements[index].state = state;
            }
        }

        void ElementCollection::resetAllStates() {
            for (auto& element : m_elements) {
                element.resetState();
            }
        }

        // ============================================================================
        // Grid Size Calculation
        // ============================================================================

        float ElementCollection::calculateGridWidth(float elementWidth, float elementGap) const {
            if (m_elements.empty()) {
                return config::GridConfig::DEFAULT_GRID_WIDTH;
            }

            uint32_t count = static_cast<uint32_t>(m_elements.size());

            // Calculate exact width needed:
            // LEFT_PADDING + (count * elementWidth) + ((count - 1) * gap) + RIGHT_PADDING
            //
            // For N elements:
            // - N rectangles of width elementWidth
            // - N-1 gaps between them
            // - Padding on both sides

            float elementsWidth = static_cast<float>(count) * elementWidth;
            float gapsWidth = static_cast<float>(count > 1 ? count - 1 : 0) * elementGap;
            float totalPadding = config::GridConfig::GRID_LEFT_PADDING + config::GridConfig::GRID_RIGHT_PADDING;

            float totalWidth = totalPadding + elementsWidth + gapsWidth;

            // Ensure minimum width
            return std::max(totalWidth, config::GridConfig::MIN_GRID_WIDTH);
        }

        float ElementCollection::getRecommendedElementWidth() const {
            if (m_elements.empty()) {
                return BASE_ELEMENT_WIDTH;
            }

            uint32_t count = static_cast<uint32_t>(m_elements.size());

            // Scale element width based on count
            // Wider bars for fewer elements, narrower for more
            if (count <= 50) {
                return 20.0f;
            }
            else if (count <= 100) {
                return 15.0f;
            }
            else if (count <= 200) {
                return 10.0f;
            }
            else if (count <= 500) {
                return 5.0f;
            }
            else if (count <= 1000) {
                return 3.0f;
            }
            else if (count <= 2000) {
                return 2.0f;
            }
            else if (count <= 5000) {
                return 1.5f;
            }
            else {
                return MIN_ELEMENT_WIDTH;  // 1px for very large counts
            }
        }

        float ElementCollection::getRecommendedElementGap() const {
            if (m_elements.empty()) {
                return BASE_ELEMENT_GAP;
            }

            uint32_t count = static_cast<uint32_t>(m_elements.size());

            // Smaller gap for more elements to fit better
            if (count <= 100) {
                return 2.0f;
            }
            else if (count <= 500) {
                return 1.5f;
            }
            else if (count <= 1000) {
                return 1.0f;
            }
            else if (count <= 2000) {
                return 0.75f;
            }
            else {
                return MIN_ELEMENT_GAP;  // 0.5px
            }
        }

    } // namespace data
} // namespace quicksort