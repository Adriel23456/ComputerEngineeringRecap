// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/PipelineTracker.h
// ============================================================================

#pragma once

/**
 * @file PipelineTracker.h
 * @brief Cycle-accurate pipeline slot tracker for the Tomasulo diagram overlay.
 *
 * Reads signals from TomasuloBus after each cycle and maps them onto the
 * 13 named diagram slots used by TomasuloMainView / SlotOverlayRenderer:
 *
 *   Slot 0  : ISSUE      — instruction currently being decoded/issued
 *   Slots 1–2  : SB0–SB1    — store buffer occupancy
 *   Slots 3–5  : LB0–LB2    — load buffer occupancy
 *   Slots 6–11 : RS_*        — reservation station occupancy
 *   Slot 12 : COMMIT     — instruction currently being committed
 *
 * An internal 32-entry ROB text table is maintained so committed entries
 * can be labelled with their disassembled mnemonic even after they leave
 * the visible RS/buffer slots.
 *
 * Usage:
 *   Call update(bus) once per step() in TomasuloCPU, then
 *   call tracker().labels() from the UI thread (under the sim mutex)
 *   to push the labels into TomasuloMainView.
 *
 * @note
 *   - SRP: Only tracks pipeline slot labels — no rendering, no simulation logic.
 *   - Kept in simulation/ root because it bridges the bus signals to the UI
 *     without belonging to any single pipeline stage.
 */

#include "apps/cpu_tomasulo/ui/widgets/TomasuloDisassembler.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <array>
#include <string>

class PipelineTracker {
public:
    static constexpr int SLOT_COUNT = 13;

    /** @brief Slot indices matching TomasuloMainView::Slot exactly. */
    enum Slot {
        ISSUE = 0,
        SB0 = 1, SB1 = 2,
        LB0 = 3, LB1 = 4, LB2 = 5,
        RS_INTALU0 = 6, RS_INTALU1 = 7, RS_FPALU0 = 8,
        RS_INTMUL0 = 9, RS_FPMUL0 = 10, RS_BRANCH0 = 11,
        COMMIT = 12
    };

    /**
     * @brief Updates all slot labels from a freshly evaluated bus snapshot.
     *
     * Must be called exactly once per cycle, after
     * TomasuloCycleCoordinator::executeCycle() returns.
     *
     * Logic per slot:
     *   - Flush_o       → clear all execute/commit slots and ROB text table.
     *   - FreeXxx_o     → clear the corresponding slot to "---".
     *   - ROBAlloc_o    → store disassembly in the ROB text table and set
     *                     the corresponding RS/buffer slot.
     *   - CommitPop_i   → show the committing entry in COMMIT slot, then
     *                     erase its ROB text table entry.
     *   - InstReady_o   → update the ISSUE slot with the current instruction.
     *
     * @param bus  The bus snapshot for the completed cycle.
     */
    void update(const TomasuloBus& bus) {
        // ── Flush ─────────────────────────────────────────────────
        if (bus.Flush_o) {
            for (int i = SB0; i <= RS_BRANCH0; ++i)
                m_labels[i] = "---";
            m_labels[COMMIT] = "---";
            for (auto& s : m_robText) s.clear();
        }

        // ── Free signals ──────────────────────────────────────────
        if (bus.FreeSB0_o)      m_labels[SB0] = "---";
        if (bus.FreeSB1_o)      m_labels[SB1] = "---";
        if (bus.FreeLB0_o)      m_labels[LB0] = "---";
        if (bus.FreeLB1_o)      m_labels[LB1] = "---";
        if (bus.FreeLB2_o)      m_labels[LB2] = "---";
        if (bus.FreeRSIAU0_o)   m_labels[RS_INTALU0] = "---";
        if (bus.FreeRSIAU1_o)   m_labels[RS_INTALU1] = "---";
        if (bus.FreeRSFPAU0_o)  m_labels[RS_FPALU0] = "---";
        if (bus.FreeRSIMUL0_o)  m_labels[RS_INTMUL0] = "---";
        if (bus.FreeRSFPMUL0_o) m_labels[RS_FPMUL0] = "---";
        if (bus.FreeRSBr0_o)    m_labels[RS_BRANCH0] = "---";

        // ── Issue slot ────────────────────────────────────────────
        if (bus.InstReady_o && !bus.StallIF_o) {
            m_labels[ISSUE] = TomasuloDisassembler::disassemble(bus.InstrF_o);
        }
        else {
            m_labels[ISSUE] = "---";
        }

        // ── Allocation ────────────────────────────────────────────
        if (bus.ROBAlloc_o) {
            uint8_t     robTag = bus.ROBTail_o;
            std::string text = TomasuloDisassembler::disassemble(bus.InstrF_o);
            if (robTag < 32) m_robText[robTag] = text;

            // AllocSourceStation_o maps directly to slot index+1
            uint8_t ss = bus.AllocSourceStation_o;
            if (ss <= 10) m_labels[ss + 1] = text;
        }

        // ── Commit slot ───────────────────────────────────────────
        if (bus.CommitPop_i) {
            uint8_t tag = bus.ROBHead_o;
            if (tag < 32 && !m_robText[tag].empty()) {
                m_labels[COMMIT] = "ROB#" + std::to_string(tag) + " " + m_robText[tag];
                m_robText[tag].clear();
            }
            else {
                m_labels[COMMIT] = "ROB#" + std::to_string(tag);
            }
        }
        else {
            m_labels[COMMIT] = "---";
        }
    }

    /** @brief Clears all slot labels and the ROB text table (called on CPU reset). */
    void reset() {
        for (auto& l : m_labels) l = "---";
        for (auto& s : m_robText) s.clear();
    }

    /** @brief Returns the current label array for all 13 slots. */
    const std::array<std::string, SLOT_COUNT>& labels() const { return m_labels; }

private:
    std::array<std::string, SLOT_COUNT> m_labels;   ///< One label per diagram slot.
    std::string m_robText[32] = {};                 ///< Disassembly text per ROB entry.
};