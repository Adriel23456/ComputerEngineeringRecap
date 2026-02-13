/**
 * @file TomasuloCompilerView.cpp
 * @brief Implementation of TomasuloCompilerView.
 *
 * - Load:    Opens file dialog → reads .txt into editor
 * - Save:    Opens file dialog → writes editor text to .txt
 * - Compile: Cleans source, invokes async callback
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloCompilerView.h"
#include "tinyfiledialogs.h"
#include "imgui.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace {

    const char* DEFAULT_SOURCE =
        "# Tomasulo CPU assembly example\n"
        "#\n"
        "# Arithmetic:\n"
        "#   ADD  REG1, REG2, REG3\n"
        "#   ADDI REG1, REG2, #10\n"
        "#   MUL  REG4, REG1, REG5\n"
        "#\n"
        "# Memory:\n"
        "#   LDR  REG1, [REG0, #0]\n"
        "#   STR  REG2, [REG0, #8]\n"
        "#\n"
        "# Branches:\n"
        "#   loop: ADD REG1, REG1, REG2\n"
        "#         BNE loop\n"
        "#\n"
        "NOP\n";

} // anonymous namespace

// ============================================================================
// Source Cleaning Helpers
// ============================================================================

std::string TomasuloCompilerView::removeBOM(const std::string& source) {
    if (source.size() >= 3 &&
        static_cast<unsigned char>(source[0]) == 0xEF &&
        static_cast<unsigned char>(source[1]) == 0xBB &&
        static_cast<unsigned char>(source[2]) == 0xBF) {
        return source.substr(3);
    }
    if (!source.empty() &&
        (static_cast<unsigned char>(source[0]) == 0xEF ||
            static_cast<unsigned char>(source[0]) == 0xFF)) {
        return source.substr(1);
    }
    return source;
}

std::string TomasuloCompilerView::cleanSource(const std::string& source) {
    std::string result;
    result.reserve(source.size());

    for (size_t i = 0; i < source.length(); ++i) {
        unsigned char c = static_cast<unsigned char>(source[i]);

        if (c == '\r') {
            result += '\n';
            if (i + 1 < source.length() && source[i + 1] == '\n') ++i;
        }
        else if (c == '\n') {
            result += '\n';
        }
        else if (c == '\t') {
            result += '\t';
        }
        else if (c >= 32 && c <= 126) {
            result += static_cast<char>(c);
        }
        // ignore all other bytes
    }
    return result;
}

// ============================================================================
// Construction
// ============================================================================

TomasuloCompilerView::TomasuloCompilerView() {
    m_source.reserve(16 * 1024);
    m_source.assign(DEFAULT_SOURCE);

    if (m_source.capacity() - m_source.size() < 1024)
        m_source.reserve(m_source.size() + 4096);
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
// Callbacks
// ============================================================================

void TomasuloCompilerView::setCompileCallback(
    std::function<void(const std::string&)> callback) {
    m_compileCallback = std::move(callback);
}

// ============================================================================
// Status Display
// ============================================================================

void TomasuloCompilerView::setCompileMessage(const std::string& message,
    float durationSeconds) {
    m_compileMessage = message;
    m_messageTimer = durationSeconds;

    // If we receive a result message, the compile is done
    if (message.find("Compiling...") == std::string::npos) {
        m_isCompiling = false;
    }
}

// ============================================================================
// Update
// ============================================================================

void TomasuloCompilerView::update(float deltaTime) {
    if (m_messageTimer > 0.0f) {
        m_messageTimer -= deltaTime;
        if (m_messageTimer <= 0.0f)
            m_compileMessage.clear();
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

    const float lineH = ImGui::GetTextLineHeight();
    const float messageHeight = lineH + style.WindowPadding.y * 2.0f + 2.0f;

    const float footerHeight =
        SPACING + messageHeight + SPACING + BUTTON_HEIGHT +
        style.ItemSpacing.y * 2.0f + style.WindowPadding.y + 4.0f;

    // ── Editor ──────────────────────────────────────────────────
    ImGui::BeginChild("##tomasulo_editor_region",
        ImVec2(0.0f, -footerHeight), false);
    {
        if (m_source.capacity() - m_source.size() < 1024)
            m_source.reserve(m_source.size() + 4096);

        ImGuiInputTextFlags flags =
            ImGuiInputTextFlags_AllowTabInput |
            ImGuiInputTextFlags_CallbackResize;

        ImGui::InputTextMultiline(
            "##tomasulo_text_editor",
            const_cast<char*>(m_source.c_str()),
            m_source.capacity() + 1,
            ImVec2(-FLT_MIN, -FLT_MIN),
            flags,
            &TomasuloCompilerView::TextEditCallback,
            static_cast<void*>(&m_source));
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // ── Message box ─────────────────────────────────────────────
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
    ImGui::BeginChild("##tomasulo_msg_box",
        ImVec2(0.0f, messageHeight), true);
    {
        if (!m_compileMessage.empty()) {
            bool isSuccess =
                (m_compileMessage.find("Success") != std::string::npos ||
                    m_compileMessage.find("Loaded") != std::string::npos ||
                    m_compileMessage.find("Saved") != std::string::npos);
            ImVec4 color = isSuccess
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

    // ── Buttons ─────────────────────────────────────────────────
    const float availX = ImGui::GetContentRegionAvail().x;
    const float buttonWidth = (availX - 2.0f * GAP) / 3.0f;

    // ── Load ────────────────────────────────────────────────────
    if (ImGui::Button("Load", ImVec2(buttonWidth, BUTTON_HEIGHT))) {
        const char* filters[] = { "*.txt" };
        const char* filePath = tinyfd_openFileDialog(
            "Select Assembly Source File",
            "",
            1, filters,
            "Text files (*.txt)",
            0);

        if (filePath) {
            std::ifstream file(filePath);
            if (file.is_open()) {
                std::ostringstream buffer;
                buffer << file.rdbuf();

                std::string raw = buffer.str();
                raw = removeBOM(raw);
                raw = cleanSource(raw);

                m_source = raw;
                if (m_source.capacity() - m_source.size() < 1024)
                    m_source.reserve(m_source.size() + 4096);

                std::cout << "[TomasuloCompiler] Loaded: " << filePath << "\n";
                setCompileMessage(std::string("Loaded: ") + filePath, 4.0f);
            }
            else {
                setCompileMessage("Error: Could not open file.", 4.0f);
            }
        }
        else {
            std::cout << "[TomasuloCompiler] Load cancelled.\n";
        }
    }

    ImGui::SameLine(0.0f, GAP);

    // ── Save ────────────────────────────────────────────────────
    if (ImGui::Button("Save", ImVec2(buttonWidth, BUTTON_HEIGHT))) {
        const char* filters[] = { "*.txt" };
        const char* filePath = tinyfd_saveFileDialog(
            "Save Assembly File",
            "program.txt",
            1, filters,
            "Text files (*.txt)");

        if (filePath) {
            std::ofstream file(filePath);
            if (file.is_open()) {
                file << m_source;
                file.close();
                std::cout << "[TomasuloCompiler] Saved: " << filePath << "\n";
                setCompileMessage(std::string("Saved: ") + filePath, 4.0f);
            }
            else {
                setCompileMessage("Error: Could not save file.", 4.0f);
            }
        }
    }

    ImGui::SameLine(0.0f, GAP);

    // ── Compile (green) ─────────────────────────────────────────
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.55f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.68f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.09f, 0.45f, 0.16f, 1.0f));

    if (m_isCompiling) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        ImGui::Button("Compiling...", ImVec2(buttonWidth, BUTTON_HEIGHT));
        ImGui::PopStyleVar();
    }
    else {
        if (ImGui::Button("Compile", ImVec2(buttonWidth, BUTTON_HEIGHT))) {
            std::cout << "[TomasuloCompiler] Compile pressed\n";

            if (m_compileCallback) {
                // Clean source before sending to assembler
                std::string cleaned = removeBOM(m_source);
                cleaned = cleanSource(cleaned);

                m_compileCallback(cleaned);
                setCompileMessage("Compiling...", 30.0f);
                m_isCompiling = true;
            }
            else {
                setCompileMessage("Compile: callback not connected.", 4.0f);
            }
        }
    }

    ImGui::PopStyleColor(3);
}

// ============================================================================
// Source Access
// ============================================================================

void TomasuloCompilerView::setText(const std::string& text) {
    m_source = text;
    if (m_source.capacity() - m_source.size() < 1024)
        m_source.reserve(m_source.size() + 4096);
}

std::string TomasuloCompilerView::getText() const {
    return m_source;
}