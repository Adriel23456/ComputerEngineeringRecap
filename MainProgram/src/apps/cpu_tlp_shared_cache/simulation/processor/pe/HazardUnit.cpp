// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/processor/pe/HazardUnit.cpp
// ============================================================================

/**
 * @file HazardUnit.cpp
 * @brief Implementation of HazardUnit.
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/HazardUnit.h"

namespace cpu_tlp {

    void HazardUnit::reset() {
        m_inMemoryOperation = false;
        m_ackSent = false;
        m_branchActive = false;
        m_branchCycles = 0;
        m_branchWaitingForW = false;
    }

    HazardOutputs HazardUnit::detect(
        bool INS_READY,
        bool C_REQUEST_M, bool C_READY,
        bool SegmentationFault,
        bool PCSrc_AND,
        bool PCSrc_W,
        uint8_t Rd_in_D, uint8_t Rn_in, uint8_t Rm_in,
        uint8_t Rd_in_E, uint8_t Rd_in_M, uint8_t Rd_in_W,
        bool RegWrite_E, bool RegWrite_M, bool RegWrite_W,
        bool BranchE,
        uint8_t /*BranchOp_E*/
    ) {
        HazardOutputs out{};

        // 1. Segmentation Fault - stall everything
        if (SegmentationFault) {
            out.StallF = out.StallD = out.StallE = out.StallM = out.StallW = true;
            return out;
        }

        // 2. Instruction latency
        if (!INS_READY) {
            out.StallF = true;
            out.StallD = true;
            return out;
        }

        // 3. Cache latency with handshake protocol
        bool memoryStall = false;

        if (C_REQUEST_M) {
            if (!m_inMemoryOperation) {
                m_inMemoryOperation = true;
                m_ackSent = false;
            }

            if (!m_ackSent) {
                if (C_READY) {
                    m_ackSent = true;
                    out.C_READY_ACK = true;
                    memoryStall = true;
                }
                else {
                    memoryStall = true;
                }
            }
            else {
                if (C_READY) {
                    out.C_READY_ACK = true;
                    memoryStall = true;
                }
                else {
                    out.C_READY_ACK = false;
                    m_inMemoryOperation = false;
                    m_ackSent = false;
                    memoryStall = false;
                }
            }
        }
        else {
            m_inMemoryOperation = false;
            m_ackSent = false;
            out.C_READY_ACK = false;
        }

        if (memoryStall) {
            out.StallF = out.StallD = out.StallE = out.StallM = out.StallW = true;
            return out;
        }

        // 4. RAW hazards
        bool rawHazard = false;

        if (Rn_in != 0) {
            if (RegWrite_E && Rd_in_E != 0 && Rd_in_E == Rn_in) {
                rawHazard = true;
            }
            if (RegWrite_M && Rd_in_M != 0 && Rd_in_M == Rn_in) {
                rawHazard = true;
            }
        }
        if (Rm_in != 0) {
            if (RegWrite_E && Rd_in_E != 0 && Rd_in_E == Rm_in) {
                rawHazard = true;
            }
            if (RegWrite_M && Rd_in_M != 0 && Rd_in_M == Rm_in) {
                rawHazard = true;
            }
        }

        if (rawHazard) {
            out.StallF = out.StallD = true;
            out.FlushE = true;
            return out;
        }

        // 5. Branch handling
        if (BranchE && !m_branchActive) {
            m_branchActive = true;
            m_branchCycles = 0;
            m_branchWaitingForW = false;
        }

        if (m_branchActive) {
            m_branchCycles++;

            if (m_branchCycles == 1) {
                out.StallF = true;
                out.FlushD = true;
            }
            else if (m_branchCycles == 2) {
                if (PCSrc_AND) {
                    out.StallF = true;
                    out.FlushD = true;
                    m_branchWaitingForW = true;
                }
                else {
                    m_branchActive = false;
                }
            }
            else if (m_branchWaitingForW) {
                if (PCSrc_W) {
                    out.FlushD = true;
                    m_branchWaitingForW = false;
                    m_branchActive = false;
                }
                else {
                    out.StallF = true;
                    out.FlushD = true;
                }
            }
            else {
                m_branchActive = false;
            }
        }

        return out;
    }

} // namespace cpu_tlp