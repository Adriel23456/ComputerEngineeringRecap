// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/widgets/InstructionDisassembler.h
// ============================================================================

#pragma once

/**
 * @file InstructionDisassembler.h
 * @brief Instruction disassembly utility.
 *
 * Converts 64-bit encoded instructions back to human-readable
 * assembly mnemonics for UI display.
 *
 * @note Follows:
 *   - SRP: Only handles instruction decoding
 *   - Stateless: All methods are static
 */

#include <string>
#include <cstdint>
#include <cstring>

namespace cpu_tlp {

    /**
     * @class InstructionDisassembler
     * @brief Static utility for instruction disassembly.
     */
    class InstructionDisassembler {
    public:
        /**
         * @brief Disassembles 64-bit instruction to string.
         * @param instruction Encoded instruction.
         * @return Human-readable assembly string.
         */
        static std::string disassemble(uint64_t instruction);

    private:
        /**
         * @brief Gets mnemonic for opcode.
         * @param opcode 8-bit opcode value.
         * @return Mnemonic string.
         */
        static const char* getOpcodeMnemonic(uint8_t opcode);

        /**
         * @brief Gets register name for code.
         * @param regCode 4-bit register code.
         * @return Register name string.
         */
        static std::string getRegisterName(uint8_t regCode);

        /**
         * @brief Formats immediate value.
         * @param imm 32-bit immediate.
         * @param isFloat true if float format.
         * @return Formatted string with # prefix.
         */
        static std::string formatImmediate(uint32_t imm, bool isFloat);

        /**
         * @brief Sign-extends 32-bit value.
         * @param value Unsigned value.
         * @return Sign-extended signed value.
         */
        static int32_t signExtend32(uint32_t value);
    };

} // namespace cpu_tlp