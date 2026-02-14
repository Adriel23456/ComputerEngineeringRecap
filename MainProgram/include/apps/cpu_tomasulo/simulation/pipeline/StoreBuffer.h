#pragma once

/**
 * @file StoreBuffer.h
 * @brief Components 12-13: Store Buffer base class.
 *
 * Holds STR/STRB instructions while waiting for:
 *   1. Base address operand (Rn) to become ready
 *   2. Data operand (Rm/Rd) to become ready
 *   3. AGU to compute effective address
 *
 * After address + data are resolved, notifies ROB via StoreComplete.
 * Actual memory write happens only at COMMIT time.
 *
 * @note OCP: SB0 and SB1 inherit and override bus accessor methods.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class StoreBuffer : public ITomasuloComponent {
public:
    enum class ID : uint8_t { SB0 = 0, SB1 = 1 };

    explicit StoreBuffer(ID id);

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

protected:
    // ── Bus accessor interface (overridden per instance) ────────
    virtual bool     readAlloc(const TomasuloBus& bus) const = 0;
    virtual bool     readFree(const TomasuloBus& bus) const = 0;
    virtual bool     readAGUDone(const TomasuloBus& bus) const = 0;
    virtual uint64_t readAGUAddress(const TomasuloBus& bus) const = 0;
    virtual bool     readAGUSegFault(const TomasuloBus& bus) const = 0;

    virtual void writeBusy(TomasuloBus& bus, bool v) const = 0;
    virtual void writeAGUReq(TomasuloBus& bus, bool v) const = 0;
    virtual void writeAGUBase(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeAGUOffset(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeAGUROBTag(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeStCompleteValid(TomasuloBus& bus, bool v) const = 0;
    virtual void writeStCompleteROBTag(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeStCompleteAddr(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeStCompleteData(TomasuloBus& bus, uint64_t v) const = 0;

    ID m_id;

private:
    // ── Internal state ──────────────────────────────────────────
    bool     m_busy = false;
    uint8_t  m_op = 0;

    uint64_t m_baseValue = 0;
    uint8_t  m_baseTag = 0;
    bool     m_baseTagValid = false;

    uint64_t m_offset = 0;

    uint64_t m_address = 0;
    bool     m_addressReady = false;

    uint64_t m_dataValue = 0;
    uint8_t  m_dataTag = 0;
    bool     m_dataTagValid = false;

    uint8_t  m_robTag = 0;
    bool     m_segFault = false;
    bool     m_stCompleteNotified = false;

    const char* idStr() const;
};