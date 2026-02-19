// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/cdb/CDB.h
// ============================================================================

#pragma once

/**
 * @file CDB.h
 * @brief Components 37-38: CDB_A and CDB_B. Pure wire routing.
 *
 * Both buses are populated by CDB_Arbiter with the two winning results.
 * CDB_A carries the highest-priority result; CDB_B carries the second.
 * All reservation stations, load buffers, and the ROB snoop both buses
 * every cycle to capture values matching their pending ROB tags.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

 /**
  * @brief CDB channel A — carries the first (highest-priority) broadcast result.
  * Purely combinational: copies ArbToCDBA_* fields onto CDBA_* fields.
  */
class CDB_A : public ITomasuloComponent {
public:
    const char* name() const override { return "CDB_A"; }
    /** @brief Routes ArbToCDBA_* signals onto CDBA_* broadcast wires. */
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< No sequential state.
    void reset() override {}                       ///< No state to reset.
};

/**
 * @brief CDB channel B — carries the second (next-priority) broadcast result.
 * Purely combinational: copies ArbToCDBB_* fields onto CDBB_* fields.
 */
class CDB_B : public ITomasuloComponent {
public:
    const char* name() const override { return "CDB_B"; }
    /** @brief Routes ArbToCDBB_* signals onto CDBB_* broadcast wires. */
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< No sequential state.
    void reset() override {}                       ///< No state to reset.
};