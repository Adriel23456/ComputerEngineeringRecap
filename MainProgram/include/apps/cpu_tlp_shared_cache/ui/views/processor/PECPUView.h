// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/views/processor/PECPUView.h
// ============================================================================

#pragma once

/**
 * @file PECPUView.h
 * @brief Generic CPU pipeline view for any Processing Element.
 *
 * Displays pipeline diagram with instruction labels for each stage.
 * Single implementation works for all PEs (PE0-PE3).
 *
 * @note Follows:
 *   - DRY: Single implementation for all PEs
 *   - OCP: Easy to extend with new PE indices
 *   - SRP: Only handles CPU pipeline display
 */

#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/ZoomPanImage.h"
#include <array>
#include <memory>
#include <string>

namespace sf { class Texture; }

/**
 * @class PECPUView
 * @brief Generic CPU pipeline visualization view.
 *
 * Displays the CPU pipeline diagram with 5 instruction label boxes
 * corresponding to pipeline stages. Works for any PE index.
 */
class PECPUView : public ICpuTLPView {
public:
    /**
     * @brief Constructs view for specific PE.
     * @param peIndex Processing element index (0-3).
     */
    explicit PECPUView(int peIndex);

    /**
     * @brief Renders the pipeline view.
     */
    void render() override;

    // ========================================================================
    // Label Management
    // ========================================================================

    /**
     * @brief Sets single stage label.
     * @param index Stage index (0-4).
     * @param text Label text.
     */
    void setLabel(size_t index, const std::string& text);

    /**
     * @brief Sets all stage labels.
     * @param labels Array of 5 label strings.
     */
    void setLabels(const std::array<std::string, 5>& labels);

    /**
     * @brief Gets current labels.
     * @return Const reference to label array.
     */
    const std::array<std::string, 5>& getLabels() const { return m_labels; }

    /**
     * @brief Gets PE index.
     * @return PE index (0-3).
     */
    int getPEIndex() const { return m_peIndex; }

private:
    /**
     * @brief Ensures texture is loaded.
     */
    void ensureLoaded();

    /**
     * @brief Renders pipeline stage overlay boxes.
     */
    void renderOverlay(const ImVec2& origin, float scale, ImDrawList* dl);

    int m_peIndex;                                         ///< PE index (0-3)
    std::shared_ptr<sf::Texture> m_tex;                    ///< Pipeline texture
    ZoomPanImage m_viewer;                                 ///< Image viewer
    std::array<std::string, 5> m_labels{ "NOP", "NOP", "NOP", "NOP", "NOP" };
};