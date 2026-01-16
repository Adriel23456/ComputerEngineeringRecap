// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/processor/PEComponent.cpp
// ============================================================================

/**
 * @file PEComponent.cpp
 * @brief Implementation of PEComponent.
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/PEComponent.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/InstructionDisassembler.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/Log.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <cstring>

namespace cpu_tlp {

    // ========================================================================
    // Constructor / Destructor
    // ========================================================================

    PEComponent::PEComponent(int pe_id)
        : m_peId(pe_id)
    {
        reset();
    }

    PEComponent::~PEComponent() {
        shutdown();
    }

    // ========================================================================
    // Lifecycle
    // ========================================================================

    bool PEComponent::initialize(std::shared_ptr<CPUSystemSharedData> sharedData) {
        if (m_isRunning.load()) {
            return false;
        }

        m_sharedData = std::move(sharedData);
        if (!m_sharedData) {
            return false;
        }

        // Configure PE ID in register file
        m_registerFile.setPEID(m_peId);

        // Connect register file to shared snapshot
        m_registerFile.onRegisterWrite = [this](uint8_t addr, uint64_t value) {
            if (addr < 12) {
                m_sharedData->pe_registers[m_peId].registers[addr].store(
                    value, std::memory_order_release);
            }
            };

        // Initial synchronization of register snapshot
        for (int i = 0; i < 12; ++i) {
            uint64_t val = m_registerFile.read(i);
            m_sharedData->pe_registers[m_peId].registers[i].store(
                val, std::memory_order_release);
        }

        // Reset control signals
        auto& ctrl = m_sharedData->pe_control[m_peId];
        ctrl.command.store(0, std::memory_order_release);
        ctrl.step_count.store(0, std::memory_order_release);
        ctrl.running.store(false, std::memory_order_release);
        ctrl.should_stop.store(false, std::memory_order_release);

        m_isRunning.store(true, std::memory_order_release);
        m_sharedData->system_should_stop.store(false, std::memory_order_release);

        // Start execution thread
        m_executionThread = std::make_unique<std::thread>(&PEComponent::threadMain, this);

        std::cout << "[PE" << m_peId << "] Initialized successfully\n";
        return true;
    }

    void PEComponent::shutdown() {
        if (!m_isRunning.load()) return;

        std::cout << "[PE" << m_peId << "] Shutting down...\n";

        m_sharedData->system_should_stop.store(true, std::memory_order_release);
        auto& ctrl = m_sharedData->pe_control[m_peId];
        ctrl.should_stop.store(true, std::memory_order_release);

        if (m_executionThread && m_executionThread->joinable()) {
            m_executionThread->join();
        }
        m_executionThread.reset();
        m_isRunning.store(false, std::memory_order_release);

        std::cout << "[PE" << m_peId << "] Shutdown complete\n";
    }

    bool PEComponent::isRunning() const {
        return m_isRunning.load(std::memory_order_acquire);
    }

    void PEComponent::reset() {
        // Reset pipeline state
        m_pcF = 0x0;
        m_pcPlus8F = 0x0;

        m_ifId = {};
        m_ifIdNext = {};
        m_ifId.Instr_F = NOP_INSTRUCTION;

        m_idEx = {};
        m_idExNext = {};
        m_idEx.ALUControl_D = 0x22;
        m_idEx.Instr_D = NOP_INSTRUCTION;

        m_exMem = {};
        m_exMemNext = {};
        m_exMem.Instr_E = NOP_INSTRUCTION;

        m_memWb = {};
        m_memWbNext = {};
        m_memWb.Instr_M = NOP_INSTRUCTION;

        m_instrD = NOP_INSTRUCTION;
        m_pcIn = 0x0;
        m_segmentationFault = false;
        m_segFaultSticky = false;

        // Reset register file
        m_registerFile.reset();
        m_registerFile.setPEID(m_peId);

        // Synchronize register snapshot after reset
        if (m_sharedData) {
            for (int i = 0; i < 12; ++i) {
                uint64_t val = m_registerFile.read(i);
                m_sharedData->pe_registers[m_peId].registers[i].store(
                    val, std::memory_order_release);
            }
        }

        m_hazards = {};
        m_hazardUnit.reset();

        // Reset traffic counter state
        m_memReqCounted = false;
        m_prevAck = false;

        // Reset instruction tracking
        m_stageInstructions.fill(NOP_INSTRUCTION);

        if (m_sharedData) {
            for (int i = 0; i < 5; ++i) {
                m_sharedData->pe_instruction_tracking[m_peId].stage_instructions[i].store(
                    NOP_INSTRUCTION, std::memory_order_release);
            }

            // Reset shared signals
            auto& instConn = m_sharedData->instruction_connections[m_peId];
            instConn.PC_F.store(0x0, std::memory_order_release);

            auto& cacheConn = m_sharedData->cache_connections[m_peId];
            cacheConn.ALUOut_M.store(0x0, std::memory_order_release);
            cacheConn.RD_Rm_Special_M.store(0x0, std::memory_order_release);
            cacheConn.C_WE_M.store(false, std::memory_order_release);
            cacheConn.C_ISB_M.store(false, std::memory_order_release);
            cacheConn.C_REQUEST_M.store(false, std::memory_order_release);
        }

        std::cout << "[PE" << m_peId << "] Reset complete\n";
    }

    // ========================================================================
    // Control API
    // ========================================================================

    void PEComponent::step() {
        if (!m_sharedData) return;
        auto& ctrl = m_sharedData->pe_control[m_peId];
        ctrl.command.store(1, std::memory_order_release);
        ctrl.running.store(true, std::memory_order_release);
    }

    void PEComponent::stepUntil(int value) {
        if (!m_sharedData) return;
        if (value <= 0) value = 1;
        auto& ctrl = m_sharedData->pe_control[m_peId];
        ctrl.step_count.store(value, std::memory_order_release);
        ctrl.command.store(2, std::memory_order_release);
        ctrl.running.store(true, std::memory_order_release);
    }

    void PEComponent::stepIndefinitely() {
        if (!m_sharedData) return;
        auto& ctrl = m_sharedData->pe_control[m_peId];
        ctrl.should_stop.store(false, std::memory_order_release);
        ctrl.command.store(3, std::memory_order_release);
        ctrl.running.store(true, std::memory_order_release);
    }

    void PEComponent::stopExecution() {
        if (!m_sharedData) return;
        auto& ctrl = m_sharedData->pe_control[m_peId];
        ctrl.should_stop.store(true, std::memory_order_release);
    }

    // ========================================================================
    // Thread Management
    // ========================================================================

    void PEComponent::threadMain() {
        using namespace std::chrono_literals;

        std::cout << "[PE" << m_peId << "] Thread started\n";

        while (!m_sharedData->system_should_stop.load(std::memory_order_acquire)) {
            auto& ctrl = m_sharedData->pe_control[m_peId];
            int cmd = ctrl.command.load(std::memory_order_acquire);

            switch (cmd) {
            case 0:  // Idle
                ctrl.running.store(false, std::memory_order_release);
                std::this_thread::sleep_for(3ms);
                break;

            case 1:  // Step
                executeCycle();
                ctrl.command.store(0, std::memory_order_release);
                ctrl.running.store(false, std::memory_order_release);
                break;

            case 2: {  // Step until
                int left = ctrl.step_count.load(std::memory_order_acquire);
                if (left <= 0) {
                    ctrl.command.store(0, std::memory_order_release);
                    ctrl.running.store(false, std::memory_order_release);
                    break;
                }
                executeCycle();
                ctrl.step_count.store(left - 1, std::memory_order_release);
                if (left - 1 <= 0 || ctrl.should_stop.load(std::memory_order_acquire)) {
                    ctrl.command.store(0, std::memory_order_release);
                    ctrl.running.store(false, std::memory_order_release);
                    ctrl.should_stop.store(false, std::memory_order_release);
                }
                break;
            }

            case 3: {  // Step indefinitely
                if (ctrl.should_stop.load(std::memory_order_acquire)) {
                    ctrl.command.store(0, std::memory_order_release);
                    ctrl.running.store(false, std::memory_order_release);
                    ctrl.should_stop.store(false, std::memory_order_release);
                    break;
                }
                executeCycle();
                std::this_thread::sleep_for(250us);
                break;
            }

            case 4:  // Reset
                std::cout << "[PE" << m_peId << "] Executing RESET\n";
                reset();
                ctrl.command.store(0, std::memory_order_release);
                ctrl.running.store(false, std::memory_order_release);
                break;

            default:
                std::cerr << "[PE" << m_peId << "] Unknown command: " << cmd << "\n";
                ctrl.command.store(0, std::memory_order_release);
                ctrl.running.store(false, std::memory_order_release);
                break;
            }
        }

        std::cout << "[PE" << m_peId << "] Thread ending\n";
    }

    void PEComponent::executeCycle() {
        auto& instConn = m_sharedData->instruction_connections[m_peId];

        m_pcPlus8F = m_pcF + 8;

        stageWriteBack();
        stageMemory();
        stageExecute();
        stageDecode();
        stageFetch();

        updateInstructionTracking();

        // Write C_READY_ACK to shared connection
        auto& cacheConn = m_sharedData->cache_connections[m_peId];
        cacheConn.C_READY_ACK.store(m_hazards.C_READY_ACK, std::memory_order_release);

        // Traffic counter (count once per memory operation)
        if (m_exMem.C_REQUEST_E && !m_memReqCounted) {
            countRw_();
            m_memReqCounted = true;
        }
        if (m_prevAck && !m_hazards.C_READY_ACK) {
            m_memReqCounted = false;
        }
        m_prevAck = m_hazards.C_READY_ACK;

        // Update flip-flops
        if (!m_hazards.StallW) {
            m_memWb = m_memWbNext;
        }

        if (!m_hazards.StallM) {
            m_exMem = m_exMemNext;
        }

        if (!m_hazards.StallE) {
            if (m_hazards.FlushE) {
                uint8_t preserved_flags = m_idEx.Flags_in;
                m_idEx = {};
                m_idEx.ALUControl_D = 0x22;
                m_idEx.FlagsUpd_D = 0;
                m_idEx.Instr_D = FLUSH_INSTRUCTION;
                m_idEx.Flags_in = preserved_flags;
            }
            else {
                m_idEx = m_idExNext;
            }
        }

        if (!m_hazards.StallD) {
            if (m_hazards.FlushD) {
                m_ifId.Instr_F = FLUSH_INSTRUCTION;
            }
            else {
                m_ifId = m_ifIdNext;
            }
        }

        uint64_t old_pc = m_pcF;
        if (!m_hazards.StallF) {
            m_pcF = m_pcPrime;

            if (m_pcF != old_pc) {
                instConn.INS_READY.store(false, std::memory_order_release);
            }
        }
        instConn.PC_F.store(m_pcF, std::memory_order_release);

        std::this_thread::sleep_for(std::chrono::microseconds(850));
    }

    // ========================================================================
    // Pipeline Stages
    // ========================================================================

    void PEComponent::stageFetch() {
        auto& instConn = m_sharedData->instruction_connections[m_peId];

        bool ins_ready = instConn.INS_READY.load(std::memory_order_acquire);
        uint64_t instr = instConn.InstrF.load(std::memory_order_acquire);

        if (ins_ready) {
            m_ifIdNext.Instr_F = instr;
            m_ifIdNext.PC_F = m_pcF;
        }
        else {
            m_ifIdNext = m_ifId;
        }
    }

    void PEComponent::stageDecode() {
        m_instrD = m_ifId.Instr_F;
        m_pcIn = m_ifId.PC_F;

        // Decompose instruction
        m_opIn = (m_instrD >> 56) & 0xFF;
        m_rdInD = (m_instrD >> 52) & 0xF;
        m_rnIn = (m_instrD >> 48) & 0xF;
        m_rmIn = (m_instrD >> 44) & 0xF;
        m_immIn = (m_instrD >> 12) & 0xFFFFFFFF;

        // Control Unit
        m_ctrlSignals = m_controlUnit.decode(m_opIn);

        // INC/DEC: unary operations that read Rd
        if (m_opIn == 0x1C || m_opIn == 0x1D) {
            m_rnIn = m_rdInD;
            m_rmIn = m_rdInD;
        }

        // SWI: stop execution
        if (m_opIn == 0x4C) {
            auto& ctrl = m_sharedData->pe_control[m_peId];
            ctrl.should_stop.store(true, std::memory_order_release);
            ctrl.command.store(0, std::memory_order_release);
            ctrl.running.store(false, std::memory_order_release);
            m_sharedData->ui_signals[m_peId].swi_count.fetch_add(1, std::memory_order_acq_rel);
        }

        // Register file read
        m_rdRnOut = m_registerFile.read(m_rnIn);
        m_rdRmOut = m_registerFile.read(m_rmIn);
        m_upperOut = m_registerFile.getUpper();
        m_lowerOut = m_registerFile.getLower();

        // Segmentation fault check
        m_segmentationFault = false;
        if (m_pcIn < m_upperOut || m_pcIn > m_lowerOut) {
            m_segmentationFault = true;
            m_segFaultSticky = true;
            log_build_and_print([&](std::ostringstream& oss) {
                oss << "[PE" << m_peId << "] SEGMENTATION FAULT at PC=0x"
                    << std::hex << m_pcIn << std::dec << "\n";
                });
        }

        // Extend immediate
        m_immExtIn = extendImmediate(m_immIn, m_ctrlSignals.DataType);

        // Mux for branching
        m_srcAD = m_ctrlSignals.BranchE ? m_pcIn : m_rdRnOut;

        // Mux for immediate
        m_srcBD = m_ctrlSignals.ImmOp ? m_immExtIn : m_rdRmOut;

        // Hazard detection
        auto& instConn = m_sharedData->instruction_connections[m_peId];
        bool ins_ready = instConn.INS_READY.load(std::memory_order_acquire);

        auto& cacheConn = m_sharedData->cache_connections[m_peId];
        bool c_request = m_exMem.C_REQUEST_E;
        bool c_ready = cacheConn.C_READY.load(std::memory_order_acquire);

        m_hazards = m_hazardUnit.detect(
            ins_ready, c_request, c_ready,
            m_segFaultSticky,
            m_pcSrcAnd,
            m_memWb.PCSrc_M,
            m_rdInD, m_rnIn, m_rmIn,
            m_idEx.Rd_in_D, m_exMem.Rd_in_E, m_memWb.Rd_in_M,
            m_idEx.RegWrite_D, m_exMem.RegWrite_E, m_memWb.RegWrite_M,
            m_ctrlSignals.BranchE,
            m_idEx.BranchOp_D
        );

        // Prepare next flip-flop
        uint8_t rd = m_rdInD;
        bool regWriteAllowed = m_ctrlSignals.RegWrite_D && (rd != REG_ZERO) && (rd != REG_PEID);

        m_idExNext.RegWrite_D = regWriteAllowed;
        m_idExNext.MemOp_D = m_ctrlSignals.MemOp_D;
        m_idExNext.C_WE_D = m_ctrlSignals.C_WE_D;
        m_idExNext.C_REQUEST_D = m_ctrlSignals.C_REQUEST_D;
        m_idExNext.C_ISB_D = m_ctrlSignals.C_ISB_D;
        m_idExNext.PCSrc_D = m_ctrlSignals.PCSrc_D;
        m_idExNext.FlagsUpd_D = m_ctrlSignals.FlagsUpd_D;
        m_idExNext.ALUControl_D = m_ctrlSignals.ALUControl_D;
        m_idExNext.BranchOp_D = m_ctrlSignals.BranchOp_D;
        m_idExNext.Flags_in = m_flagsPrime;
        m_idExNext.SrcA_D = m_srcAD;
        m_idExNext.SrcB_D = m_srcBD;
        m_idExNext.RD_Rm_out = m_rdRmOut;
        m_idExNext.Rd_in_D = rd;
        m_idExNext.Instr_D = m_instrD;
    }

    void PEComponent::stageExecute() {
        uint8_t flags_e = m_idEx.Flags_in;
        uint64_t srcA = m_idEx.SrcA_D;
        uint64_t srcB = m_idEx.SrcB_D;
        uint8_t aluCtrl = m_idEx.ALUControl_D;

        // ALU execution
        ALUResult aluRes = m_alu.execute(aluCtrl, srcA, srcB, flags_e);
        m_aluResultE = aluRes.value;
        m_aluFlagsOut = aluRes.flags;

        // Condition evaluation
        if (m_idEx.FlagsUpd_D) {
            m_flagsPrime = m_aluFlagsOut;
        }
        else {
            m_flagsPrime = flags_e;
        }

        m_condExE = evaluateBranchCondition(m_idEx.BranchOp_D, m_flagsPrime);

        // Branch AND
        m_pcSrcAnd = m_idEx.PCSrc_D && m_condExE;

        // Prepare next flip-flop
        m_exMemNext.RegWrite_E = m_idEx.RegWrite_D;
        m_exMemNext.MemOp_E = m_idEx.MemOp_D;
        m_exMemNext.C_WE_E = m_idEx.C_WE_D;
        m_exMemNext.C_REQUEST_E = m_idEx.C_REQUEST_D;
        m_exMemNext.C_ISB_E = m_idEx.C_ISB_D;
        m_exMemNext.PCSrc_AND = m_pcSrcAnd;
        m_exMemNext.ALUResult_E = m_aluResultE;
        m_exMemNext.RD_Rm_Special_E = m_idEx.RD_Rm_out;
        m_exMemNext.Rd_in_E = m_idEx.Rd_in_D;
        m_exMemNext.Instr_E = m_idEx.Instr_D;
    }

    void PEComponent::stageMemory() {
        uint64_t aluOut = m_exMem.ALUResult_E;
        uint64_t rdRm = m_exMem.RD_Rm_Special_E;

        // Write signals to cache
        auto& cacheConn = m_sharedData->cache_connections[m_peId];
        cacheConn.ALUOut_M.store(aluOut, std::memory_order_release);
        cacheConn.RD_Rm_Special_M.store(rdRm, std::memory_order_release);
        cacheConn.C_WE_M.store(m_exMem.C_WE_E, std::memory_order_release);
        cacheConn.C_ISB_M.store(m_exMem.C_ISB_E, std::memory_order_release);
        cacheConn.C_REQUEST_M.store(m_exMem.C_REQUEST_E, std::memory_order_release);

        // Read cache response
        uint64_t rdCout = cacheConn.RD_C_out.load(std::memory_order_acquire);

        // Memory mux
        m_aluOutMO = m_exMem.MemOp_E ? rdCout : aluOut;

        // Prepare next flip-flop
        m_memWbNext.RegWrite_M = m_exMem.RegWrite_E;
        m_memWbNext.PCSrc_M = m_exMem.PCSrc_AND;
        m_memWbNext.ALUOutM_O = m_aluOutMO;
        m_memWbNext.Rd_in_M = m_exMem.Rd_in_E;
        m_memWbNext.Instr_M = m_exMem.Instr_E;
    }

    void PEComponent::stageWriteBack() {
        uint8_t regWrite = m_memWb.RegWrite_M;
        uint8_t pcsrc = m_memWb.PCSrc_M;
        uint64_t aluOutW = m_memWb.ALUOutM_O;
        uint8_t rdInW = m_memWb.Rd_in_M;

        // Register file write
        m_registerFile.write(rdInW, aluOutW, regWrite);

        // PC mux
        m_pcPrime = pcsrc ? aluOutW : m_pcPlus8F;
    }

    // ========================================================================
    // Helpers
    // ========================================================================

    uint64_t PEComponent::extendImmediate(uint32_t imm, bool dataType) {
        if (dataType == 0) {
            // Sign extension from 32-bit to 64-bit
            if (imm & 0x80000000) {
                return 0xFFFFFFFF00000000ULL | imm;
            }
            else {
                return imm;
            }
        }
        else {
            // Float 32-bit to double 64-bit conversion
            float f;
            std::memcpy(&f, &imm, 4);
            double d = static_cast<double>(f);
            uint64_t result;
            std::memcpy(&result, &d, 8);
            return result;
        }
    }

    bool PEComponent::evaluateBranchCondition(uint8_t branchOp, uint8_t flags) {
        bool N = (flags & 0x8) != 0;
        bool Z = (flags & 0x4) != 0;
        bool C = (flags & 0x2) != 0;
        bool V = (flags & 0x1) != 0;

        switch (branchOp) {
        case 0x0: return false;           // No branch
        case 0x1: return true;            // B (always)
        case 0x2: return Z;               // BEQ
        case 0x3: return !Z;              // BNE
        case 0x4: return N != V;          // BLT
        case 0x5: return !Z && (N == V);  // BGT
        case 0x6: return V;               // BUN (unordered)
        case 0x7: return !V;              // BORD (ordered)
        default: return false;
        }
    }

    void PEComponent::updateInstructionTracking() {
        std::array<uint64_t, 5> snap;

        snap[0] = m_ifIdNext.Instr_F;
        snap[1] = m_ifId.Instr_F;
        snap[2] = m_idEx.Instr_D;
        snap[3] = m_exMem.Instr_E;
        snap[4] = m_memWb.Instr_M;

        m_stageInstructions = snap;

        if (m_sharedData) {
            auto& track = m_sharedData->pe_instruction_tracking[m_peId];
            track.version.fetch_add(1, std::memory_order_acq_rel);
            for (int i = 0; i < 5; ++i) {
                track.stage_instructions[i].store(snap[i], std::memory_order_release);
            }
            track.version.fetch_add(1, std::memory_order_acq_rel);
        }
    }

} // namespace cpu_tlp