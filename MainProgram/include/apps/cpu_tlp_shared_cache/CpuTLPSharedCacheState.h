// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.h
// ============================================================================

#pragma once

/**
 * @file CpuTLPSharedCacheState.h
 * @brief Main state coordinator for CPU TLP simulation.
 *
 * Manages simulation components and UI views for a 4-PE
 * system with shared L1 caches and MESI coherency.
 *
 * @note Follows:
 *   - SRP: Coordinates components, doesn't implement simulation logic
 *   - DIP: Depends on abstractions (ICpuTLPView, component interfaces)
 */

#include "core/fsm/State.h"
#include <memory>
#include <array>
#include <deque>
#include <string>
#include <cstdint>

 // Forward declarations
namespace sf { class RenderWindow; }
namespace cpu_tlp {
    struct CPUSystemSharedData;
    class InstructionMemoryComponent;
    class SharedMemoryComponent;
    class InterconnectComponent;
    class L1Component;
    class PEComponent;  // <-- CHANGED: Single generic PE class
}

class StateManager;
class ICpuTLPView;
class PECPUView;
class PERegView;
class PEMemView;

/**
 * @class CpuTLPSharedCacheState
 * @brief Main simulation state coordinator.
 */
class CpuTLPSharedCacheState : public State {
public:
    CpuTLPSharedCacheState(StateManager* sm, sf::RenderWindow* win);
    ~CpuTLPSharedCacheState() override;

    // Prevent copying
    CpuTLPSharedCacheState(const CpuTLPSharedCacheState&) = delete;
    CpuTLPSharedCacheState& operator=(const CpuTLPSharedCacheState&) = delete;

    // ========================================================================
    // State Interface
    // ========================================================================

    void handleEvent(sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;

    // ========================================================================
    // PE Control Methods
    // ========================================================================

    void resetPE(int peIndex);
    void stepPE(int peIndex);
    void stepUntilPE(int peIndex, int steps);
    void stepIndefinitelyPE(int peIndex);
    void stopPE(int peIndex);
    void resetAnalysis();

private:
    // ========================================================================
    // Panel Enumeration
    // ========================================================================

    enum class Panel {
        Compiler = 0,
        GeneralView,
        PE0CPU, PE0Reg, PE0Mem,
        PE1CPU, PE1Reg, PE1Mem,
        PE2CPU, PE2Reg, PE2Mem,
        PE3CPU, PE3Reg, PE3Mem,
        RAM,
        AnalysisData,
        COUNT
    };

    static constexpr size_t panelIndex(Panel p) { return static_cast<size_t>(p); }
    static constexpr size_t PANEL_COUNT = static_cast<size_t>(Panel::COUNT);

    // ========================================================================
    // Initialization
    // ========================================================================

    void registerControlCallbacks();
    void buildAllViews();
    ICpuTLPView* getView(Panel panel);

    // ========================================================================
    // Update Helpers
    // ========================================================================

    void updateRegisterViews();
    void updateCPUViews();
    void checkSWIEvents();
    void updateAnalysisView();
    void updateCacheViews();
    void updateSingleCacheView(int peId);

    // ========================================================================
    // Rendering Helpers
    // ========================================================================

    void renderSidebar(float width, float height);
    void renderContentPanel(float width, float height);
    bool createSidebarButton(const char* label, bool selected, float width, float height);

    // ========================================================================
    // SWI Popup Management
    // ========================================================================

    std::string makeSwiPopupId(int peId);
    void renderSWIPopups();

    // ========================================================================
    // Utility
    // ========================================================================

    static std::string formatMesiState(uint8_t state);

    // ========================================================================
    // Typed View Accessors
    // ========================================================================

    PECPUView* getCPUView(int peIndex);
    PERegView* getRegView(int peIndex);
    PEMemView* getMemView(int peIndex);

    // ========================================================================
    // Data Members
    // ========================================================================

    // Shared data
    std::shared_ptr<cpu_tlp::CPUSystemSharedData> m_sharedData;

    // Simulation components
    std::unique_ptr<cpu_tlp::InstructionMemoryComponent> m_instructionMemory;
    std::unique_ptr<cpu_tlp::SharedMemoryComponent> m_sharedMemoryComponent;
    std::unique_ptr<cpu_tlp::InterconnectComponent> m_interconnect;
    std::array<std::unique_ptr<cpu_tlp::L1Component>, 4> m_l1Caches;

    // Processing element components (generic PEComponent array)
    std::array<std::unique_ptr<cpu_tlp::PEComponent>, 4> m_pes;

    // UI views
    std::array<std::unique_ptr<ICpuTLPView>, PANEL_COUNT> m_views;
    Panel m_selectedPanel = Panel::Compiler;

    // SWI tracking
    std::array<uint32_t, 4> m_swiSeenCount{};
    std::deque<int> m_swiQueue;
    int m_activeSwiPopupPe = -1;

    // Cache update throttling
    float m_cacheUpdateTimer = 0.0f;
    static constexpr float CACHE_UPDATE_INTERVAL = 0.05f;
};