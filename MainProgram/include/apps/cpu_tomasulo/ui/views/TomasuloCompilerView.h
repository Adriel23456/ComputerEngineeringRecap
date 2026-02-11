#pragma once

/**
 * @file TomasuloCompilerView.h
 * @brief Assembly code editor view for Tomasulo CPU.
 *
 * Provides text editor with Load, Save, Compile buttons.
 * No compilation logic wired — UI shell only.
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

    void setText(const std::string& text);
    std::string getText() const;

    void setCompileCallback(std::function<void(const std::string&)> callback);

private:
    static int TextEditCallback(ImGuiInputTextCallbackData* data);

    std::string m_source;
    std::function<void(const std::string&)> m_compileCallback;

    bool m_isCompiling = false;
    std::string m_compileMessage;
    float m_messageTimer = 0.0f;
};