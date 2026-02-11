#pragma once

/**
 * @file CpuTomasuloState.h
 * @brief Main state coordinator for Tomasulo CPU simulation UI.
 */

#include "core/fsm/State.h"
#include <memory>
#include <array>
#include <string>
#include <cstdint>

namespace sf { class RenderWindow; }
class StateManager;
class ITomasuloView;
class TomasuloMainView;

class CpuTomasuloState : public State {
public:
    CpuTomasuloState(StateManager* sm, sf::RenderWindow* win);
    ~CpuTomasuloState() override;

    CpuTomasuloState(const CpuTomasuloState&) = delete;
    CpuTomasuloState& operator=(const CpuTomasuloState&) = delete;

    void handleEvent(sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;

    TomasuloMainView* getMainView();

private:
    enum class Panel {
        MainView = 0,
        Compiler,
        RAM,
        ICache,
        DCache,
        Registers,
        ROB,
        DataAnalysis,
        COUNT
    };

    static constexpr size_t panelIndex(Panel p) { return static_cast<size_t>(p); }
    static constexpr size_t PANEL_COUNT = static_cast<size_t>(Panel::COUNT);

    void buildAllViews();
    ITomasuloView* getView(Panel panel);

    void renderSidebar(float width, float height);
    void renderContentPanel(float width, float height);
    void renderBottomBar(float width, float height);
    bool createSidebarButton(const char* label, bool selected, float width, float height);

    std::array<std::unique_ptr<ITomasuloView>, PANEL_COUNT> m_views;
    Panel m_selectedPanel = Panel::MainView;

    int m_untilSteps = 1;
    uint64_t m_cycleCount = 0;
};