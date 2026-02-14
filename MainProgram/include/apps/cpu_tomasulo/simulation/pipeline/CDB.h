#pragma once

/**
 * @file CDB.h
 * @brief Components 37-38: CDB_A and CDB_B. Pure wire routing.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class CDB_A : public ITomasuloComponent {
public:
    const char* name() const override { return "CDB_A"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override {}
};

class CDB_B : public ITomasuloComponent {
public:
    const char* name() const override { return "CDB_B"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override {}
};