// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/pe/PipelineRegisters.h
// ============================================================================

#pragma once

/**
 * @file PipelineRegisters.h
 * @brief Pipeline stage register structures for PE component.
 *
 * Defines flip-flop structures between pipeline stages:
 * IF/ID, ID/EX, EX/MEM, MEM/WB
 *
 * @note Follows:
 *   - SRP: Only defines data structures for pipeline registers
 *   - OCP: Structures can be extended without modification
 */

#include <cstdint>

namespace cpu_tlp {

    /// @brief NOP instruction encoding
    static constexpr uint64_t NOP_INSTRUCTION = 0x4D00000000000000ULL;

    /// @brief Flush instruction encoding (used during pipeline flush)
    static constexpr uint64_t FLUSH_INSTRUCTION = 0x4D00000000000001ULL;

    /**
     * @struct FetchDecodeReg
     * @brief IF/ID pipeline register.
     */
    struct FetchDecodeReg {
        uint64_t Instr_F = NOP_INSTRUCTION;  ///< Fetched instruction
        uint64_t PC_F = 0;                    ///< Program counter at fetch
    };

    /**
     * @struct DecodeExecuteReg
     * @brief ID/EX pipeline register.
     */
    struct DecodeExecuteReg {
        uint8_t RegWrite_D = 0;      ///< Register write enable
        uint8_t MemOp_D = 0;         ///< Memory operation flag
        uint8_t C_WE_D = 0;          ///< Cache write enable
        uint8_t C_REQUEST_D = 0;     ///< Cache request flag
        uint8_t C_ISB_D = 0;         ///< Instruction synchronization barrier
        uint8_t PCSrc_D = 0;         ///< PC source selector
        uint8_t FlagsUpd_D = 0;      ///< Flags update enable
        uint8_t ALUControl_D = 0;    ///< ALU operation control
        uint8_t BranchOp_D = 0;      ///< Branch operation type
        uint8_t Flags_in = 0;        ///< Input flags from Execute
        uint64_t SrcA_D = 0;         ///< ALU source A
        uint64_t SrcB_D = 0;         ///< ALU source B
        uint64_t RD_Rm_out = 0;      ///< Rm register value (for store)
        uint8_t Rd_in_D = 0;         ///< Destination register
        uint64_t Instr_D = NOP_INSTRUCTION;  ///< Instruction in Decode
    };

    /**
     * @struct ExecuteMemoryReg
     * @brief EX/MEM pipeline register.
     */
    struct ExecuteMemoryReg {
        uint8_t RegWrite_E = 0;      ///< Register write enable
        uint8_t MemOp_E = 0;         ///< Memory operation flag
        uint8_t C_WE_E = 0;          ///< Cache write enable
        uint8_t C_REQUEST_E = 0;     ///< Cache request flag
        uint8_t C_ISB_E = 0;         ///< Instruction synchronization barrier
        uint8_t PCSrc_AND = 0;       ///< Branch taken flag
        uint64_t ALUResult_E = 0;    ///< ALU result
        uint64_t RD_Rm_Special_E = 0;///< Store data value
        uint8_t Rd_in_E = 0;         ///< Destination register
        uint64_t Instr_E = NOP_INSTRUCTION;  ///< Instruction in Execute
    };

    /**
     * @struct MemoryWriteBackReg
     * @brief MEM/WB pipeline register.
     */
    struct MemoryWriteBackReg {
        uint8_t RegWrite_M = 0;      ///< Register write enable
        uint8_t PCSrc_M = 0;         ///< PC source (branch taken in WB)
        uint64_t ALUOutM_O = 0;      ///< Memory/ALU output
        uint8_t Rd_in_M = 0;         ///< Destination register
        uint64_t Instr_M = NOP_INSTRUCTION;  ///< Instruction in Memory
    };

} // namespace cpu_tlp