#pragma once

/**
 * @file LoadBuffer.h
 * @brief Components 14-16: Load Buffer base class.
 *
 * Holds LDR/LDRB instructions while waiting for:
 *   1. Base address operand (Rn) to become ready
 *   2. AGU to compute effective address
 *   3. Memory subsystem to return data
 *
 * After memory data arrives, publishes result on the CDB.
 * Unlike Store Buffers, Load Buffers broadcast on the CDB.
 *
 * @note OCP: LB0, LB1, LB2 inherit and override bus accessor methods.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class LoadBuffer : public ITomasuloComponent {
public:
    enum class ID : uint8_t { LB0 = 0, LB1 = 1, LB2 = 2 };

    explicit LoadBuffer(ID id);

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

protected:
    // -- Bus accessor interface (overridden per instance) --------
    virtual bool     readAlloc(const TomasuloBus& bus) const = 0;
    virtual bool     readFree(const TomasuloBus& bus) const = 0;
    virtual bool     readAGUDone(const TomasuloBus& bus) const = 0;
    virtual uint64_t readAGUAddress(const TomasuloBus& bus) const = 0;
    virtual bool     readAGUSegFault(const TomasuloBus& bus) const = 0;
    virtual bool     readMemDone(const TomasuloBus& bus) const = 0;
    virtual uint64_t readMemData(const TomasuloBus& bus) const = 0;
    virtual bool     readCDBStall(const TomasuloBus& bus) const = 0;

    virtual void writeBusy(TomasuloBus& bus, bool v) const = 0;
    virtual void writeAGUReq(TomasuloBus& bus, bool v) const = 0;
    virtual void writeAGUBase(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeAGUOffset(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeAGUROBTag(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeMemReq(TomasuloBus& bus, bool v) const = 0;
    virtual void writeMemAddr(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeMemOp(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeMemROBTag(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeCDBReq(TomasuloBus& bus, bool v) const = 0;
    virtual void writeCDBValue(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeCDBROBTag(TomasuloBus& bus, uint8_t v) const = 0;
    virtual void writeCDBException(TomasuloBus& bus, uint8_t v) const = 0;

    ID m_id;

private:
    // -- Internal state ------------------------------------------
    bool     m_busy = false;
    uint8_t  m_op = 0;

    uint64_t m_baseValue = 0;
    uint8_t  m_baseTag = 0;
    bool     m_baseTagValid = false;

    uint64_t m_offset = 0;

    uint64_t m_address = 0;
    bool     m_addressReady = false;

    uint64_t m_loadedData = 0;
    bool     m_memDoneInt = false;

    uint8_t  m_destROBTag = 0;
    bool     m_segFault = false;
    bool     m_memRequested = false;
    bool     m_cdbRequested = false;

    const char* idStr() const;
};