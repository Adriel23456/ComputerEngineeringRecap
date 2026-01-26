// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/views/CompilerView.cpp
// ============================================================================

/**
 * @file CompilerView.cpp
 * @brief Implementation of CompilerView.
 */

#include "apps/cpu_tlp_shared_cache/ui/views/CompilerView.h"
#include "apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.h"
#include "tinyfiledialogs.h"
#include "imgui.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

namespace {

    /** @brief Default example source code. */
    const char* DEFAULT_SOURCE =
        "# Simple example of summing from 1 to 10000:\n"
        "MOVI REG1, #1\n"
        "MOVI REG2, #0\n"
        ".Loop:\n"
        "\tADD REG2, REG2, REG1\n"
        "\tADDI REG1, REG1, #1\n"
        "\tCMPI REG1, #10000\n"
        "\tBLT .Loop\n"
        "SWI\n";

    /**
     * @brief Removes UTF-8 BOM from string if present.
     * @param source Input string.
     * @return String with BOM removed.
     */
    std::string removeBOM(const std::string& source) {
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

    /**
     * @brief Cleans source code by removing non-ASCII characters.
     *
     * Normalizes line endings and filters out control characters.
     *
     * @param source Raw source code.
     * @return Cleaned ASCII-only source.
     */
    std::string cleanSource(const std::string& source) {
        std::string result;
        result.reserve(source.size());

        for (size_t i = 0; i < source.length(); ++i) {
            unsigned char c = static_cast<unsigned char>(source[i]);

            // Normalize line endings
            if (c == '\r') {
                result += '\n';
                // Skip \n in \r\n sequence
                if (i + 1 < source.length() && source[i + 1] == '\n') {
                    ++i;
                }
            }
            else if (c == '\n') {
                result += '\n';
            }
            // Keep standard printable ASCII (space through tilde)
            else if (c >= 32 && c <= 126) {
                result += static_cast<char>(c);
            }
            // Ignore all other characters (control chars, non-ASCII)
        }

        return result;
    }

} // anonymous namespace

// ============================================================================
// Construction
// ============================================================================

CompilerView::CompilerView() {
    m_source.reserve(16 * 1024);
    m_source.assign(DEFAULT_SOURCE);

    if (m_source.capacity() - m_source.size() < 1024) {
        m_source.reserve(m_source.size() + 4096);
    }
}

// ============================================================================
// ImGui Callback
// ============================================================================

int CompilerView::TextEditCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        auto* str = static_cast<std::string*>(data->UserData);
        str->resize(static_cast<size_t>(data->BufTextLen));
        data->Buf = const_cast<char*>(str->c_str());
    }
    return 0;
}

// ============================================================================
// Compilation
// ============================================================================

bool CompilerView::compileToFile(const std::string& sourceCode) {
    try {
        // Clean the source code
        std::string cleanedSource = removeBOM(sourceCode);
        cleanedSource = cleanSource(cleanedSource);

        // Write temporary source file
        std::string tmpPath = std::string(RESOURCES_PATH) + "Assets/CPU_TLP/TempSource.txt";
        MKDIR((std::string(RESOURCES_PATH) + "Assets/CPU_TLP").c_str());

        std::ofstream tmpFile(tmpPath);
        if (!tmpFile.is_open()) {
            std::cerr << "[CompilerView] Could not create temp file: " << tmpPath << std::endl;
            return false;
        }
        tmpFile << cleanedSource;
        tmpFile.close();

        // Assemble the source
        Assembler assembler;
        std::vector<uint64_t> instructions = assembler.assembleFile(tmpPath);

        // Write binary output
        std::string outPath = std::string(RESOURCES_PATH) + "Assets/CPU_TLP/InstMem.bin";
        std::ofstream out(outPath, std::ios::binary | std::ios::trunc);
        if (!out.is_open()) {
            std::cerr << "[CompilerView] Could not open file for writing: " << outPath << std::endl;
            return false;
        }

        // Write instructions in little-endian format
        for (uint64_t inst : instructions) {
            uint8_t bytes[8];
            for (int i = 0; i < 8; ++i) {
                bytes[i] = static_cast<uint8_t>((inst >> (i * 8)) & 0xFF);
            }
            out.write(reinterpret_cast<const char*>(bytes), 8);
        }
        out.close();

        std::cout << "[CompilerView] Compiled " << instructions.size()
            << " instructions to " << outPath << std::endl;

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "[CompilerView] Compilation failed: " << e.what() << std::endl;
        m_compileMessage = std::string("Compilation Error: ") + e.what();
        return false;
    }
}

void CompilerView::setCompileCallback(std::function<void(const std::string&)> callback) {
    m_compileCallback = std::move(callback);
}

// ============================================================================
// Update
// ============================================================================

void CompilerView::update(float deltaTime) {
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

void CompilerView::render() {
    const float SPACING = 10.0f;
    const float BUTTON_HEIGHT = 46.0f;

    ImVec2 available = ImGui::GetContentRegionAvail();
    float editorHeight = available.y - BUTTON_HEIGHT - SPACING;

    if (!m_compileMessage.empty()) {
        editorHeight -= 30.0f;
    }
    if (editorHeight < 0.0f) {
        editorHeight = 0.0f;
    }

    // Ensure buffer capacity
    if (m_source.capacity() - m_source.size() < 1024) {
        m_source.reserve(m_source.size() + 4096);
    }

    // Text editor
    ImGuiInputTextFlags flags =
        ImGuiInputTextFlags_AllowTabInput |
        ImGuiInputTextFlags_CallbackResize;

    ImGui::InputTextMultiline(
        "##plain_text_editor",
        const_cast<char*>(m_source.c_str()),
        m_source.capacity() + 1,
        ImVec2(available.x, editorHeight),
        flags,
        &CompilerView::TextEditCallback,
        static_cast<void*>(&m_source)
    );

    // Status message
    if (!m_compileMessage.empty()) {
        ImGui::Spacing();

        bool isSuccess = (m_compileMessage.find("Success") != std::string::npos);
        ImVec4 color = isSuccess
            ? ImVec4(0.2f, 0.9f, 0.2f, 1.0f)
            : ImVec4(0.9f, 0.2f, 0.2f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("%s", m_compileMessage.c_str());
        ImGui::PopStyleColor();
    }

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // Buttons
    const float GAP = 10.0f;
    const float buttonWidth = (available.x - 2.0f * GAP) / 3.0f;

    // Load button
    if (ImGui::Button("Load", ImVec2(buttonWidth, BUTTON_HEIGHT))) {
        const char* filters[] = { "*.txt" };
        const char* filePath = tinyfd_openFileDialog(
            "Select Assembly Source File",
            "",
            1, filters, "Text files (*.txt)",
            0
        );

        if (filePath) {
            std::ifstream file(filePath);
            if (file.is_open()) {
                std::ostringstream buffer;
                buffer << file.rdbuf();
                m_source = buffer.str();
                std::cout << "[CompilerView] Loaded file: " << filePath << std::endl;
                m_compileMessage = std::string("Loaded: ") + filePath;
            }
            else {
                m_compileMessage = "Error: Could not open file.";
            }
        }
        else {
            std::cout << "[CompilerView] Load cancelled.\n";
        }
        m_messageTimer = 3.0f;
    }

    ImGui::SameLine(0.0f, GAP);

    // Save button
    if (ImGui::Button("Save", ImVec2(buttonWidth, BUTTON_HEIGHT))) {
        const char* filePath = tinyfd_saveFileDialog(
            "Save Assembly File",
            "program.txt",
            0, nullptr, nullptr
        );

        if (filePath) {
            std::ofstream file(filePath);
            if (file.is_open()) {
                file << m_source;
                file.close();
                m_compileMessage = std::string("Saved: ") + filePath;
            }
            else {
                m_compileMessage = "Error: Could not save file.";
            }
        }
        m_messageTimer = 3.0f;
    }

    ImGui::SameLine(0.0f, GAP);

    // Compile button (green styling)
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
            m_isCompiling = true;
            bool success = compileToFile(m_source);

            if (success) {
                m_compileMessage = "Compilation Success! Instructions loaded.";
                if (m_compileCallback) {
                    m_compileCallback(m_source);
                }
            }
            else {
                if (m_compileMessage.empty()) {
                    m_compileMessage = "Compilation Error.";
                }
            }

            m_messageTimer = 3.0f;
            m_isCompiling = false;
        }
    }

    ImGui::PopStyleColor(3);
}

// ============================================================================
// Source Access
// ============================================================================

void CompilerView::setText(const std::string& text) {
    m_source = text;
    if (m_source.capacity() - m_source.size() < 1024) {
        m_source.reserve(m_source.size() + 4096);
    }
}

std::string CompilerView::getText() const {
    return m_source;
}