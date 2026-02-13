#pragma once

/**
 * @file TomasuloCompilerView.h
 * @brief Assembly code editor view for Tomasulo CPU.
 *
 * Provides text editor with:
 * - Load: opens file dialog to load a .txt file into the editor
 * - Save: opens file dialog to save editor contents as .txt
 * - Compile: invokes registered callback to assemble and load into RAM
 *
 * @note
 *   - SRP: Only handles source editing UI and file I/O dialogs
 *   - OCP: Compile logic injected via callback, not hardcoded
 *   - DIP: Depends on callback abstraction, not simulation internals
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

    // ── Source text access ───────────────────────────────────────

    void        setText(const std::string& text);
    std::string getText() const;

    // ── Callbacks ───────────────────────────────────────────────

    /**
     * @brief Sets the callback invoked when Compile is pressed.
     * @param callback  Receives the source text.
     */
    void setCompileCallback(std::function<void(const std::string&)> callback);

    // ── Status display ──────────────────────────────────────────

    /**
     * @brief Sets the status-bar message and auto-clear timer.
     * @param message         Text to display.
     * @param durationSeconds How long the message stays visible.
     */
    void setCompileMessage(const std::string& message,
        float durationSeconds = 5.0f);

private:
    static int TextEditCallback(ImGuiInputTextCallbackData* data);

    // ── Source cleaning helpers ──────────────────────────────────
    static std::string removeBOM(const std::string& source);
    static std::string cleanSource(const std::string& source);

    std::string m_source;
    std::function<void(const std::string&)> m_compileCallback;

    bool        m_isCompiling = false;
    std::string m_compileMessage;
    float       m_messageTimer = 0.0f;
};