/**
 * @file RegisterFilePipeline.cpp
 * @brief Component 9 implementation — Register File with Qi tags.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/RegisterFilePipeline.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/processor/TomasuloRegisterFile.h"
#include <iostream>

RegisterFilePipeline::RegisterFilePipeline(TomasuloRegisterFile* archFile)
    : m_archFile(archFile)
{
    m_qi.fill(0);
    m_qiValid.fill(false);
}

// ============================================================================
// Read Port Helper
// ============================================================================

void RegisterFilePipeline::readPort(uint8_t regIdx,
    const TomasuloBus& /*bus*/,
    uint64_t& value,
    uint8_t& qi,
    bool& qiValid) const
{
    if (regIdx < NUM_REGS && m_qiValid[regIdx]) {
        // Register is being renamed — consumer must wait on ROB
        qi = m_qi[regIdx];
        qiValid = true;
        value = 0;
    }
    else {
        // Register is architecturally current
        qi = 0;
        qiValid = false;
        value = (regIdx < NUM_REGS) ? m_archFile->get(regIdx) : 0;
    }
}

// ============================================================================
// Evaluate (combinational)
// ============================================================================

void RegisterFilePipeline::evaluate(TomasuloBus& bus) {
    // ── Read Port 1: Rn ─────────────────────────────────────────
    readPort(bus.Rn_in_o, bus,
        bus.RD1_Value_o, bus.RD1_Qi_o, bus.RD1_QiValid_o);

    // ── Read Port 2: Rm (always read, RS handles UseImm mux) ───
    readPort(bus.Rm_in_o, bus,
        bus.RD2_Value_o, bus.RD2_Qi_o, bus.RD2_QiValid_o);

    // ── Store data port: Rm value (for STR — data to store) ─────
    // In this ISA, STR encodes the data source register in Rm,
    // not Rd. This reads the same register as port 2, which is
    // correct — the ROB forwarding via ROBReadReady2/Value2
    // already looks up Rm's tag.
    readPort(bus.Rm_in_o, bus,
        bus.RD_StoreValue_o, bus.RD_StoreQi_o, bus.RD_StoreQiValid_o);

    // ── Segmentation registers ──────────────────────────────────
    bus.UPPER_o = m_archFile->get(TomasuloRegisterFile::UPPER);
    bus.LOWER_o = m_archFile->get(TomasuloRegisterFile::LOWER);
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void RegisterFilePipeline::clockEdge(TomasuloBus& bus) {
    // ── 1. Flush — clear all tags ───────────────────────────────
    if (bus.Flush_o) {
        for (int r = 0; r < NUM_REGS; ++r)
            m_qiValid[r] = false;
        std::cout << "[RegisterFile] FLUSH: all Qi tags cleared.\n";

        // Sync cleared tags to arch file for UI visibility
        for (int r = 0; r < NUM_REGS; ++r)
            m_archFile->setQi(r, 0, false);

        return;  // ← THE FIX: prevent spurious tag writes from the flushed issue
    }

    // ── 2. Commit write — update architectural register + clear tag
    if (bus.CommitWrEn_i) {
        uint8_t reg = bus.CommitWrAddr_i;
        if (reg < NUM_REGS) {
            m_archFile->set(reg, bus.CommitWrData_i);

            // Clear tag ONLY if it still points to the committed ROB entry.
            // A later instruction may have overwritten the tag.
            if (m_qiValid[reg] && m_qi[reg] == bus.CommitROBIdx_i) {
                m_qiValid[reg] = false;
                std::cout << "[RegisterFile] Commit: R" << (int)reg
                    << " = 0x" << std::hex << bus.CommitWrData_i
                    << std::dec << ", tag cleared (ROB#"
                    << (int)bus.CommitROBIdx_i << ")\n";
            }
            else {
                std::cout << "[RegisterFile] Commit: R" << (int)reg
                    << " = 0x" << std::hex << bus.CommitWrData_i
                    << std::dec << ", tag retained (newer rename)\n";
            }
        }
    }

    // ── 3. Issue — write Qi tag for destination register ────────
    if (bus.ROBAlloc_o && bus.TagWrEn_o) {
        uint8_t dest = bus.Dest_in_o;
        if (dest < NUM_REGS) {
            m_qi[dest] = bus.ROBTail_o;
            m_qiValid[dest] = true;
            std::cout << "[RegisterFile] Rename: R" << (int)dest
                << " -> ROB#" << (int)bus.ROBTail_o << "\n";
        }
    }

    // ── 4. Sync Qi tags to architectural file for UI visibility ──
    for (int r = 0; r < NUM_REGS; ++r) {
        m_archFile->setQi(r, m_qi[r], m_qiValid[r]);
    }
}

// ============================================================================
// Reset
// ============================================================================

void RegisterFilePipeline::reset() {
    m_qi.fill(0);
    m_qiValid.fill(false);
    // Sync cleared tags to arch file
    for (int r = 0; r < NUM_REGS; ++r) {
        m_archFile->setQi(r, 0, false);
    }
    std::cout << "[RegisterFile] reset(): all Qi tags cleared.\n";
}