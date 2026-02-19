// ============================================================================
// File: src/apps/cpu_tomasulo/ui/views/TomasuloRAMView.cpp
// ============================================================================

/**
 * @file TomasuloRAMView.cpp
 * @brief Implementation of TomasuloRAMView.
 *
 * Renders the RAM table in the upper region and the control row
 * (RESET RAM + Load .bin) in a fixed-height child at the bottom,
 * with a status bar in between.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloRAMView.h"
#include "tinyfiledialogs.h"

#include <imgui.h>
#include <iostream>

 // ============================================================================
 // Callbacks
 // ============================================================================

void TomasuloRAMView::setResetCallback(std::function<void()> callback) {
    m_resetCallback = std::move(callback);
}

void TomasuloRAMView::setLoadBinaryCallback(
    std::function<void(const std::string&)> callback) {
    m_loadBinaryCallback = std::move(callback);
}

// ============================================================================
// Status Display
// ============================================================================

void TomasuloRAMView::setStatusMessage(const std::string& message,
    float durationSeconds) {
    m_statusMessage = message;
    m_statusTimer = durationSeconds;
}

// ============================================================================
// Update
// ============================================================================

void TomasuloRAMView::update(float deltaTime) {
    if (m_statusTimer > 0.0f) {
        m_statusTimer -= deltaTime;
        if (m_statusTimer <= 0.0f)
            m_statusMessage.clear();
    }
}

// ============================================================================
// Main Render
// ============================================================================

void TomasuloRAMView::render() {
    ImVec2            available = ImGui::GetContentRegionAvail();
    const ImGuiStyle& style = ImGui::GetStyle();

    const float SPACING = 10.0f;
    const float BUTTON_HEIGHT = 46.0f;

    const float STATUS_HEIGHT =
        ImGui::GetTextLineHeight() + style.WindowPadding.y * 2.0f + 2.0f;

    // Fixed-height child for the button row (prevents bottom clipping)
    const float BUTTON_AREA_HEIGHT =
        BUTTON_HEIGHT + style.WindowPadding.y * 1.5f;

    // Extra breathing room so the last element is never flush with the edge
    const float BOTTOM_PAD = style.WindowPadding.y + 6.0f;

    // Table takes whatever height remains after the footer
    const float bottomAreaHeight =
        STATUS_HEIGHT + SPACING +
        BUTTON_AREA_HEIGHT + BOTTOM_PAD + SPACING;

    float tableHeight = available.y - bottomAreaHeight;
    if (tableHeight < 0.0f) tableHeight = 0.0f;

    // ── RAM Table ───────────────────────────────────────────────
    ImGuiWindowFlags childFlags =
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##TomasuloRAMTableArea",
        ImVec2(available.x, tableHeight), false, childFlags);
    {
        m_table.render("##TOMASULO_RAM_TABLE");
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // ── Status Bar ──────────────────────────────────────────────
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
    ImGui::BeginChild("##tomasulo_ram_status", ImVec2(0.0f, STATUS_HEIGHT), true);
    {
        if (!m_statusMessage.empty()) {
            bool   isOk = (m_statusMessage.find("Success") != std::string::npos);
            ImVec4 col = isOk
                ? ImVec4(0.2f, 0.9f, 0.2f, 1.0f)
                : ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, col);
            ImGui::TextUnformatted(m_statusMessage.c_str());
            ImGui::PopStyleColor();
        }
        else {
            ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "Ready.");
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // ── Control Buttons (in their own child to prevent clipping) ─
    ImGui::BeginChild("##tomasulo_ram_controls",
        ImVec2(0.0f, BUTTON_AREA_HEIGHT), false,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        renderControlButtons(ImGui::GetContentRegionAvail().x);
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1.0f, BOTTOM_PAD));
}

// ============================================================================
// Control Buttons
// ============================================================================

void TomasuloRAMView::renderControlButtons(float availableWidth) {
    const float GAP = 10.0f;
    const float BUTTON_HEIGHT = 46.0f;
    const float buttonWidth = (availableWidth - GAP) * 0.5f;
    const ImVec2 buttonSize(buttonWidth, BUTTON_HEIGHT);

    // ── RESET RAM (red) ─────────────────────────────────────────
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.16f, 0.16f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.78f, 0.22f, 0.22f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.58f, 0.12f, 0.12f, 1.0f));

    if (ImGui::Button("RESET RAM", buttonSize)) {
        std::cout << "[TomasuloRAM] RESET RAM pressed\n";
        if (m_resetCallback) {
            m_resetCallback();
            setStatusMessage("Reset requested...", 3.0f);
        }
        m_table.resetPage();
    }

    ImGui::PopStyleColor(3);
    ImGui::SameLine(0.0f, GAP);

    // ── Load .bin (green) ───────────────────────────────────────
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.55f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.68f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.09f, 0.45f, 0.16f, 1.0f));

    if (ImGui::Button("Load .bin", buttonSize)) {
        const char* filters[] = { "*.bin" };
        const char* filePath = tinyfd_openFileDialog(
            "Select Binary Data File", "", 1, filters, "Binary files (*.bin)", 0);

        if (filePath) {
            std::cout << "[TomasuloRAM] Load .bin: " << filePath << "\n";
            if (m_loadBinaryCallback) {
                m_loadBinaryCallback(std::string(filePath));
                setStatusMessage("Loading binary...", 30.0f);
            }
            else {
                setStatusMessage("Error: Load callback not connected.", 4.0f);
            }
        }
        else {
            std::cout << "[TomasuloRAM] Load cancelled.\n";
        }
    }

    ImGui::PopStyleColor(3);
    ImGui::Dummy(ImVec2(1.0f, 4.0f));
}