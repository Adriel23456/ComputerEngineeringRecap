// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/pe/HazardUnit.h
// ============================================================================

#pragma once

/**
 * @file HazardUnit.h
 * @brief Hazard detection unit for PE pipeline.
 *
 * Detects and handles:
 * - Data hazards (RAW)
 * - Control hazards (branches)
 * - Structural hazards (memory latency)
 *
 * @note Follows:
 *   - SRP: Only detects hazards and generates stall/flush signals
 *   - Stateful: Maintains handshake and branch state
 */

#include <cstdint>

namespace cpu_tlp {

    /**
     * @struct HazardOutputs
     * @brief Hazard detection outputs.
     */
    struct HazardOutputs {
        bool StallF = false;      ///< Stall Fetch stage
        bool StallD = false;      ///< Stall Decode stage
        bool FlushD = false;      ///< Flush Decode stage
        bool StallE = false;      ///< Stall Execute stage
        bool FlushE = false;      ///< Flush Execute stage
        bool StallM = false;      ///< Stall Memory stage
        bool StallW = false;      ///< Stall WriteBack stage
        bool C_READY_ACK = false; ///< Cache ready acknowledgment
    };

    /**
     * @class HazardUnit
     * @brief Pipeline hazard detection and resolution.
     */
    class HazardUnit {
    public:
        /**
         * @brief Detects hazards and generates stall/flush signals.
         * @param INS_READY Instruction memory ready flag.
         * @param C_REQUEST_M Cache request in Memory stage.
         * @param C_READY Cache ready signal.
         * @param SegmentationFault Segmentation fault flag.
         * @param PCSrc_AND Branch taken in Execute.
         * @param PCSrc_W Branch taken in WriteBack.
         * @param Rd_in_D Destination register in Decode.
         * @param Rn_in Source register Rn in Decode.
         * @param Rm_in Source register Rm in Decode.
         * @param Rd_in_E Destination register in Execute.
         * @param Rd_in_M Destination register in Memory.
         * @param Rd_in_W Destination register in WriteBack.
         * @param RegWrite_E Register write enable in Execute.
         * @param RegWrite_M Register write enable in Memory.
         * @param RegWrite_W Register write enable in WriteBack.
         * @param BranchE Branch instruction in Execute.
         * @param BranchOp_E Branch operation type.
         * @return Hazard outputs structure.
         */
        HazardOutputs detect(
            bool INS_READY,
            bool C_REQUEST_M, bool C_READY,
            bool SegmentationFault,
            bool PCSrc_AND,
            bool PCSrc_W,
            uint8_t Rd_in_D, uint8_t Rn_in, uint8_t Rm_in,
            uint8_t Rd_in_E, uint8_t Rd_in_M, uint8_t Rd_in_W,
            bool RegWrite_E, bool RegWrite_M, bool RegWrite_W,
            bool BranchE,
            uint8_t BranchOp_E
        );

        /**
         * @brief Resets hazard unit state.
         */
        void reset();

    private:
        int m_branchCycles = 0;
        bool m_branchActive = false;
        bool m_branchWaitingForW = false;
        bool m_inMemoryOperation = false;
        bool m_ackSent = false;
    };

} // namespace cpu_tlp