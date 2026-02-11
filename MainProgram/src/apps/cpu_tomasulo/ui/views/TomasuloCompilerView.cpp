/**
 * @file TomasuloCompilerView.cpp
 * @brief Implementation of TomasuloCompilerView.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloCompilerView.h"
#include "imgui.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace {

    const char* DEFAULT_SOURCE =
        "# Tomasulo CPU assembly example:\n"
        "# (instruction set not yet defined)\n"
        "NOP\n";

} // anonymous namespace

// ============================================================================
// Construction
// ============================================================================

TomasuloCompilerView::TomasuloCompilerView() {
    m_source.reserve(16 * 1024);
    m_source.assign(DEFAULT_SOURCE);

    if (m_source.capacity() - m_source.size() < 1024) {
        m_source.reserve(m_source.size() + 4096);
    }
}

// ============================================================================
// ImGui Callback
// ============================================================================

int TomasuloCompilerView::TextEditCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        auto* str = static_cast<std::string*>(data->UserData);
        str->resize(static_cast<size_t>(data->BufTextLen));
        data->Buf = const_cast<char*>(str->c_str());
    }
    return 0;
}

// ============================================================================
// Callback Setter
// ============================================================================

void TomasuloCompilerView::setCompileCallback(std::function<void(const std::string&)> callback) {
    m_compileCallback = std::move(callback);
}

// ============================================================================
// Update
// ============================================================================

void TomasuloCompilerView::update(float deltaTime) {
    if (m_messageTimer > 0.0f) {
        m_messageTimer -= deltaTime;
        if (m_messageTimer <= 0.0f) {
            m_compileMessage.clear();
        }
    }
}

// ============================================================================
// Rendering
// ============================================================================

void TomasuloCompilerView::render() {
    const float SPACING = 10.0f;
    const float BUTTON_HEIGHT = 46.0f;
    const float GAP = 10.0f;

    ImGuiStyle& style = ImGui::GetStyle();

    // Message box height: 1 line of text + vertical padding (and a tiny safety margin)
    const float lineH = ImGui::GetTextLineHeight(); // or GetTextLineHeightWithSpacing()
    const float messageHeight = lineH + style.WindowPadding.y * 2.0f + 2.0f;

    // --- TOP: Editor takes "everything except footer" -------------------------
    // Footer = spacing + message + spacing + buttons + item spacing/padding safety
    const float footerHeight =
        SPACING +
        messageHeight +
        SPACING +
        BUTTON_HEIGHT +
        style.ItemSpacing.y * 2.0f +     // accounts for Dummy() + layout spacing
        style.WindowPadding.y +          // extra padding that often bites
        4.0f;                            // tiny safety margin

    // Create an editor child that fills remaining height minus footer
    ImGui::BeginChild("##tomasulo_editor_region", ImVec2(0.0f, -footerHeight), false);
    {
        // Ensure buffer capacity
        if (m_source.capacity() - m_source.size() < 1024) {
            m_source.reserve(m_source.size() + 4096);
        }

        ImGuiInputTextFlags flags =
            ImGuiInputTextFlags_AllowTabInput |
            ImGuiInputTextFlags_CallbackResize;

        // Fill the child region
        ImGui::InputTextMultiline(
            "##tomasulo_text_editor",
            const_cast<char*>(m_source.c_str()),
            m_source.capacity() + 1,
            ImVec2(-FLT_MIN, -FLT_MIN),
            flags,
            &TomasuloCompilerView::TextEditCallback,
            static_cast<void*>(&m_source)
        );
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // --- MIDDLE: Always-visible message box -----------------------------------
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
    ImGui::BeginChild("##tomasulo_msg_box", ImVec2(0.0f, messageHeight), true);
    {
        if (!m_compileMessage.empty()) {
            const bool isSuccess = (m_compileMessage.find("Success") != std::string::npos);
            const ImVec4 color = isSuccess
                ? ImVec4(0.2f, 0.9f, 0.2f, 1.0f)
                : ImVec4(0.9f, 0.2f, 0.2f, 1.0f);

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(m_compileMessage.c_str());
            ImGui::PopStyleColor();
        }
        else {
            ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "Ready.");
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // --- BOTTOM: Buttons -------------------------------------------------------
    const float availX = ImGui::GetContentRegionAvail().x;
    const float buttonWidth = (availX - 2.0f * GAP) / 3.0f;

    if (ImGui::Button("Load", ImVec2(buttonWidth, BUTTON_HEIGHT))) {
        std::cout << "[TomasuloCompiler] Load pressed (not yet wired)\n";
        m_compileMessage = "Load: not yet implemented.";
        m_messageTimer = 3.0f;
    }

    ImGui::SameLine(0.0f, GAP);

    if (ImGui::Button("Save", ImVec2(buttonWidth, BUTTON_HEIGHT))) {
        std::cout << "[TomasuloCompiler] Save pressed (not yet wired)\n";
        m_compileMessage = "Save: not yet implemented.";
        m_messageTimer = 3.0f;
    }

    ImGui::SameLine(0.0f, GAP);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.55f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.68f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.09f, 0.45f, 0.16f, 1.0f));

    if (ImGui::Button("Compile", ImVec2(buttonWidth, BUTTON_HEIGHT))) {
        std::cout << "[TomasuloCompiler] Compile pressed (not yet wired)\n";
        m_compileMessage = "Compile: not yet implemented.";
        m_messageTimer = 3.0f;
    }

    ImGui::PopStyleColor(3);
}

// ============================================================================
// Source Access
// ============================================================================

void TomasuloCompilerView::setText(const std::string& text) {
    m_source = text;
    if (m_source.capacity() - m_source.size() < 1024) {
        m_source.reserve(m_source.size() + 4096);
    }
}

std::string TomasuloCompilerView::getText() const {
    return m_source;
}