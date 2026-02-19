// ============================================================================
// File: src/apps/cpu_tomasulo/CpuTomasuloState.cpp
// ============================================================================

/**
 * @file CpuTomasuloState.cpp
 * @brief Implementation of CpuTomasuloState.
 *
 * Key responsibilities:
 *   1. buildAllViews()     — Instantiates all panel views.
 *   2. bindDataSources()   — Points table widgets at simulation data.
 *   3. wireCallbacks()     — Connects UI buttons to async controller.
 *   4. pollResults()       — Dispatches completed async results to views.
 *   5. renderContentPanel  — Locks sim mutex for data-dependent panels.
 */

#include "apps/cpu_tomasulo/CpuTomasuloState.h"
#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloMainView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloCompilerView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloRAMView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloICacheView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloDCacheView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloRegistersView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloROBView.h"
#include "apps/cpu_tomasulo/ui/views/TomasuloAnalysisView.h"

#include <imgui.h>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <cstring>

 // ============================================================================
 // Placeholder View  (anonymous, internal use only)
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
    m_controller.start();   // Start background worker thread
    bindDataSources();      // Point table widgets -> simulation data
    wireCallbacks();        // Connect UI buttons -> async controller
}

CpuTomasuloState::~CpuTomasuloState() {
    m_controller.stop();    // Join worker thread before views are destroyed
}

// ============================================================================
// View Management
// ============================================================================

void CpuTomasuloState::buildAllViews() {
    m_views[panelIndex(Panel::MainView)] = std::make_unique<TomasuloMainView>();
    m_views[panelIndex(Panel::Compiler)] = std::make_unique<TomasuloCompilerView>();
    m_views[panelIndex(Panel::RAM)] = std::make_unique<TomasuloRAMView>();
    m_views[panelIndex(Panel::ICache)] = std::make_unique<TomasuloICacheView>();
    m_views[panelIndex(Panel::DCache)] = std::make_unique<TomasuloDCacheView>();
    m_views[panelIndex(Panel::Registers)] = std::make_unique<TomasuloRegistersView>();
    m_views[panelIndex(Panel::ROB)] = std::make_unique<TomasuloROBView>();
    m_views[panelIndex(Panel::DataAnalysis)] = std::make_unique<TomasuloAnalysisView>();
}

ITomasuloView* CpuTomasuloState::getView(Panel panel) {
    return m_views[panelIndex(panel)].get();
}

// ============================================================================
// Data Source Binding  (table widgets -> simulation data, single source of truth)
// ============================================================================

void CpuTomasuloState::bindDataSources() {

    // RAM table reads directly from simulation RAM
    if (auto* ramView = dynamic_cast<TomasuloRAMView*>(getView(Panel::RAM))) {
        ramView->getTable().bindDataSource(&m_controller.cpu().ram());
    }

    // Register table reads directly from simulation register file
    if (auto* regView = dynamic_cast<TomasuloRegistersView*>(getView(Panel::Registers))) {
        regView->getTable().bindDataSource(&m_controller.cpu().registers());
    }
}

// ============================================================================
// Callback Wiring  (UI buttons -> async controller)
// ============================================================================

void CpuTomasuloState::wireCallbacks() {

    // ── Compile button ──────────────────────────────────────────
    if (auto* cv = dynamic_cast<TomasuloCompilerView*>(getView(Panel::Compiler))) {
        cv->setCompileCallback([this](const std::string& source) {
            m_controller.requestCompile(source);
            });
    }

    // ── RAM control buttons ─────────────────────────────────────
    if (auto* rv = dynamic_cast<TomasuloRAMView*>(getView(Panel::RAM))) {
        rv->setResetCallback([this]() {
            m_controller.requestResetRAM();
            });
        rv->setLoadBinaryCallback([this](const std::string& path) {
            m_controller.requestLoadBinary(path);
            });
    }
}

// ============================================================================
// Result Polling  (worker thread -> UI dispatch)
// ============================================================================

void CpuTomasuloState::pollResults() {
    if (!m_controller.hasResult()) return;

    SimTaskResult result = m_controller.consumeResult();

    // Update cycle counter from any task that reports it
    m_cycleCount = result.cycleCount;

    switch (result.type) {

    case SimTask::Type::Compile: {
        if (auto* cv = dynamic_cast<TomasuloCompilerView*>(getView(Panel::Compiler))) {
            cv->setCompileMessage(result.message, 6.0f);
        }
        break;
    }

    case SimTask::Type::LoadBinary: {
        if (auto* rv = dynamic_cast<TomasuloRAMView*>(getView(Panel::RAM))) {
            rv->setStatusMessage(result.message, 6.0f);
        }
        break;
    }

    case SimTask::Type::Reset: {
        if (auto* rv = dynamic_cast<TomasuloRAMView*>(getView(Panel::RAM))) {
            rv->setStatusMessage(result.message, 3.0f);
        }
        if (auto* av = dynamic_cast<TomasuloAnalysisView*>(getView(Panel::DataAnalysis))) {
            av->resetAll();
        }
        m_showSWIPopup = false;
        break;
    }

    case SimTask::Type::ResetRAM: {
        if (auto* rv = dynamic_cast<TomasuloRAMView*>(getView(Panel::RAM))) {
            rv->setStatusMessage(result.message, 3.0f);
        }
        break;
    }

    case SimTask::Type::Step:
    case SimTask::Type::StepUntil:
    case SimTask::Type::InfiniteStep: {
        if (result.message.find("SWI reached") != std::string::npos) {
            m_showSWIPopup = true;
        }
        break;
    }

    case SimTask::Type::InfiniteStepMS: {
        if (result.message.find("SWI reached") != std::string::npos) {
            m_showSWIPopup = true;
        }
        break;
    }

    default: break;

    } // switch
}

// ============================================================================
// Sim-lock Query
// ============================================================================

bool CpuTomasuloState::panelNeedsSimLock(Panel p) const {
    return p == Panel::MainView
        || p == Panel::RAM
        || p == Panel::Registers
        || p == Panel::ICache
        || p == Panel::DCache
        || p == Panel::ROB
        || p == Panel::DataAnalysis;
}

// ============================================================================
// State Interface
// ============================================================================

void CpuTomasuloState::handleEvent(sf::Event& event) {
    if (auto* view = getView(m_selectedPanel))
        view->handleEvent(event);
}

void CpuTomasuloState::update(float deltaTime) {
    // 1. Poll async results from the simulation worker
    pollResults();

    // 2. Update all views (timers, animations, etc.)
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

        // ── SWI Popup ───────────────────────────────────────────
        if (m_showSWIPopup) {
            ImGui::OpenPopup("##SWIPopup");
            m_showSWIPopup = false;  // Only trigger open once
        }

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(380, 0), ImGuiCond_Appearing);

        if (ImGui::BeginPopupModal("##SWIPopup", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {

            ImGui::Dummy(ImVec2(1, 8));
            ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f),
                "  SWI reached during execution");
            ImGui::Dummy(ImVec2(1, 4));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(1, 4));

            char msg[96];
            std::snprintf(msg, sizeof(msg), "CPU halted at cycle %llu.",
                static_cast<unsigned long long>(m_cycleCount));
            ImGui::TextWrapped("%s", msg);

            ImGui::Dummy(ImVec2(1, 8));

            float btnW = 120.0f;
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - btnW) * 0.5f);
            if (ImGui::Button("OK", ImVec2(btnW, 32))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::Dummy(ImVec2(1, 4));
            ImGui::EndPopup();
        }
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
// Content Panel  (non-blocking sim mutex lock for data-dependent views)
// ============================================================================

void CpuTomasuloState::renderContentPanel(float width, float height) {
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##TomasuloContent", ImVec2(width, height), true, flags);

    if (auto* view = getView(m_selectedPanel)) {
        bool locked = false;
        if (panelNeedsSimLock(m_selectedPanel)) {
            locked = m_controller.mutex().try_lock();  // NON-BLOCKING
        }

        if (locked || !panelNeedsSimLock(m_selectedPanel)) {
            // Sync simulation data -> UI widgets before rendering
            switch (m_selectedPanel) {
            case Panel::MainView:     syncMainView();     break;
            case Panel::ICache:       syncICacheView();   break;
            case Panel::DCache:       syncDCacheView();   break;
            case Panel::ROB:          syncROBView();      break;
            case Panel::DataAnalysis: syncAnalysisView(); break;
            default: break;
            }
        }

        // Always render — uses latest synced data, or stale data if lock failed
        view->render();

        if (locked) {
            m_controller.mutex().unlock();
        }
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

    bool busy = m_controller.isBusy();
    bool infinite = m_controller.isRunningInfinite();

    // ── RESET (red) ─────────────────────────────────────────────
    float wReset = ImGui::CalcTextSize("RESET").x + TEXT_PADDING;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.82f, 0.22f, 0.22f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.60f, 0.12f, 0.12f, 1.0f));
    if (ImGui::Button("RESET", ImVec2(wReset, BUTTON_HEIGHT))) {
        if (infinite) m_controller.requestStopInfinite();
        m_controller.requestReset();
        m_cycleCount = 0;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0.0f, GAP);

    // ── STEP ────────────────────────────────────────────────────
    float wStep = ImGui::CalcTextSize("STEP").x + TEXT_PADDING;
    bool stepDisabled = busy || infinite;
    if (stepDisabled) ImGui::BeginDisabled();
    if (ImGui::Button("STEP", ImVec2(wStep, BUTTON_HEIGHT))) {
        m_controller.requestStep();
    }
    if (stepDisabled) ImGui::EndDisabled();

    ImGui::SameLine(0.0f, GAP);

    // ── StepUntil (with cycle-count input) ──────────────────────
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
        if (stepDisabled) ImGui::BeginDisabled();
        if (ImGui::Button("StepUntil", ImVec2(wStepUntil, BUTTON_HEIGHT))) {
            m_controller.requestStepUntil(m_untilSteps);
        }
        if (stepDisabled) ImGui::EndDisabled();
    }
    ImGui::EndGroup();

    ImGui::SameLine(0.0f, GAP);

    // ── InfiniteStep (green) ─────────────────────────────────────
    float wInfinite = ImGui::CalcTextSize("InfiniteStep").x + TEXT_PADDING;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.55f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.68f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.09f, 0.45f, 0.16f, 1.0f));
    if (stepDisabled) ImGui::BeginDisabled();
    if (ImGui::Button("InfiniteStep", ImVec2(wInfinite, BUTTON_HEIGHT))) {
        m_controller.requestInfiniteStep();
    }
    if (stepDisabled) ImGui::EndDisabled();
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0.0f, GAP);

    // ── Delay Selector (cycles through values on click) ──────────
    char msLabel[16];
    std::snprintf(msLabel, sizeof(msLabel), "%dms", kStepMsValues[m_stepMsIndex]);
    float wMsBtn = ImGui::CalcTextSize("500ms").x + 16.0f;  // fixed width to prevent layout shift
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.30f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.42f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.18f, 0.18f, 0.22f, 1.0f));
    if (ImGui::Button(msLabel, ImVec2(wMsBtn, BUTTON_HEIGHT))) {
        m_stepMsIndex = (m_stepMsIndex + 1) % kStepMsCount;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0.0f, 4.0f);

    // ── StepMS (teal) ────────────────────────────────────────────
    float wStepMs = ImGui::CalcTextSize("StepMS").x + TEXT_PADDING;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.45f, 0.55f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.58f, 0.68f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.08f, 0.38f, 0.45f, 1.0f));
    if (stepDisabled) ImGui::BeginDisabled();
    if (ImGui::Button("StepMS", ImVec2(wStepMs, BUTTON_HEIGHT))) {
        m_controller.requestInfiniteStepMS(kStepMsValues[m_stepMsIndex]);
    }
    if (stepDisabled) ImGui::EndDisabled();
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0.0f, GAP);

    // ── STOP ─────────────────────────────────────────────────────
    float wStop = ImGui::CalcTextSize("STOP").x + TEXT_PADDING;
    bool stopDisabled = !infinite;
    if (stopDisabled) ImGui::BeginDisabled();
    if (ImGui::Button("STOP", ImVec2(wStop, BUTTON_HEIGHT))) {
        m_controller.requestStopInfinite();
    }
    if (stopDisabled) ImGui::EndDisabled();

    ImGui::SameLine(0.0f, GAP * 2.0f);

    // ── Cycle Counter ────────────────────────────────────────────
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

    if (selected) ImGui::PopStyleColor(3);

    return clicked;
}

// ============================================================================
// Typed View Accessor
// ============================================================================

TomasuloMainView* CpuTomasuloState::getMainView() {
    return dynamic_cast<TomasuloMainView*>(getView(Panel::MainView));
}

// ============================================================================
// I-Cache Sync
// ============================================================================

void CpuTomasuloState::syncICacheView() {
    auto* view = dynamic_cast<TomasuloICacheView*>(getView(Panel::ICache));
    if (!view) return;

    const I_Cache& ic = m_controller.cpu().iCache();
    CacheTable& table = view->getTable();

    for (int s = 0; s < ic.numSets(); ++s) {
        for (int w = 0; w < ic.numWays(); ++w) {
            bool     valid = ic.lineValid(s, w);
            uint64_t tag = ic.lineTag(s, w);

            // Format tag as hex string
            char tagStr[24];
            std::snprintf(tagStr, sizeof(tagStr), "0x%014llX",
                static_cast<unsigned long long>(tag));

            // Convert uint64_t[8] -> uint8_t[64] (little-endian word packing)
            std::array<uint8_t, CacheTable::kLineSizeBytes> bytes{};
            const uint64_t* words = ic.lineData(s, w);
            if (words) {
                for (int i = 0; i < 8; ++i) {
                    std::memcpy(&bytes[i * 8], &words[i], 8);
                }
            }

            table.setLineBySetWay(s, w, std::string(tagStr), bytes, valid);
        }
    }
}

// ============================================================================
// D-Cache Sync
// ============================================================================

void CpuTomasuloState::syncDCacheView() {
    auto* view = dynamic_cast<TomasuloDCacheView*>(getView(Panel::DCache));
    if (!view) return;

    const D_Cache& dc = m_controller.cpu().dCache();
    CacheTable& table = view->getTable();

    for (int s = 0; s < dc.numSets(); ++s) {
        for (int w = 0; w < dc.numWays(); ++w) {
            bool     valid = dc.lineValid(s, w);
            uint64_t tag = dc.lineTag(s, w);

            char tagStr[24];
            std::snprintf(tagStr, sizeof(tagStr), "0x%014llX",
                static_cast<unsigned long long>(tag));

            std::array<uint8_t, CacheTable::kLineSizeBytes> bytes{};
            const uint8_t* data = dc.lineData(s, w);
            if (data) {
                std::memcpy(bytes.data(), data, CacheTable::kLineSizeBytes);
            }

            table.setLineBySetWay(s, w, std::string(tagStr), bytes, valid);
        }
    }
}

// ============================================================================
// ROB Sync
// ============================================================================

void CpuTomasuloState::syncROBView() {
    auto* view = dynamic_cast<TomasuloROBView*>(getView(Panel::ROB));
    if (!view) return;

    const ROB& rob = m_controller.cpu().rob();
    ROBTable& table = view->getTable();

    table.setHead(rob.head());
    table.setTail(rob.tail());
    table.setCount(rob.count());

    for (int i = 0; i < ROBTable::kEntryCount; ++i) {
        ROB::EntryView  ev = rob.getEntryView(i);
        ROBTable::ROBEntry& te = table.getEntry(i);

        // ── Core fields (main table) ────────────────────────────
        te.busy = ev.busy;
        te.ready = ev.ready;
        te.type = ev.type;
        te.destReg = ev.destReg;
        te.value = ev.value;
        te.exception = ev.exception;
        te.pc = ev.pc;
        te.sourceStation = ev.sourceStation;

        // ── Flags fields (detail panel) ─────────────────────────
        te.flagsResult = ev.flagsResult;
        te.flagsValid = ev.flagsValid;
        te.modifiesFlags = ev.modifiesFlags;

        // ── Branch fields (detail panel) ────────────────────────
        te.predicted = ev.predicted;
        te.branchTaken = ev.mispredict ? !ev.predicted : ev.predicted;
        te.branchTarget = ev.branchTarget;
        te.mispredict = ev.mispredict;

        // ── Store fields (detail panel) ──────────────────────────
        te.storeAddr = ev.storeAddr;
        te.storeData = ev.storeData;
        te.storeReady = ev.storeReady;
    }
}

// ============================================================================
// Analysis Sync
// ============================================================================

void CpuTomasuloState::syncAnalysisView() {
    auto* view = dynamic_cast<TomasuloAnalysisView*>(getView(Panel::DataAnalysis));
    if (!view) return;

    const auto& cpu = m_controller.cpu();
    const auto& stats = cpu.stats();

    view->setTotalCycles(cpu.cycleCount());
    view->setCommittedInstructions(stats.committedInstructions);
    view->setCommittedMemoryInstructions(stats.committedMemoryInstructions);
    view->setBranchMispredictions(stats.branchMispredictions);

    // Cache latency = total miss events (how many times we missed)
    view->setCyclesLostCacheLatency(
        cpu.iCache().missCount() + cpu.dCache().missCount());

    // RAM latency = actual cycles spent stalled waiting for RAM
    view->setCyclesLostRAMLatency(
        cpu.iCache().missStallCycles() + cpu.dCache().missStallCycles());

    // Station usage (index matches station order in the pipeline)
    view->setUsesSB(0, stats.stationUses[0]);
    view->setUsesSB(1, stats.stationUses[1]);
    view->setUsesLB(0, stats.stationUses[2]);
    view->setUsesLB(1, stats.stationUses[3]);
    view->setUsesLB(2, stats.stationUses[4]);
    view->setUsesRS(0, stats.stationUses[5]);
    view->setUsesRS(1, stats.stationUses[6]);
    view->setUsesRS(2, stats.stationUses[7]);
    view->setUsesRS(3, stats.stationUses[8]);
    view->setUsesRS(4, stats.stationUses[9]);
    view->setUsesRS(5, stats.stationUses[10]);
}

// ============================================================================
// Main View Sync
// ============================================================================

void CpuTomasuloState::syncMainView() {
    auto* view = dynamic_cast<TomasuloMainView*>(getView(Panel::MainView));
    if (!view) return;
    view->setLabels(m_controller.cpu().tracker().labels());
}