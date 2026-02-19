// ============================================================================
// File: include/apps/cpu_tomasulo/ui/views/TomasuloMainView.h
// ============================================================================

#pragma once

/**
 * @file TomasuloMainView.h
 * @brief Main architecture diagram view for the Tomasulo CPU.
 *
 * Acts as a thin Mediator that wires together four focused components:
 *   - Camera2D            — pan / zoom / inertia / viewport clamping
 *   - ShaderBackground    — animated GLSL wormhole backdrop
 *   - BorderRenderer      — decorative 3-D frame around the diagram
 *   - SlotOverlayRenderer — labeled boxes drawn on top of the diagram
 *
 * The diagram image is loaded once via TextureCache and rendered into
 * an ImDrawList canvas. Labels are pushed each frame by
 * CpuTomasuloState::syncMainView() while the simulation mutex is held.
 *
 * Interaction:
 *   - Scroll        : zoom anchored to cursor
 *   - Left/right drag : pan
 *   - Double-right-click / RESET button : reset camera
 *
 * @note
 *   - SRP: Each rendering concern is isolated in its own component.
 *   - OCP: New overlays are added by extending SlotDef / Slot enum.
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/components/Camera2D.h"
#include "apps/cpu_tomasulo/ui/components/ShaderBackground.h"
#include "apps/cpu_tomasulo/ui/components/SlotOverlayRenderer.h"

#include "imgui.h"
#include <array>
#include <memory>
#include <string>

namespace sf { class Texture; }

class TomasuloMainView : public ITomasuloView {
public:
    // ── Slot Identifiers ─────────────────────────────────────────
    enum Slot {
        ISSUE = 0,
        SB0, SB1,
        LB0, LB1, LB2,
        RS_INTALU0, RS_INTALU1,
        RS_FPALU0,
        RS_INTMUL0,
        RS_FPMUL0,
        RS_BRANCH0,
        COMMIT
    };

    static constexpr int SLOT_COUNT = 13;
    using SlotDef = SlotOverlayRenderer::SlotDef;

    // ── Construction ─────────────────────────────────────────────
    TomasuloMainView();

    // ── ITomasuloView ────────────────────────────────────────────
    void render() override;

    // ── Label Access ─────────────────────────────────────────────
    void setLabel(Slot slot, const std::string& text);
    void setLabels(const std::array<std::string, SLOT_COUNT>& labels);
    const std::array<std::string, SLOT_COUNT>& getLabels() const { return m_labels; }
    void clearLabels();

private:
    /** @brief Loads the diagram texture on first use via TextureCache. */
    void ensureLoaded();

    /** @brief Returns the ImU32 border color for a given slot index. */
    static ImU32 slotBorderColor(int index);

    /**
     * @brief Draws the floating RESET camera button in the bottom-right corner.
     *
     * Uses the foreground draw list so it renders above all canvas content.
     * Input is handled via a manual hit-test to avoid conflicts with the
     * InvisibleButton that captures pan/zoom input.
     */
    void drawResetButton(const ImVec2& canvasP0, const ImVec2& canvasSize);

    // ── Data ─────────────────────────────────────────────────────
    std::shared_ptr<sf::Texture>                 m_tex;
    std::array<std::string, SLOT_COUNT>          m_labels;
    static const std::array<SlotDef, SLOT_COUNT> s_slotDefs;  ///< World-space slot layout.

    // ── Components ───────────────────────────────────────────────
    Camera2D             m_camera;
    ShaderBackground     m_background;
    SlotOverlayRenderer  m_overlay;
};