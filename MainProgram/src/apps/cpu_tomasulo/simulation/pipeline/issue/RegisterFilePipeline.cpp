// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/issue/RegisterFilePipeline.cpp
// ============================================================================

/**
 * @file RegisterFilePipeline.cpp
 * @brief Component 9 implementation — Register File with per-register Qi tags.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/issue/RegisterFilePipeline.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRegisterFile.h"
#include <iostream>

 // ============================================================================
 // Construction
 // ============================================================================

RegisterFilePipeline::RegisterFilePipeline(TomasuloRegisterFile* archFile)
    : m_archFile(archFile)
{
    m_qi.fill(0);
    m_qiValid.fill(false);
}

// ============================================================================
// Read Port Helper
// ============================================================================

/**
 * @brief Reads one register and returns either its architectural value or its
 *        rename tag, depending on whether the register is currently in-flight.
 */
void RegisterFilePipeline::readPort(uint8_t regIdx,
    const TomasuloBus& /*bus*/,
    uint64_t& value,
    uint8_t& qi,
    bool& qiValid) const
{
    if (regIdx < NUM_REGS && m_qiValid[regIdx]) {
        // Register is renamed — consumer must wait for the ROB entry to complete.
        qi = m_qi[regIdx];
        qiValid = true;
        value = 0;  // Value not yet available.
    }
    else {
        // Register is architecturally current — read value directly.
        qi = 0;
        qiValid = false;
        value = (regIdx < NUM_REGS) ? m_archFile->get(regIdx) : 0;
    }
}

// ============================================================================
// Evaluate (combinational)
// ============================================================================

void RegisterFilePipeline::evaluate(TomasuloBus& bus) {

    // ── Port 1: Rn (first source operand) ────────────────────────
    readPort(bus.Rn_in_o, bus,
        bus.RD1_Value_o, bus.RD1_Qi_o, bus.RD1_QiValid_o);

    // ── Port 2: Rm (second source operand) ───────────────────────
    // The RS will later mux in the immediate value when UseImm is set;
    // we always read Rm here to keep this component ISA-agnostic.
    readPort(bus.Rm_in_o, bus,
        bus.RD2_Value_o, bus.RD2_Qi_o, bus.RD2_QiValid_o);

    // ── Port 3: Store data (Rm again for STR instructions) ────────
    // In this ISA the STR data source is encoded in the Rm field, not Rd.
    // Reading Rm a second time gives the store buffer its operand tag.
    readPort(bus.Rm_in_o, bus,
        bus.RD_StoreValue_o, bus.RD_StoreQi_o, bus.RD_StoreQiValid_o);

    // ── Segmentation registers (always architectural, never renamed) ─
    bus.UPPER_o = m_archFile->get(TomasuloRegisterFile::UPPER);
    bus.LOWER_o = m_archFile->get(TomasuloRegisterFile::LOWER);
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void RegisterFilePipeline::clockEdge(TomasuloBus& bus) {

    // ── 1. Flush — clear all rename tags ────────────────────────
    // On a branch misprediction the speculative state is discarded.
    // Architectural values are untouched because only committed instructions
    // write to the architectural file.
    if (bus.Flush_o) {
        for (int r = 0; r < NUM_REGS; ++r)
            m_qiValid[r] = false;

        // Mirror the cleared tags into the architectural file for UI visibility.
        for (int r = 0; r < NUM_REGS; ++r)
            m_archFile->setQi(r, 0, false);

        return;  // Skip commit/issue writes that belong to the flushed instruction.
    }

    // ── 2. Commit: update architectural value and retire rename tag ─
    if (bus.CommitWrEn_i) {
        uint8_t reg = bus.CommitWrAddr_i;
        if (reg < NUM_REGS) {
            m_archFile->set(reg, bus.CommitWrData_i);

            // Clear the tag only if it still points to the committed entry.
            // A later issued instruction may have already overwritten the tag
            // with a newer ROB index — in that case, leave the tag intact.
            if (m_qiValid[reg] && m_qi[reg] == bus.CommitROBIdx_i) {
                m_qiValid[reg] = false;
            }
        }
    }

    // ── 3. Issue: write rename tag for destination register ──────
    if (bus.ROBAlloc_o && bus.TagWrEn_o) {
        uint8_t dest = bus.Dest_in_o;
        if (dest < NUM_REGS) {
            m_qi[dest] = bus.ROBTail_o;
            m_qiValid[dest] = true;
        }
    }

    // ── 4. Mirror tags to architectural file for UI display ──────
    for (int r = 0; r < NUM_REGS; ++r)
        m_archFile->setQi(r, m_qi[r], m_qiValid[r]);
}

// ============================================================================
// Reset
// ============================================================================

void RegisterFilePipeline::reset() {
    m_qi.fill(0);
    m_qiValid.fill(false);
    // Sync cleared tags to the architectural file so the UI shows clean state.
    for (int r = 0; r < NUM_REGS; ++r)
        m_archFile->setQi(r, 0, false);
    std::cout << "[RegisterFile] reset(): all Qi tags cleared.\n";
}