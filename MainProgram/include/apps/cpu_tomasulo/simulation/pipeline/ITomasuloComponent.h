#pragma once

/**
 * @file ITomasuloComponent.h
 * @brief Interface for all Tomasulo pipeline components.
 *
 * @note
 *   - ISP: Minimal interface — evaluate, clockEdge, reset, name.
 *   - OCP: New components implement this without changing existing ones.
 */

#include <string>

struct TomasuloBus;

class ITomasuloComponent {
public:
    virtual ~ITomasuloComponent() = default;

    /** @brief Component display name (for debug prints). */
    virtual const char* name() const = 0;

    /**
     * @brief Evaluate combinational outputs based on current bus state.
     * Called one or more times per cycle until signals settle.
     */
    virtual void evaluate(TomasuloBus& bus) = 0;

    /**
     * @brief Latch sequential state on the rising clock edge.
     * Called exactly once per cycle AFTER all evaluations.
     */
    virtual void clockEdge(TomasuloBus& bus) = 0;

    /**
     * @brief Reset internal state to power-on defaults.
     */
    virtual void reset() = 0;
};