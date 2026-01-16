// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/views/RAMView.cpp
// ============================================================================

/**
 * @file RAMView.cpp
 * @brief Implementation of RAMView.
 */

#include "apps/cpu_tlp_shared_cache/ui/views/RAMView.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.h"
#include "tinyfiledialogs.h"
#include <imgui.h>
#include <iostream>

void RAMView::render() {
    if (!m_sharedMemoryComponent) {
        ImGui::Text("RAM Component not initialized");
        return;
    }

    ImVec2 available = ImGui::GetContentRegionAvail();
    const float SPACING = 8.0f;
    const float BUTTON_HEIGHT = 46.0f;

    float tableHeight = available.y - (SPACING + BUTTON_HEIGHT);
    if (tableHeight < 0.0f) {
        tableHeight = 0.0f;
    }

    // Get memory reference
    auto& sharedRAM = m_sharedMemoryComponent->getMemory();

    // Synchronize data from SharedMemory to table widget
    for (uint16_t byteAddr = 0; byteAddr < SharedMemory::MEM_SIZE_BYTES; byteAddr += 8) {
        uint64_t value = sharedRAM.get(byteAddr);
        int rowIndex = byteAddr / 8;
        m_table.setDataByIndex(rowIndex, value);
    }

    // Render table
    ImGuiWindowFlags childFlags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##RAMTableArea", ImVec2(available.x, tableHeight), false, childFlags);
    {
        m_table.render("##RAM_TABLE");
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // Control buttons
    const float GAP = 10.0f;
    const float buttonWidth = (available.x - GAP) * 0.5f;
    const ImVec2 buttonSize(buttonWidth, BUTTON_HEIGHT);

    // Reset button (red)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.16f, 0.16f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.78f, 0.22f, 0.22f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.58f, 0.12f, 0.12f, 1.0f));

    if (ImGui::Button("Reset", buttonSize)) {
        std::cout << "[RAM] RESET pressed\n";
        sharedRAM.reset();
    }

    ImGui::PopStyleColor(3);
    ImGui::SameLine(0.0f, GAP);

    // Load button (green)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.55f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.68f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.09f, 0.45f, 0.16f, 1.0f));

    if (ImGui::Button("Load", buttonSize)) {
        const char* filters[] = { "*.bin" };
        const char* filePath = tinyfd_openFileDialog(
            "Select Binary File",
            "",
            1, filters, "Binary files (*.bin)",
            0
        );

        if (filePath) {
            std::cout << "[RAM] Loading binary: " << filePath << "\n";
            if (!sharedRAM.loadFromFile(filePath, 0, 8)) {
                std::cerr << "[RAM] Error loading binary file.\n";
            }
            else {
                std::cout << "[RAM] Binary loaded successfully.\n";
            }
        }
        else {
            std::cout << "[RAM] Load cancelled.\n";
        }
    }

    ImGui::PopStyleColor(3);
}