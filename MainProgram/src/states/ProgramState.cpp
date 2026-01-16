// ============================================================================
// File: src/states/ProgramState.cpp
// ============================================================================

/**
 * @file ProgramState.cpp
 * @brief Implementation of ProgramState placeholder.
 */

#include "states/ProgramState.h"
#include "imgui.h"

 // ============================================================================
 // Construction
 // ============================================================================

ProgramState::ProgramState(StateManager* stateManager, sf::RenderWindow* window, std::string title)
    : State(stateManager, window)
    , m_title(std::move(title))
{
}

// ============================================================================
// Event Handling
// ============================================================================

void ProgramState::handleEvent(sf::Event& event) {
    (void)event; // Placeholder has no input handling
}

// ============================================================================
// Update
// ============================================================================

void ProgramState::update(float /*deltaTime*/) {
    // Placeholder has no update logic
}

// ============================================================================
// Rendering
// ============================================================================

void ProgramState::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;

    if (ImGui::Begin("##ProgramStateWindow", nullptr, flags)) {
        ImGui::Text("Active Program: %s", m_title.c_str());
        ImGui::Separator();
        ImGui::TextWrapped(
            "This is a stub view for the program. There is no return to the main menu from here. "
            "Press ESC to open Settings (overlay) to adjust graphics/audio or exit."
        );
    }
    ImGui::End();
}

void ProgramState::renderBackground() {
    m_window->clear(sf::Color(30, 30, 30));
}