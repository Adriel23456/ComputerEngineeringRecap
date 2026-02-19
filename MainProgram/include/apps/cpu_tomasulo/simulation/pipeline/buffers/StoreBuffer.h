// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/buffers/StoreBuffer.h
// ============================================================================

#pragma once

/**
 * @file StoreBuffer.h
 * @brief Store Buffer base class (Components 12–13).
 *
 * Manages one STR or STRB instruction through address calculation and
 * data collection. The actual memory write is deferred to commit time.
 *
 *   IDLE → ALLOCATED → (base + data ready) → AGU → (address ready) →
 *   STORE COMPLETE notification → ROB → FREED at commit
 *
 * Two independent operand dependencies:
 *   - Base address (Rn): same 3-tier resolution as load buffers.
 *   - Store data (Rm):   3-tier resolution via RD_StoreQiValid_o /
 *                        ROBReadReady2_o / CDB snoop.
 *
 * StoreComplete notification:
 *   When both address and data are known, StoreComplete signals are
 *   asserted combinationally (evaluate). ROB uses these to update its
 *   entry with the store address and data, enabling speculative forwarding.
 *
 * StoreComplete is NOT a commit signal — the Commit_Unit handles the
 * actual memory write when the store reaches the ROB head.
 *
 * Template Method pattern: SB0/SB1 override the bus accessors;
 * all state-machine logic lives in this base class.
 *
 * @note
 *   - SRP: Only manages the store pipeline for a single instruction slot.
 *   - OCP: Adding SB2 requires only a new subclass with overridden accessors.
 *   - DRY: Shared store logic avoids duplication across SB0/SB1.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class StoreBuffer : public ITomasuloComponent {
public:
    enum class ID : uint8_t { SB0 = 0, SB1 = 1 };

    explicit StoreBuffer(ID id);

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

    // ── Per-instance bus write accessors ─────────────────────────
    virtual void writeBusy(TomasuloBus& bus, bool v)                const = 0;
    virtual void writeAGUReq(TomasuloBus& bus, bool v)              const = 0;
    virtual void writeAGUBase(TomasuloBus& bus, uint64_t v)         const = 0;
    virtual void writeAGUOffset(TomasuloBus& bus, uint64_t v)       const = 0;
    virtual void writeAGUROBTag(TomasuloBus& bus, uint8_t v)        const = 0;
    virtual void writeStCompleteValid(TomasuloBus& bus, bool v)     const = 0;
    virtual void writeStCompleteROBTag(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeStCompleteAddr(TomasuloBus& bus, uint64_t v)  const = 0;
    virtual void writeStCompleteData(TomasuloBus& bus, uint64_t v)  const = 0;

    ID m_id;

private:
    // ── Store pipeline state ──────────────────────────────────────
    bool     m_busy = false;
    uint8_t  m_op = 0;       ///< STR (0x4F) or STRB (0x51).

    uint64_t m_baseValue = 0;
    uint8_t  m_baseTag = 0;       ///< ROB tag of base operand producer.
    bool     m_baseTagValid = false;

    uint64_t m_offset = 0;       ///< Sign-extended immediate offset.

    uint64_t m_address = 0;
    bool     m_addressReady = false;

    uint64_t m_dataValue = 0;
    uint8_t  m_dataTag = 0;       ///< ROB tag of data operand producer.
    bool     m_dataTagValid = false;

    uint8_t  m_robTag = 0;
    bool     m_segFault = false;
    bool     m_stCompleteNotified = false;   ///< True = ROB already notified.

    const char* idStr() const;
};