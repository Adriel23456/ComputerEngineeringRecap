#pragma once

/**
 * @file CpuTomasuloState.h
 * @brief Main state coordinator for Tomasulo CPU simulation UI.
 *
 * Owns the TomasuloSimController (which owns the simulation) and
 * all UI views. Wires async callbacks and handles result dispatch.
 *
 * Thread model:
 *   - This class runs entirely on the UI/main thread.
 *   - The simulation controller manages a background worker thread.
 *   - Render of data-dependent views is protected by the sim mutex.
 *
 * @note
 *   - SRP: Coordinates views and simulation, delegates all work
 *   - OCP: New panels added via Panel enum + buildAllViews
 */

#include "core/fsm/State.h"
#include "apps/cpu_tomasulo/simulation/TomasuloSimController.h"

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
    TomasuloSimController& controller() { return m_controller; }
    const TomasuloSimController& controller() const { return m_controller; }

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
    void bindDataSources();    ///< Points table widgets -> simulation data
    void wireCallbacks();      ///< Connects buttons -> async controller
    void pollResults();        ///< Checks for completed async tasks

    /** @brief True if the selected panel reads simulation state. */
    bool panelNeedsSimLock(Panel p) const;

    ITomasuloView* getView(Panel panel);

    void renderSidebar(float width, float height);
    void renderContentPanel(float width, float height);
    void renderBottomBar(float width, float height);
    bool createSidebarButton(const char* label, bool selected,
        float width, float height);

    // ── Simulation ──────────────────────────────────────────────
    TomasuloSimController m_controller;

    // ── UI ──────────────────────────────────────────────────────
    std::array<std::unique_ptr<ITomasuloView>, PANEL_COUNT> m_views;
    Panel    m_selectedPanel = Panel::MainView;
    int      m_untilSteps = 1;
    uint64_t m_cycleCount = 0;

    void syncICacheView();
    void syncDCacheView();
    void syncROBView();
    void syncAnalysisView();
};