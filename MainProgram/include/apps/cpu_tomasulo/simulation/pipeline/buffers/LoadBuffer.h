// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/buffers/LoadBuffer.h
// ============================================================================

#pragma once

/**
 * @file LoadBuffer.h
 * @brief Load Buffer base class (Components 14–16).
 *
 * Manages one LDR or LDRB instruction through the full memory pipeline:
 *
 *   IDLE → ALLOCATED → (base ready) → AGU → (address ready) →
 *   MEMORY READ → (data ready) → CDB BROADCAST → FREED
 *
 * Dependencies and resolution:
 *   - Base address operand (Rn): 3-tier resolution — architectural value,
 *     ROB forward, or wait for CDB broadcast (same as RS operands).
 *   - Offset: sign-extended immediate, latched at allocation (no dependency).
 *
 * Segmentation fault path:
 *   If AGU signals segFault, skip the memory request and publish an
 *   exception (code 0x2) on the CDB instead of data.
 *
 * The CDB requested flag (m_cdbRequested) prevents double-publishing
 * when CDB arbitration stalls the request for one or more cycles.
 *
 * Template Method pattern: LB0/LB1/LB2 override the read/write bus
 * accessors; all state-machine logic lives in this base class.
 *
 * @note
 *   - SRP: Only manages the load pipeline for a single instruction slot.
 *   - OCP: Adding LB3 requires only a new subclass with overridden accessors.
 *   - DRY: Shared load logic avoids duplication across LB0/LB1/LB2.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class LoadBuffer : public ITomasuloComponent {
public:
    enum class ID : uint8_t { LB0 = 0, LB1 = 1, LB2 = 2 };

    explicit LoadBuffer(ID id);

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

protected:
    // ── Per-instance bus read accessors ──────────────────────────
    virtual bool     readAlloc(const TomasuloBus& bus)      const = 0;
    virtual bool     readFree(const TomasuloBus& bus)       const = 0;
    virtual bool     readAGUDone(const TomasuloBus& bus)    const = 0;
    virtual uint64_t readAGUAddress(const TomasuloBus& bus) const = 0;
    virtual bool     readAGUSegFault(const TomasuloBus& bus)const = 0;
    virtual bool     readMemDone(const TomasuloBus& bus)    const = 0;
    virtual uint64_t readMemData(const TomasuloBus& bus)    const = 0;
    virtual bool     readCDBStall(const TomasuloBus& bus)   const = 0;

    // ── Per-instance bus write accessors ─────────────────────────
    virtual void writeBusy(TomasuloBus& bus, bool v)          const = 0;
    virtual void writeAGUReq(TomasuloBus& bus, bool v)        const = 0;
    virtual void writeAGUBase(TomasuloBus& bus, uint64_t v)   const = 0;
    virtual void writeAGUOffset(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeAGUROBTag(TomasuloBus& bus, uint8_t v)  const = 0;
    virtual void writeMemReq(TomasuloBus& bus, bool v)        const = 0;
    virtual void writeMemAddr(TomasuloBus& bus, uint64_t v)   const = 0;
    virtual void writeMemOp(TomasuloBus& bus, uint8_t v)      const = 0;
    virtual void writeMemROBTag(TomasuloBus& bus, uint8_t v)  const = 0;
    virtual void writeCDBReq(TomasuloBus& bus, bool v)        const = 0;
    virtual void writeCDBValue(TomasuloBus& bus, uint64_t v)  const = 0;
    virtual void writeCDBROBTag(TomasuloBus& bus, uint8_t v)  const = 0;
    virtual void writeCDBException(TomasuloBus& bus, uint8_t v) const = 0;

    ID m_id;

private:
    // ── Load pipeline state ───────────────────────────────────────
    bool     m_busy = false;
    uint8_t  m_op = 0;       ///< LDR (0x4E) or LDRB (0x50).

    uint64_t m_baseValue = 0;
    uint8_t  m_baseTag = 0;       ///< ROB tag of base operand producer.
    bool     m_baseTagValid = false;   ///< True = base is still pending.

    uint64_t m_offset = 0;       ///< Sign-extended immediate offset.

    uint64_t m_address = 0;
    bool     m_addressReady = false;

    uint64_t m_loadedData = 0;
    bool     m_memDoneInt = false;   ///< True = memory data has arrived.

    uint8_t  m_destROBTag = 0;
    bool     m_segFault = false;
    bool     m_cdbRequested = false;   ///< True = CDB request already sent.

    const char* idStr() const;
};