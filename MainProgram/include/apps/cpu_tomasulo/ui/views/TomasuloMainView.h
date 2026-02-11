#pragma once

/**
 * @file TomasuloMainView.h
 * @brief Main architecture diagram view for the Tomasulo CPU.
 *
 * Displays the CPU architecture diagram with overlay instruction
 * label boxes for each pipeline slot:
 *   - Issue:   1 slot  (Control_Unit)
 *   - Execute: 11 slots (SB0, SB1, LB0-2, RS_IntALU0-1,
 *                         RS_FPALU0, RS_IntMUL0, RS_FPMUL0, RS_Branch0)
 *   - Commit:  2 slots  (CDB_A, CDB_B)
 *
 * @note Follows PECPUView pattern from TLP CPU.
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/ZoomPanImage.h"
#include <array>
#include <memory>
#include <string>

namespace sf { class Texture; }

class TomasuloMainView : public ITomasuloView {
public:
    // ========================================================================
    // Slot Enumeration
    // ========================================================================

    enum Slot {
        // Issue (1)
        ISSUE = 0,

        // Execute (11)
        SB0, SB1,
        LB0, LB1, LB2,
        RS_INTALU0, RS_INTALU1,
        RS_FPALU0,
        RS_INTMUL0,
        RS_FPMUL0,
        RS_BRANCH0,

        // Commit (2)
        COMMIT_CDB_A, COMMIT_CDB_B,

        SLOT_COUNT  // = 14
    };

    TomasuloMainView();

    void render() override;

    // ========================================================================
    // Label Management
    // ========================================================================

    /** @brief Sets a single slot label. */
    void setLabel(Slot slot, const std::string& text);

    /** @brief Sets all 14 labels at once. */
    void setLabels(const std::array<std::string, SLOT_COUNT>& labels);

    /** @brief Gets all labels. */
    const std::array<std::string, SLOT_COUNT>& getLabels() const { return m_labels; }

    /** @brief Clears all labels to "---". */
    void clearLabels();

private:
    // ========================================================================
    // Internal Types
    // ========================================================================

    /** @brief Defines an overlay box position in image-pixel space. */
    struct SlotDef {
        float x, y;          ///< Top-left corner (image pixels)
        float w, h;          ///< Width, height (image pixels)
        const char* name;    ///< Debug / tooltip name
    };

    // ========================================================================
    // Methods
    // ========================================================================

    void ensureLoaded();
    void renderOverlay(const ImVec2& origin, float scale, ImDrawList* dl);

    // ========================================================================
    // Data
    // ========================================================================

    std::shared_ptr<sf::Texture>                m_tex;
    ZoomPanImage                                m_viewer;
    std::array<std::string, SLOT_COUNT>         m_labels;

    /** @brief Pixel-space definitions for each slot box. */
    static const std::array<SlotDef, SLOT_COUNT> s_slotDefs;
};