// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/pe/ALU.h
// ============================================================================

#pragma once

/**
 * @file ALU.h
 * @brief Arithmetic Logic Unit for PE component.
 *
 * Supports integer and floating-point operations with flag computation.
 *
 * @note Follows:
 *   - SRP: Only performs arithmetic/logic operations
 *   - Stateless: No internal state, pure function
 */

#include <cstdint>

namespace cpu_tlp {

    /**
     * @struct ALUResult
     * @brief Result of ALU operation.
     */
    struct ALUResult {
        uint64_t value;  ///< Computed value
        uint8_t flags;   ///< NZCV flags
    };

    /**
     * @class ALU
     * @brief Stateless ALU with integer and floating-point operations.
     */
    class ALU {
    public:
        /**
         * @brief Executes ALU operation.
         * @param control Operation code (0x00-0x22).
         * @param A First operand.
         * @param B Second operand.
         * @param flagsIn Input flags (for carry operations).
         * @return Result with value and flags.
         */
        ALUResult execute(uint8_t control, uint64_t A, uint64_t B, uint8_t flagsIn) const;
    };

} // namespace cpu_tlp