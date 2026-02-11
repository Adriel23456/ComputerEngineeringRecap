/**
 * @file CpuTomasuloState.cpp
 * @brief Implementation of CpuTomasuloState.
 */

#include "apps/cpu_tomasulo/CpuTomasuloState.h"
#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloMainView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloCompilerView.h"

#include <imgui.h>
#include <iostream>
#include <algorithm>

 // ============================================================================
 // Placeholder View
 // ============================================================================

namespace {

    class PlaceholderView : public ITomasuloView {
    public:
        explicit PlaceholderView(const char* name) : m_name(name) {}

        void render() override {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                "%s — (not yet implemented)", m_name);
        }

    private:
        const char* m_name;
    };

} // anonymous namespace

// ============================================================================
// Construction / Destruction
// ============================================================================

CpuTomasuloState::CpuTomasuloState(StateManager* sm, sf::RenderWindow* win)
    : State(sm, win)
{
    buildAllViews();
}

CpuTomasuloState::~CpuTomasuloState() = default;

// ============================================================================
// View Management
// ============================================================================

void CpuTomasuloState::buildAllViews() {
    m_views[panelIndex(Panel::MainView)] = std::make_unique<TomasuloMainView>();
    m_views[panelIndex(Panel::Compiler)] = std::make_unique<TomasuloCompilerView>();
    m_views[panelIndex(Panel::RAM)] = std::make_unique<PlaceholderView>("RAM");
    m_views[panelIndex(Panel::ICache)] = std::make_unique<PlaceholderView>("I-Cache");
    m_views[panelIndex(Panel::DCache)] = std::make_unique<PlaceholderView>("D-Cache");
    m_views[panelIndex(Panel::Registers)] = std::make_unique<PlaceholderView>("Registers");
    m_views[panelIndex(Panel::ROB)] = std::make_unique<PlaceholderView>("ROB");
    m_views[panelIndex(Panel::DataAnalysis)] = std::make_unique<PlaceholderView>("Data Analysis");
}

ITomasuloView* CpuTomasuloState::getView(Panel panel) {
    return m_views[panelIndex(panel)].get();
}

// ============================================================================
// State Interface
// ============================================================================

void CpuTomasuloState::handleEvent(sf::Event& event) {
    if (auto* view = getView(m_selectedPanel)) {
        view->handleEvent(event);
    }
}

void CpuTomasuloState::update(float deltaTime) {
    for (auto& view : m_views) {
        if (view) view->update(deltaTime);
    }
}

void CpuTomasuloState::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    if (ImGui::Begin("##CpuTomasulo", nullptr, flags)) {
        ImVec2 available = ImGui::GetContentRegionAvail();

        const float sidebarWidth = available.x * 0.18f;
        const float separator = 4.0f;
        const float contentWidth = available.x - sidebarWidth - separator;
        const float bottomBarHeight = 60.0f;
        const float topHeight = available.y - bottomBarHeight - separator;

        renderSidebar(sidebarWidth, topHeight);
        ImGui::SameLine(0.0f, separator);
        renderContentPanel(contentWidth, topHeight);

        ImGui::Dummy(ImVec2(1.0f, separator * 0.5f));
        renderBottomBar(available.x, bottomBarHeight);
    }
    ImGui::End();
}

void CpuTomasuloState::renderBackground() {
    m_window->clear(sf::Color(20, 20, 25));
}

// ============================================================================
// Sidebar
// ============================================================================

void CpuTomasuloState::renderSidebar(float width, float height) {
    ImGui::BeginChild("##TomasuloSidebar", ImVec2(width, height), true,
        ImGuiWindowFlags_AlwaysVerticalScrollbar);

    constexpr float TOP_PADDING = 14.0f;
    constexpr float BUTTON_HEIGHT = 56.0f;
    constexpr float BUTTON_SPACING = 10.0f;
    float buttonWidth = ImGui::GetContentRegionAvail().x;

    ImGui::Dummy(ImVec2(1, TOP_PADDING));

    struct NavItem { const char* label; Panel panel; };
    const NavItem items[] = {
        { "Main View",     Panel::MainView     },
        { "Compiler",      Panel::Compiler      },
        { "RAM",           Panel::RAM           },
        { "I-Cache",       Panel::ICache        },
        { "D-Cache",       Panel::DCache        },
        { "Registers",     Panel::Registers     },
        { "ROB",           Panel::ROB           },
        { "Data Analysis", Panel::DataAnalysis  },
    };

    for (const auto& item : items) {
        if (createSidebarButton(item.label, m_selectedPanel == item.panel,
            buttonWidth, BUTTON_HEIGHT)) {
            m_selectedPanel = item.panel;
        }
        ImGui::Dummy(ImVec2(1, BUTTON_SPACING));
    }

    ImGui::EndChild();
}

// ============================================================================
// Content Panel
// ============================================================================

void CpuTomasuloState::renderContentPanel(float width, float height) {
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##TomasuloContent", ImVec2(width, height), true, flags);

    if (auto* view = getView(m_selectedPanel)) {
        view->render();
    }

    ImGui::EndChild();
}

// ============================================================================
// Bottom Control Bar
// ============================================================================

void CpuTomasuloState::renderBottomBar(float width, float height) {
    ImGui::BeginChild("##TomasuloBottomBar", ImVec2(width, height), true);

    const float GAP = 10.0f;
    const float TEXT_PADDING = 30.0f;
    const float BUTTON_HEIGHT = 40.0f;
    ImVec2 available = ImGui::GetContentRegionAvail();

    float offsetY = (available.y - BUTTON_HEIGHT) * 0.5f;
    if (offsetY > 0.0f) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);

    // --- RESET (red) ---
    float wReset = ImGui::CalcTextSize("RESET").x + TEXT_PADDING;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.82f, 0.22f, 0.22f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.60f, 0.12f, 0.12f, 1.0f));
    if (ImGui::Button("RESET", ImVec2(wReset, BUTTON_HEIGHT))) {
        m_cycleCount = 0;
        std::cout << "[Tomasulo] RESET pressed\n";
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0.0f, GAP);

    // --- STEP ---
    float wStep = ImGui::CalcTextSize("STEP").x + TEXT_PADDING;
    if (ImGui::Button("STEP", ImVec2(wStep, BUTTON_HEIGHT))) {
        std::cout << "[Tomasulo] STEP pressed\n";
    }

    ImGui::SameLine(0.0f, GAP);

    // --- StepUntil ---
    ImGui::BeginGroup();
    {
        if (m_untilSteps < 1) m_untilSteps = 1;

        float inputWidth = available.x * 0.08f;
        ImGui::PushItemWidth(inputWidth);
        int tmp = m_untilSteps;
        ImGui::InputScalar("##tomasulo_until_steps", ImGuiDataType_S32, &tmp,
            nullptr, nullptr, nullptr, ImGuiInputTextFlags_CharsDecimal);
        if (tmp < 1) tmp = 1;
        m_untilSteps = tmp;
        ImGui::PopItemWidth();

        ImGui::SameLine();

        float wStepUntil = ImGui::CalcTextSize("StepUntil").x + TEXT_PADDING;
        if (ImGui::Button("StepUntil", ImVec2(wStepUntil, BUTTON_HEIGHT))) {
            std::cout << "[Tomasulo] StepUntil " << m_untilSteps << " pressed\n";
        }
    }
    ImGui::EndGroup();

    ImGui::SameLine(0.0f, GAP);

    // --- InfiniteStep (green) ---
    float wInfinite = ImGui::CalcTextSize("InfiniteStep").x + TEXT_PADDING;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.55f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.68f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.09f, 0.45f, 0.16f, 1.0f));
    if (ImGui::Button("InfiniteStep", ImVec2(wInfinite, BUTTON_HEIGHT))) {
        std::cout << "[Tomasulo] InfiniteStep pressed\n";
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0.0f, GAP);

    // --- STOP ---
    float wStop = ImGui::CalcTextSize("STOP").x + TEXT_PADDING;
    if (ImGui::Button("STOP", ImVec2(wStop, BUTTON_HEIGHT))) {
        std::cout << "[Tomasulo] STOP pressed\n";
    }

    ImGui::SameLine(0.0f, GAP * 2.0f);

    // --- Cycles counter ---
    char cyclesText[64];
    std::snprintf(cyclesText, sizeof(cyclesText), "Cycles: %llu",
        static_cast<unsigned long long>(m_cycleCount));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.3f, 1.0f), "%s", cyclesText);

    ImGui::EndChild();
}

// ============================================================================
// Sidebar Button Helper
// ============================================================================

bool CpuTomasuloState::createSidebarButton(const char* label, bool selected,
    float width, float height) {
    ImVec2 textSize = ImGui::CalcTextSize(label);
    float paddingX = ImGui::GetStyle().FramePadding.x * 2.f;
    float avail = width - paddingX;
    float scale = avail > 0.f ? (avail / std::max(1.f, textSize.x)) : 1.f;
    scale = std::clamp(scale, 0.85f, 1.35f);

    ImVec4 normalColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImVec4 normalHov = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
    ImVec4 normalAct = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

    ImVec4 activeColor(normalColor.x * 0.7f + 0.3f, normalColor.y * 0.7f + 0.3f,
        normalColor.z * 0.7f + 0.3f, 1.0f);
    ImVec4 activeHov(normalHov.x * 0.7f + 0.3f, normalHov.y * 0.7f + 0.3f,
        normalHov.z * 0.7f + 0.3f, 1.0f);
    ImVec4 activeAct(normalAct.x * 0.7f + 0.3f, normalAct.y * 0.7f + 0.3f,
        normalAct.z * 0.7f + 0.3f, 1.0f);

    if (selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, activeHov);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeAct);
    }

    ImGui::SetWindowFontScale(scale);
    bool clicked = ImGui::Button(label, ImVec2(width, height));
    ImGui::SetWindowFontScale(1.0f);

    if (selected) {
        ImGui::PopStyleColor(3);
    }

    return clicked;
}

// ============================================================================
// Typed View Accessor
// ============================================================================

TomasuloMainView* CpuTomasuloState::getMainView() {
    return dynamic_cast<TomasuloMainView*>(getView(Panel::MainView));
}