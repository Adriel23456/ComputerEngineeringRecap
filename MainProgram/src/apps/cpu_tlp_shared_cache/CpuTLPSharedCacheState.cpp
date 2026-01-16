// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.cpp
// ============================================================================

/**
 * @file CpuTLPSharedCacheState.cpp
 * @brief Implementation of CpuTLPSharedCacheState.
 */

#include "apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.h"

 // Simulation components
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
#include "apps/cpu_tlp_shared_cache/simulation/memory/InstructionMemoryComponent.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/PEComponent.h"  // <-- CHANGED
#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/L1Component.h"

// UI Views
#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include "apps/cpu_tlp_shared_cache/ui/views/CompilerView.h"
#include "apps/cpu_tlp_shared_cache/ui/views/GeneralView.h"
#include "apps/cpu_tlp_shared_cache/ui/views/RAMView.h"
#include "apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.h"
#include "apps/cpu_tlp_shared_cache/ui/views/processor/PECPUView.h"
#include "apps/cpu_tlp_shared_cache/ui/views/processor/PERegView.h"
#include "apps/cpu_tlp_shared_cache/ui/views/processor/PEMemView.h"

// Utilities
#include "apps/cpu_tlp_shared_cache/ui/widgets/InstructionDisassembler.h"
#include "apps/cpu_tlp_shared_cache/CpuTLPControlAPI.h"

#include <imgui.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

// ============================================================================
// Construction / Destruction
// ============================================================================

CpuTLPSharedCacheState::CpuTLPSharedCacheState(StateManager* sm, sf::RenderWindow* win)
    : State(sm, win)
{
    // Initialize shared data
    m_sharedData = std::make_shared<cpu_tlp::CPUSystemSharedData>();
    m_sharedData->analysis.reset();

    // Initialize instruction memory
    m_instructionMemory = std::make_unique<cpu_tlp::InstructionMemoryComponent>();
    if (!m_instructionMemory->initialize(m_sharedData)) {
        std::cerr << "[CpuTLP] InstructionMemory initialization failed\n";
    }

    // Initialize shared memory
    m_sharedMemoryComponent = std::make_unique<cpu_tlp::SharedMemoryComponent>();
    if (!m_sharedMemoryComponent->initialize(m_sharedData)) {
        std::cerr << "[CpuTLP] SharedMemory initialization failed\n";
    }

    // Initialize interconnect
    m_interconnect = std::make_unique<cpu_tlp::InterconnectComponent>();
    if (!m_interconnect->initialize(m_sharedData, /*masters=*/4)) {
        std::cerr << "[CpuTLP] Interconnect initialization failed\n";
    }

    auto* bus = m_interconnect->raw();

    // Initialize L1 caches (using array)
    for (int i = 0; i < 4; ++i) {
        m_l1Caches[i] = std::make_unique<cpu_tlp::L1Component>(i);
        if (!m_l1Caches[i]->initialize(m_sharedData, bus)) {
            std::cerr << "[CpuTLP] L1Cache" << i << " initialization failed\n";
        }
    }

    // Initialize processing elements (using generic PEComponent)
    for (int i = 0; i < 4; ++i) {
        m_pes[i] = std::make_unique<cpu_tlp::PEComponent>(i);
        if (!m_pes[i]->initialize(m_sharedData)) {
            std::cerr << "[CpuTLP] PE" << i << " init failed\n";
        }
    }

    // Initialize SWI tracking
    m_swiSeenCount.fill(0);
    m_activeSwiPopupPe = -1;

    // Register callbacks and build views
    registerControlCallbacks();
    buildAllViews();
}

CpuTLPSharedCacheState::~CpuTLPSharedCacheState() {
    // Shutdown in reverse order

    // PEs first
    for (int i = 3; i >= 0; --i) {
        if (m_pes[i]) m_pes[i]->shutdown();
        m_pes[i].reset();
    }

    // L1 caches
    for (int i = 3; i >= 0; --i) {
        if (m_l1Caches[i]) m_l1Caches[i]->shutdown();
        m_l1Caches[i].reset();
    }

    // Interconnect
    if (m_interconnect) m_interconnect->shutdown();
    m_interconnect.reset();

    // Memory components
    if (m_sharedMemoryComponent) m_sharedMemoryComponent->shutdown();
    m_sharedMemoryComponent.reset();

    if (m_instructionMemory) m_instructionMemory->shutdown();
    m_instructionMemory.reset();
}

// ============================================================================
// Control Callback Registration
// ============================================================================

void CpuTLPSharedCacheState::registerControlCallbacks() {
    // PE0
    cpu_tlp_ui::onResetPE0 = [this]() { resetPE(0); };
    cpu_tlp_ui::onStepPE0 = [this]() { stepPE(0); };
    cpu_tlp_ui::onStepUntilPE0 = [this](int n) { stepUntilPE(0, n); };
    cpu_tlp_ui::onStepIndefinitelyPE0 = [this]() { stepIndefinitelyPE(0); };
    cpu_tlp_ui::onStopPE0 = [this]() { stopPE(0); };

    // PE1
    cpu_tlp_ui::onResetPE1 = [this]() { resetPE(1); };
    cpu_tlp_ui::onStepPE1 = [this]() { stepPE(1); };
    cpu_tlp_ui::onStepUntilPE1 = [this](int n) { stepUntilPE(1, n); };
    cpu_tlp_ui::onStepIndefinitelyPE1 = [this]() { stepIndefinitelyPE(1); };
    cpu_tlp_ui::onStopPE1 = [this]() { stopPE(1); };

    // PE2
    cpu_tlp_ui::onResetPE2 = [this]() { resetPE(2); };
    cpu_tlp_ui::onStepPE2 = [this]() { stepPE(2); };
    cpu_tlp_ui::onStepUntilPE2 = [this](int n) { stepUntilPE(2, n); };
    cpu_tlp_ui::onStepIndefinitelyPE2 = [this]() { stepIndefinitelyPE(2); };
    cpu_tlp_ui::onStopPE2 = [this]() { stopPE(2); };

    // PE3
    cpu_tlp_ui::onResetPE3 = [this]() { resetPE(3); };
    cpu_tlp_ui::onStepPE3 = [this]() { stepPE(3); };
    cpu_tlp_ui::onStepUntilPE3 = [this](int n) { stepUntilPE(3, n); };
    cpu_tlp_ui::onStepIndefinitelyPE3 = [this]() { stepIndefinitelyPE(3); };
    cpu_tlp_ui::onStopPE3 = [this]() { stopPE(3); };

    // Analysis
    cpu_tlp_ui::onResetAnalysis = [this]() { resetAnalysis(); };
}

// ============================================================================
// View Management
// ============================================================================

void CpuTLPSharedCacheState::buildAllViews() {
    // Compiler view with callback
    auto compilerView = std::make_unique<CompilerView>();
    compilerView->setCompileCallback([this](const std::string& /*source*/) {
        std::cout << "[CpuTLP] Compilation callback triggered\n";
        if (m_instructionMemory && m_instructionMemory->reloadInstructionMemory()) {
            std::cout << "[CpuTLP] Instruction memory reloaded\n";
        }
        });
    m_views[panelIndex(Panel::Compiler)] = std::move(compilerView);

    m_views[panelIndex(Panel::GeneralView)] = std::make_unique<GeneralView>();

    // PE views using generic classes (eliminates all duplication!)
    m_views[panelIndex(Panel::PE0CPU)] = std::make_unique<PECPUView>(0);
    m_views[panelIndex(Panel::PE0Reg)] = std::make_unique<PERegView>(0);
    m_views[panelIndex(Panel::PE0Mem)] = std::make_unique<PEMemView>(0);

    m_views[panelIndex(Panel::PE1CPU)] = std::make_unique<PECPUView>(1);
    m_views[panelIndex(Panel::PE1Reg)] = std::make_unique<PERegView>(1);
    m_views[panelIndex(Panel::PE1Mem)] = std::make_unique<PEMemView>(1);

    m_views[panelIndex(Panel::PE2CPU)] = std::make_unique<PECPUView>(2);
    m_views[panelIndex(Panel::PE2Reg)] = std::make_unique<PERegView>(2);
    m_views[panelIndex(Panel::PE2Mem)] = std::make_unique<PEMemView>(2);

    m_views[panelIndex(Panel::PE3CPU)] = std::make_unique<PECPUView>(3);
    m_views[panelIndex(Panel::PE3Reg)] = std::make_unique<PERegView>(3);
    m_views[panelIndex(Panel::PE3Mem)] = std::make_unique<PEMemView>(3);

    // RAM view
    auto ramView = std::make_unique<RAMView>();
    ramView->setSharedMemoryComponent(m_sharedMemoryComponent.get());
    m_views[panelIndex(Panel::RAM)] = std::move(ramView);

    m_views[panelIndex(Panel::AnalysisData)] = std::make_unique<AnalysisDataView>();
}

ICpuTLPView* CpuTLPSharedCacheState::getView(Panel panel) {
    return m_views[panelIndex(panel)].get();
}

PECPUView* CpuTLPSharedCacheState::getCPUView(int peIndex) {
    Panel panels[] = { Panel::PE0CPU, Panel::PE1CPU, Panel::PE2CPU, Panel::PE3CPU };
    if (peIndex >= 0 && peIndex < 4) {
        return dynamic_cast<PECPUView*>(getView(panels[peIndex]));
    }
    return nullptr;
}

PERegView* CpuTLPSharedCacheState::getRegView(int peIndex) {
    Panel panels[] = { Panel::PE0Reg, Panel::PE1Reg, Panel::PE2Reg, Panel::PE3Reg };
    if (peIndex >= 0 && peIndex < 4) {
        return dynamic_cast<PERegView*>(getView(panels[peIndex]));
    }
    return nullptr;
}

PEMemView* CpuTLPSharedCacheState::getMemView(int peIndex) {
    Panel panels[] = { Panel::PE0Mem, Panel::PE1Mem, Panel::PE2Mem, Panel::PE3Mem };
    if (peIndex >= 0 && peIndex < 4) {
        return dynamic_cast<PEMemView*>(getView(panels[peIndex]));
    }
    return nullptr;
}

// ============================================================================
// State Interface
// ============================================================================

void CpuTLPSharedCacheState::handleEvent(sf::Event& event) {
    if (auto* view = getView(m_selectedPanel)) {
        view->handleEvent(event);
    }
}

void CpuTLPSharedCacheState::update(float deltaTime) {
    updateRegisterViews();
    updateCPUViews();
    checkSWIEvents();

    // Throttled cache updates
    m_cacheUpdateTimer += deltaTime;
    if (m_cacheUpdateTimer >= CACHE_UPDATE_INTERVAL) {
        updateCacheViews();
        m_cacheUpdateTimer = 0.0f;
    }

    updateAnalysisView();

    // Update all views
    for (auto& view : m_views) {
        if (view) view->update(deltaTime);
    }
}

void CpuTLPSharedCacheState::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    if (ImGui::Begin("##CpuTLPSharedCache", nullptr, flags)) {
        // Handle SWI popup queue
        if (m_activeSwiPopupPe == -1 && !m_swiQueue.empty()) {
            m_activeSwiPopupPe = m_swiQueue.front();
            m_swiQueue.pop_front();
            ImGui::OpenPopup(makeSwiPopupId(m_activeSwiPopupPe).c_str());
        }

        ImVec2 available = ImGui::GetContentRegionAvail();
        const float sidebarWidth = available.x * 0.20f;
        const float separator = 4.0f;
        const float contentWidth = available.x - sidebarWidth - separator;

        renderSidebar(sidebarWidth, available.y);
        ImGui::SameLine(0.0f, separator);
        renderContentPanel(contentWidth, available.y);
        renderSWIPopups();
    }
    ImGui::End();
}

void CpuTLPSharedCacheState::renderBackground() {
    m_window->clear(sf::Color(20, 20, 25));
}

// ============================================================================
// Rendering Helpers
// ============================================================================

void CpuTLPSharedCacheState::renderSidebar(float width, float height) {
    ImGui::BeginChild("##LeftSidebar", ImVec2(width, height), true,
        ImGuiWindowFlags_AlwaysVerticalScrollbar);

    constexpr float TOP_PADDING = 14.0f;
    constexpr float BUTTON_HEIGHT = 56.0f;
    constexpr float BUTTON_SPACING = 10.0f;
    float buttonWidth = ImGui::GetContentRegionAvail().x;

    ImGui::Dummy(ImVec2(1, TOP_PADDING));

    struct NavItem { const char* label; Panel panel; };
    const NavItem items[] = {
        {"Compiler", Panel::Compiler},
        {"General View", Panel::GeneralView},
        {"PE0 CPU", Panel::PE0CPU},
        {"PE0 Reg", Panel::PE0Reg},
        {"PE0 Cache", Panel::PE0Mem},
        {"PE1 CPU", Panel::PE1CPU},
        {"PE1 Reg", Panel::PE1Reg},
        {"PE1 Cache", Panel::PE1Mem},
        {"PE2 CPU", Panel::PE2CPU},
        {"PE2 Reg", Panel::PE2Reg},
        {"PE2 Cache", Panel::PE2Mem},
        {"PE3 CPU", Panel::PE3CPU},
        {"PE3 Reg", Panel::PE3Reg},
        {"PE3 Cache", Panel::PE3Mem},
        {"RAM", Panel::RAM},
        {"Analysis Data", Panel::AnalysisData}
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

void CpuTLPSharedCacheState::renderContentPanel(float width, float height) {
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##RightPane", ImVec2(width, height), true, flags);

    if (auto* view = getView(m_selectedPanel)) {
        view->render();
    }

    ImGui::EndChild();
}

bool CpuTLPSharedCacheState::createSidebarButton(const char* label, bool selected,
    float width, float height) {
    ImVec2 textSize = ImGui::CalcTextSize(label);
    float paddingX = ImGui::GetStyle().FramePadding.x * 2.f;
    float available = width - paddingX;
    float scale = available > 0.f ? (available / std::max(1.f, textSize.x)) : 1.f;
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
// SWI Popup
// ============================================================================

std::string CpuTLPSharedCacheState::makeSwiPopupId(int peId) {
    return "SWI##PE" + std::to_string(peId);
}

void CpuTLPSharedCacheState::renderSWIPopups() {
    if (m_activeSwiPopupPe == -1) return;

    const int peId = m_activeSwiPopupPe;
    const std::string popupId = makeSwiPopupId(peId);
    const std::string message = "!PE" + std::to_string(peId) + " stopped via SWI!";

    ImVec2 textSize = ImGui::CalcTextSize(message.c_str());
    float minWidth = std::max(360.0f, textSize.x + 48.0f);
    float minHeight = std::max(160.0f, textSize.y + 80.0f);

    ImGui::SetNextWindowSize(ImVec2(minWidth, minHeight), ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal(popupId.c_str(), nullptr,
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextUnformatted(message.c_str());
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Go to PE", ImVec2(120, 0))) {
            Panel panels[] = { Panel::PE0CPU, Panel::PE1CPU, Panel::PE2CPU, Panel::PE3CPU };
            if (peId >= 0 && peId < 4) {
                m_selectedPanel = panels[peId];
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (!ImGui::IsPopupOpen(popupId.c_str())) {
        m_activeSwiPopupPe = -1;
    }
}

// ============================================================================
// Update Helpers
// ============================================================================

void CpuTLPSharedCacheState::updateRegisterViews() {
    for (int peId = 0; peId < 4; ++peId) {
        PERegView* view = getRegView(peId);
        if (!view) continue;

        auto& snapshot = m_sharedData->pe_registers[peId];
        for (int reg = 0; reg < 12; ++reg) {
            uint64_t value = snapshot.registers[reg].load(std::memory_order_acquire);
            view->setRegValueByIndex(reg, value);
        }
    }
}

void CpuTLPSharedCacheState::updateCPUViews() {
    for (int peId = 0; peId < 4; ++peId) {
        PECPUView* view = getCPUView(peId);
        if (!view) continue;

        auto& tracking = m_sharedData->pe_instruction_tracking[peId];
        std::array<std::string, 5> labels;

        // Seqlock read pattern
        for (int attempt = 0; attempt < 2; ++attempt) {
            uint64_t v1 = tracking.version.load(std::memory_order_acquire);
            if (v1 & 1ULL) continue;

            uint64_t raw[5];
            for (int i = 0; i < 5; ++i) {
                raw[i] = tracking.stage_instructions[i].load(std::memory_order_acquire);
            }

            uint64_t v2 = tracking.version.load(std::memory_order_acquire);
            if (v1 == v2 && !(v2 & 1ULL)) {
                for (int i = 0; i < 5; ++i) {
                    labels[i] = cpu_tlp::InstructionDisassembler::disassemble(raw[i]);
                }
                view->setLabels(labels);
                break;
            }
        }
    }
}

void CpuTLPSharedCacheState::checkSWIEvents() {
    for (int peId = 0; peId < 4; ++peId) {
        uint32_t seen = m_swiSeenCount[peId];
        uint32_t current = m_sharedData->ui_signals[peId].swi_count.load(std::memory_order_acquire);

        if (current > seen) {
            for (uint32_t k = 0; k < (current - seen); ++k) {
                m_swiQueue.push_back(peId);
            }
            m_swiSeenCount[peId] = current;
        }
    }
}

void CpuTLPSharedCacheState::updateAnalysisView() {
    auto* analysisView = dynamic_cast<AnalysisDataView*>(getView(Panel::AnalysisData));
    if (!analysisView) return;

    auto& analysis = m_sharedData->analysis;

    uint64_t traffic[4];
    for (int i = 0; i < 4; ++i) {
        traffic[i] = analysis.traffic_pe[i].load(std::memory_order_acquire);
    }

    analysisView->setTrafficPE0(traffic[0]);
    analysisView->setTrafficPE1(traffic[1]);
    analysisView->setTrafficPE2(traffic[2]);
    analysisView->setTrafficPE3(traffic[3]);
    analysisView->setReadWriteOps(traffic[0] + traffic[1] + traffic[2] + traffic[3]);
    analysisView->setCacheMisses(analysis.cache_misses.load(std::memory_order_acquire));
    analysisView->setInvalidations(analysis.invalidations.load(std::memory_order_acquire));
    analysisView->setTransactionsMESI(analysis.transactions_mesi.load(std::memory_order_acquire));
}

void CpuTLPSharedCacheState::updateCacheViews() {
    for (int peId = 0; peId < 4; ++peId) {
        updateSingleCacheView(peId);
    }
}

void CpuTLPSharedCacheState::updateSingleCacheView(int peId) {
    if (peId < 0 || peId >= 4) return;

    cpu_tlp::L1Component* l1 = m_l1Caches[peId].get();
    PEMemView* memView = getMemView(peId);

    if (!l1 || !memView) return;

    L1Cache* cache = l1->l1();
    if (!cache) return;

    for (int set = 0; set < 8; ++set) {
        for (int way = 0; way < 2; ++way) {
            CacheLineInfo info = cache->getLineInfo(set, way);

            std::ostringstream tagStream;
            tagStream << "0x" << std::hex << std::uppercase
                << std::setfill('0') << std::setw(14) << info.tag;

            std::string mesiStr = formatMesiState(static_cast<uint8_t>(info.state));
            memView->setBySetWay(set, way, tagStream.str(), info.data, mesiStr);
        }
    }
}

std::string CpuTLPSharedCacheState::formatMesiState(uint8_t state) {
    switch (state) {
    case 0: return "I";
    case 1: return "S";
    case 2: return "E";
    case 3: return "M";
    default: return "?";
    }
}

// ============================================================================
// Generic PE Control (simplified with array)
// ============================================================================

void CpuTLPSharedCacheState::resetPE(int peIndex) {
    if (peIndex < 0 || peIndex >= 4) {
        std::cerr << "[CpuTLP] Invalid PE index: " << peIndex << "\n";
        return;
    }

    if (m_pes[peIndex]) {
        m_pes[peIndex]->reset();
    }
    else {
        std::cerr << "[CpuTLP] PE" << peIndex << " is null!\n";
    }

    // Reset L1 cache
    if (m_l1Caches[peIndex] && m_l1Caches[peIndex]->l1()) {
        m_l1Caches[peIndex]->l1()->reset();
        std::cout << "[CpuTLP] L1Cache" << peIndex << " reset\n";
    }
}

void CpuTLPSharedCacheState::stepPE(int peIndex) {
    if (peIndex < 0 || peIndex >= 4) {
        std::cerr << "[CpuTLP] Invalid PE index: " << peIndex << "\n";
        return;
    }

    if (m_pes[peIndex]) {
        m_pes[peIndex]->step();
    }
    else {
        std::cerr << "[CpuTLP] PE" << peIndex << " is null!\n";
    }
}

void CpuTLPSharedCacheState::stepUntilPE(int peIndex, int steps) {
    if (peIndex < 0 || peIndex >= 4) {
        std::cerr << "[CpuTLP] Invalid PE index: " << peIndex << "\n";
        return;
    }

    if (m_pes[peIndex]) {
        m_pes[peIndex]->stepUntil(steps);
    }
    else {
        std::cerr << "[CpuTLP] PE" << peIndex << " is null!\n";
    }
}

void CpuTLPSharedCacheState::stepIndefinitelyPE(int peIndex) {
    if (peIndex < 0 || peIndex >= 4) {
        std::cerr << "[CpuTLP] Invalid PE index: " << peIndex << "\n";
        return;
    }

    if (m_pes[peIndex]) {
        m_pes[peIndex]->stepIndefinitely();
    }
    else {
        std::cerr << "[CpuTLP] PE" << peIndex << " is null!\n";
    }
}

void CpuTLPSharedCacheState::stopPE(int peIndex) {
    if (peIndex < 0 || peIndex >= 4) {
        std::cerr << "[CpuTLP] Invalid PE index: " << peIndex << "\n";
        return;
    }

    if (m_pes[peIndex]) {
        m_pes[peIndex]->stopExecution();
    }
    else {
        std::cerr << "[CpuTLP] PE" << peIndex << " is null!\n";
    }
}

void CpuTLPSharedCacheState::resetAnalysis() {
    if (!m_sharedData) return;

    m_sharedData->analysis.reset();

    if (auto* view = dynamic_cast<AnalysisDataView*>(getView(Panel::AnalysisData))) {
        view->reset();
    }
}