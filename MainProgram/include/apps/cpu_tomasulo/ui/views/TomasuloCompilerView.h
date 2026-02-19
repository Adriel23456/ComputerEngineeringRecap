// ============================================================================
// File: include/apps/cpu_tomasulo/ui/views/TomasuloCompilerView.h
// ============================================================================

#pragma once

/**
 * @file TomasuloCompilerView.h
 * @brief Assembly source editor view for the Tomasulo CPU.
 *
 * Provides a resizable text editor with three action buttons:
 *   - Load:    Opens a file dialog and reads a .txt file into the editor.
 *   - Save:    Opens a file dialog and writes the editor contents to .txt.
 *   - Compile: Cleans the source text and invokes the registered callback.
 *
 * The compile callback is injected by CpuTomasuloState::wireCallbacks()
 * so this view has no dependency on the simulation internals.
 *
 * @note
 *   - SRP: Only handles source editing UI and file I/O dialogs.
 *   - OCP: Compile logic is injected via callback, not hardcoded.
 *   - DIP: Depends on a callback abstraction, not simulation internals.
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include <string>
#include <functional>

struct ImGuiInputTextCallbackData;

class TomasuloCompilerView : public ITomasuloView {
public:
    TomasuloCompilerView();

    void handleEvent(sf::Event&) override {}
    void update(float deltaTime) override;
    void render() override;

    // ── Source Text Access ───────────────────────────────────────
    void        setText(const std::string& text);
    std::string getText() const;

    // ── Compile Callback ─────────────────────────────────────────

    /**
     * @brief Registers the callback invoked when Compile is pressed.
     * @param callback  Receives the cleaned source text.
     */
    void setCompileCallback(std::function<void(const std::string&)> callback);

    // ── Status Display ───────────────────────────────────────────

    /**
     * @brief Sets the status-bar message and its auto-clear duration.
     * @param message          Text to display in the status bar.
     * @param durationSeconds  How long the message stays visible.
     */
    void setCompileMessage(const std::string& message,
        float durationSeconds = 5.0f);

private:
    /** @brief ImGui resize callback for the dynamic string buffer. */
    static int TextEditCallback(ImGuiInputTextCallbackData* data);

    // ── Source Cleaning ──────────────────────────────────────────
    /** @brief Strips a leading UTF-8 or UTF-16 BOM from the string. */
    static std::string removeBOM(const std::string& source);

    /** @brief Normalises line endings and removes non-printable bytes. */
    static std::string cleanSource(const std::string& source);

    // ── State ────────────────────────────────────────────────────
    std::string m_source;
    std::function<void(const std::string&)> m_compileCallback;

    bool        m_isCompiling = false;
    std::string m_compileMessage;
    float       m_messageTimer = 0.0f;
};