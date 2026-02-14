#pragma once

/**
 * @file ReservationStation.h
 * @brief Base class for Components 17-21 (RS_IntALU0/1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0).
 *
 * Common two-operand reservation station logic:
 *   - 3-tier operand resolution (Arch -> ROB forward -> CDB snoop)
 *   - 3-tier flags resolution  (Arch -> ROB forward -> CDB snoop + fallback)
 *   - CDB snooping for Vj, Vk, and Flags
 *   - Ready detection and dispatch/executing tracking
 *
 * Subclasses override bus accessors and behavioral queries:
 *   - isUnaryOp(op)   : FPALU unary, FSQRT
 *   - isMOVLikeOp(op) : MOV/MVN (skip Vj, only use Vk)
 *   - isMultiCycle()   : IntMUL/FPMUL use Executing bit instead of Dispatched
 *
 * @note RS_Branch0 is NOT derived from this — it has a fundamentally different
 *       structure (no register operands, only flags + target PC).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class ReservationStation : public ITomasuloComponent {
public:
    explicit ReservationStation(const char* name);

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

protected:
    // -- Behavioral queries (overridden by subclass) -------------
    virtual bool isMultiCycle() const { return false; }
    virtual bool isUnaryOp(uint8_t op) const { return false; }
    virtual bool isMOVLikeOp(uint8_t op) const { return false; }

    // -- Bus accessor interface (overridden per instance) --------
    virtual bool     readAlloc(const TomasuloBus& bus) const = 0;
    virtual bool     readFree(const TomasuloBus& bus) const = 0;
    virtual bool     readGrant(const TomasuloBus& bus) const = 0;

    virtual void writeBusy(TomasuloBus& bus, bool v) const = 0;
    virtual void writeReq(TomasuloBus& bus, bool v) const = 0;
    virtual void writeOp(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeVj(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeVk(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeDestROBTag(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeModFlags(TomasuloBus& bus, bool v) const = 0;
    virtual void writeNeedsFlags(TomasuloBus& bus, bool v) const = 0;
    virtual void writeFlagsValue(TomasuloBus& bus, uint8_t v) const = 0;

private:
    const char* m_name;

    // -- Internal state ------------------------------------------
    bool     m_busy = false;
    uint8_t  m_op = 0;

    uint64_t m_vj = 0;
    uint8_t  m_qj = 0;
    bool     m_qjValid = false;

    uint64_t m_vk = 0;
    uint8_t  m_qk = 0;
    bool     m_qkValid = false;

    uint8_t  m_destROBTag = 0;
    bool     m_useImm = false;
    bool     m_modifiesFlags = false;
    bool     m_needsFlags = false;

    uint8_t  m_flagsQi = 0;
    bool     m_flagsQiValid = false;
    uint8_t  m_flagsValue = 0;

    bool     m_dispatched = false;  // Single-cycle units (IntALU, FPALU)
    bool     m_executing = false;   // Multi-cycle units (IntMUL, FPMUL)
};