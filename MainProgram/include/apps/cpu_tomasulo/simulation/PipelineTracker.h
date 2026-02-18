#pragma once

#include "apps/cpu_tomasulo/ui/widgets/TomasuloDisassembler.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <array>
#include <string>

class PipelineTracker {
public:
    static constexpr int SLOT_COUNT = 13;
    enum Slot {
        ISSUE = 0,
        SB0 = 1, SB1 = 2,
        LB0 = 3, LB1 = 4, LB2 = 5,
        RS_INTALU0 = 6, RS_INTALU1 = 7, RS_FPALU0 = 8,
        RS_INTMUL0 = 9, RS_FPMUL0 = 10, RS_BRANCH0 = 11,
        COMMIT = 12
    };

    void update(const TomasuloBus& bus) {
        // ── Flush ───────────────────────────────────────────────
        if (bus.Flush_o) {
            for (int i = SB0; i <= RS_BRANCH0; ++i)
                m_labels[i] = "---";
            m_labels[COMMIT] = "---";
            for (auto& s : m_robText) s.clear();
        }

        // ── Free signals ────────────────────────────────────────
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

        // ── Issue slot ──────────────────────────────────────────
        if (bus.InstReady_o && !bus.StallIF_o) {
            m_labels[ISSUE] = TomasuloDisassembler::disassemble(bus.InstrF_o);
        }
        else {
            m_labels[ISSUE] = "---";
        }

        // ── Allocation ──────────────────────────────────────────
        if (bus.ROBAlloc_o) {
            uint8_t robTag = bus.ROBTail_o;
            std::string text = TomasuloDisassembler::disassemble(bus.InstrF_o);
            if (robTag < 32) m_robText[robTag] = text;

            uint8_t ss = bus.AllocSourceStation_o;
            if (ss <= 10) {
                m_labels[ss + 1] = text;
            }
        }

        // ── Commit slot: show what's being committed this cycle ─
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

    void reset() {
        for (auto& l : m_labels) l = "---";
        for (auto& s : m_robText) s.clear();
    }

    const std::array<std::string, SLOT_COUNT>& labels() const { return m_labels; }

private:
    std::array<std::string, SLOT_COUNT> m_labels;
    std::string m_robText[32] = {};
};