// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/views/CompilerView.h
// ============================================================================

#pragma once

/**
 * @file CompilerView.h
 * @brief Assembly code editor and compiler view.
 *
 * Provides a text editor for assembly source code with
 * load, save, and compile functionality.
 *
 * @note Follows:
 *   - SRP: Editor UI separate from compilation logic
 *   - DIP: Uses callback for external compilation notification
 */

#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include <string>
#include <functional>

 /**
  * @class CompilerView
  * @brief Assembly source editor with compilation support.
  *
  * Features:
  * - Multi-line text editing with tab support
  * - Load/save assembly files
  * - Compile to binary format
  * - Status message display
  */
class CompilerView : public ICpuTLPView {
public:
    CompilerView();

    // ========================================================================
    // ICpuTLPView Interface
    // ========================================================================

    void handleEvent(sf::Event&) override {}
    void update(float deltaTime) override;
    void render() override;

    // ========================================================================
    // Source Code Access
    // ========================================================================

    /**
     * @brief Sets the editor source text.
     * @param text New source code.
     */
    void setText(const std::string& text);

    /**
     * @brief Gets the current source text.
     * @return Source code string.
     */
    std::string getText() const;

    /**
     * @brief Sets callback invoked after successful compilation.
     * @param callback Function receiving compiled source.
     */
    void setCompileCallback(std::function<void(const std::string&)> callback);

private:
    /**
     * @brief Compiles source code and writes binary output.
     * @param sourceCode Assembly source to compile.
     * @return true if compilation succeeded.
     */
    bool compileToFile(const std::string& sourceCode);

    /**
     * @brief ImGui callback for dynamic string buffer resizing.
     * @param data Callback data from ImGui.
     * @return 0 on success.
     */
    static int TextEditCallback(struct ImGuiInputTextCallbackData* data);

    std::string m_source;                                    ///< Source code buffer
    std::function<void(const std::string&)> m_compileCallback; ///< Post-compile callback

    bool m_isCompiling = false;       ///< Compilation in progress flag
    std::string m_compileMessage;     ///< Status message to display
    float m_messageTimer = 0.0f;      ///< Message display countdown
};