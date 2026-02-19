// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/agu/AGU.h
// ============================================================================

#pragma once

/**
 * @file AGU.h
 * @brief Base class for the Address Generation Units (Components 24–25).
 *
 * Computes effective address = Base + Offset, performs a segmentation
 * bounds check against UPPER_o / LOWER_o, and routes the result directly
 * to the requesting SB/LB slot via SourceID.
 *
 * Purely combinational — clockEdge() is a no-op.
 *
 * Segmentation check:
 *   segFault = (address < UPPER_o) || (address > LOWER_o)
 *   UPPER_o  = byte address one past the loaded program (code ceiling).
 *   LOWER_o  = memory ceiling (0xFFFF…FFFF at reset).
 *
 * SourceID mapping (set by AGU_Arbiter):
 *   0 = SB0, 1 = SB1, 2 = LB0, 3 = LB1, 4 = LB2
 *
 * Template Method pattern: AGU_0 and AGU_1 override the read/write
 * accessors; the shared evaluate() / routeResult() logic lives here.
 *
 * @note
 *   - SRP: Only computes and routes addresses; no operand scheduling.
 *   - OCP: Subclasses extend via virtual accessors without touching base logic.
 *   - LSP: AGU_0 / AGU_1 are interchangeable anywhere AGU* is expected.
 *   - DIP: Depends on TomasuloBus abstraction, not on concrete requestor state.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class AGU : public ITomasuloComponent {
public:
    enum class ID : uint8_t { AGU0 = 0, AGU1 = 1 };

    explicit AGU(ID id);

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Purely combinational — no state.
    void reset() override;

protected:
    // ── Per-instance bus read accessors ──────────────────────────
    virtual bool     readStart(const TomasuloBus& bus)    const = 0;
    virtual uint64_t readBase(const TomasuloBus& bus)     const = 0;
    virtual uint64_t readOffset(const TomasuloBus& bus)   const = 0;
    virtual uint8_t  readSourceID(const TomasuloBus& bus) const = 0;

    // ── Per-instance bus write accessors ─────────────────────────
    virtual void writeFree(TomasuloBus& bus, bool v)          const = 0;
    virtual void writeDone(TomasuloBus& bus, bool v)          const = 0;
    virtual void writeAddress(TomasuloBus& bus, uint64_t v)   const = 0;
    virtual void writeSegFault(TomasuloBus& bus, bool v)      const = 0;
    virtual void writeSourceIDOut(TomasuloBus& bus, uint8_t v)const = 0;

    ID m_id;

private:
    /**
     * @brief Writes the computed address and segFault to the correct SB/LB
     *        AGUDone signals based on the SourceID returned by the arbiter.
     */
    void routeResult(TomasuloBus& bus, uint8_t sourceID,
        uint64_t address, bool segFault) const;

    const char* idStr() const;
};