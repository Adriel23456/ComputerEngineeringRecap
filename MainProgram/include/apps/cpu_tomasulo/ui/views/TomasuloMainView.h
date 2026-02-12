#pragma once
/**
 * @file TomasuloMainView.h
 * @brief Main architecture diagram view for the Tomasulo CPU.
 *
 * After SOLID refactoring this class is a thin **Mediator** that
 * wires together four focused components:
 *   - Camera2D            (pan / zoom / inertia / clamping)
 *   - ShaderBackground    (animated fragment-shader backdrop)
 *   - BorderRenderer      (decorative 3-D frame)
 *   - SlotOverlayRenderer (label boxes on the diagram)
 *
 * New features:
 *   - Pan is clamped so ≥20 % of the image is always visible.
 *   - A translucent RESET button sits in the bottom-right corner.
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
    // ── Slot enum (unchanged) ─────────────────────────────────
    enum Slot {
        ISSUE = 0,
        SB0, SB1,
        LB0, LB1, LB2,
        RS_INTALU0, RS_INTALU1,
        RS_FPALU0,
        RS_INTMUL0,
        RS_FPMUL0,
        RS_BRANCH0,
        COMMIT_CDB_A, COMMIT_CDB_B,
        SLOT_COUNT
    };

    using SlotDef = SlotOverlayRenderer::SlotDef;

    // ── Construction ──────────────────────────────────────────
    TomasuloMainView();

    // ── ITomasuloView ─────────────────────────────────────────
    void render() override;

    // ── Label access ──────────────────────────────────────────
    void setLabel(Slot slot, const std::string& text);
    void setLabels(const std::array<std::string, SLOT_COUNT>& labels);
    const std::array<std::string, SLOT_COUNT>& getLabels() const { return m_labels; }
    void clearLabels();

private:
    void ensureLoaded();

    /// Returns the border color for a given slot index.
    static ImU32 slotBorderColor(int index);

    /// Draws the "RESET" button in the bottom-right of the canvas.
    void drawResetButton(const ImVec2& canvasP0, const ImVec2& canvasSize);

    // ── Data ──────────────────────────────────────────────────
    std::shared_ptr<sf::Texture>                m_tex;
    std::array<std::string, SLOT_COUNT>         m_labels;
    static const std::array<SlotDef, SLOT_COUNT> s_slotDefs;

    // ── Components (SRP) ──────────────────────────────────────
    Camera2D              m_camera;
    ShaderBackground      m_background;
    SlotOverlayRenderer   m_overlay;
};