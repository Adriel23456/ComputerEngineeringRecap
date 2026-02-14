#pragma once

/**
 * @file Control_Unit.h
 * @brief Component 8: Issue-stage control — decode, resource check, allocate.
 *
 * Determines instruction class, checks resource availability,
 * drives allocation signals for RS/Buffers/ROB, and controls
 * register renaming tag writes.
 *
 * @note SRP: Only handles issue-stage decode and allocation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class Control_Unit : public ITomasuloComponent {
public:
    const char* name() const override { return "Control_Unit"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    // Internal decode signals (not wires, just local to evaluate)
    struct DecodeResult {
        bool isIntALU = false;
        bool isIntMUL = false;
        bool isFPALU = false;
        bool isFPMUL = false;
        bool isLoad = false;
        bool isStore = false;
        bool isBranch = false;
        bool isNOP = false;
        bool isSWI = false;
        bool isCMPonly = false;
        bool isUnconditional = false;
        bool modifiesFlags = false;
        bool needsFlags = false;
        bool useImm = false;
        bool writesRd = false;
    };

    static DecodeResult decode(uint8_t op);
    static uint8_t allocTypeFromDecode(const DecodeResult& d);
};