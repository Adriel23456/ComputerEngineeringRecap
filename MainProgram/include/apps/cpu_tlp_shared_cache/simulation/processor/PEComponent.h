// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/PEComponent.h
// ============================================================================

#pragma once

/**
 * @file PEComponent.h
 * @brief Generic Processing Element component.
 *
 * Unified PE implementation that replaces PE0-PE3 components.
 * Each instance is differentiated only by PE ID.
 *
 * @note Follows:
 *   - SRP: Manages PE lifecycle and pipeline execution
 *   - DIP: Uses abstracted functional units
 *   - OCP: PE behavior extensible without modification
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/PipelineRegisters.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/RegisterFile.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/ALU.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/ControlUnit.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/HazardUnit.h"
#include <thread>
#include <memory>
#include <array>
#include <atomic>

namespace cpu_tlp {

    /**
     * @class PEComponent
     * @brief Generic processing element with 5-stage pipeline.
     *
     * Provides:
     * - Thread management for execution
     * - Pipeline stages: Fetch, Decode, Execute, Memory, WriteBack
     * - Hazard detection and resolution
     * - Shared data synchronization
     */
    class PEComponent {
    public:
        /**
         * @brief Constructs PE with specified ID.
         * @param pe_id PE identifier (0-3).
         */
        explicit PEComponent(int pe_id);
        ~PEComponent();

        // Prevent copying
        PEComponent(const PEComponent&) = delete;
        PEComponent& operator=(const PEComponent&) = delete;

        // ========================================================================
        // Lifecycle
        // ========================================================================

        /**
         * @brief Initializes PE and starts execution thread.
         * @param sharedData Shared data for inter-component communication.
         * @return true if initialization succeeded.
         */
        bool initialize(std::shared_ptr<CPUSystemSharedData> sharedData);

        /**
         * @brief Shuts down PE and stops execution thread.
         */
        void shutdown();

        /**
         * @brief Checks if PE is running.
         * @return true if execution thread is active.
         */
        bool isRunning() const;

        // ========================================================================
        // Control API
        // ========================================================================

        /**
         * @brief Executes single cycle.
         */
        void step();

        /**
         * @brief Executes specified number of cycles.
         * @param value Number of cycles.
         */
        void stepUntil(int value);

        /**
         * @brief Executes indefinitely until stopped.
         */
        void stepIndefinitely();

        /**
         * @brief Stops execution.
         */
        void stopExecution();

        /**
         * @brief Resets PE state.
         */
        void reset();

    private:
        // ========================================================================
        // Thread Management
        // ========================================================================

        void threadMain();
        void executeCycle();

        // ========================================================================
        // Pipeline Stages
        // ========================================================================

        void stageFetch();
        void stageDecode();
        void stageExecute();
        void stageMemory();
        void stageWriteBack();

        // ========================================================================
        // Helpers
        // ========================================================================

        uint64_t extendImmediate(uint32_t imm, bool dataType);
        bool evaluateBranchCondition(uint8_t branchOp, uint8_t flags);
        void updateInstructionTracking();

        inline void countRw_() {
            if (m_sharedData) {
                m_sharedData->analysis.traffic_pe[m_peId].fetch_add(1, std::memory_order_relaxed);
            }
        }

        // ========================================================================
        // Member Data
        // ========================================================================

        int m_peId;
        std::shared_ptr<CPUSystemSharedData> m_sharedData;
        std::unique_ptr<std::thread> m_executionThread;

        // CPU State
        uint64_t m_pcF = 0;
        uint64_t m_pcPlus8F = 0;

        // Pipeline registers
        FetchDecodeReg m_ifId{};
        FetchDecodeReg m_ifIdNext{};
        DecodeExecuteReg m_idEx{};
        DecodeExecuteReg m_idExNext{};
        ExecuteMemoryReg m_exMem{};
        ExecuteMemoryReg m_exMemNext{};
        MemoryWriteBackReg m_memWb{};
        MemoryWriteBackReg m_memWbNext{};

        // Decode stage signals
        uint64_t m_instrD = NOP_INSTRUCTION;
        uint64_t m_pcIn = 0;
        uint8_t m_opIn = 0;
        uint8_t m_rnIn = 0;
        uint8_t m_rmIn = 0;
        uint8_t m_rdInD = 0;
        uint32_t m_immIn = 0;
        uint64_t m_rdRnOut = 0;
        uint64_t m_rdRmOut = 0;
        uint64_t m_upperOut = 0;
        uint64_t m_lowerOut = 0;
        uint64_t m_immExtIn = 0;
        bool m_segmentationFault = false;
        uint64_t m_srcAD = 0;
        uint64_t m_srcBD = 0;
        ControlSignals m_ctrlSignals{};

        // Execute stage signals
        uint8_t m_aluFlagsOut = 0;
        uint64_t m_aluResultE = 0;
        uint8_t m_flagsPrime = 0;
        bool m_condExE = false;
        bool m_pcSrcAnd = false;

        // Memory stage signals
        uint64_t m_aluOutMO = 0;

        // WriteBack stage signals
        uint64_t m_pcPrime = 0;

        // Functional units
        RegisterFile m_registerFile;
        ALU m_alu;
        ControlUnit m_controlUnit;
        HazardUnit m_hazardUnit;
        HazardOutputs m_hazards{};

        // State flags
        std::atomic<bool> m_isRunning{ false };
        bool m_segFaultSticky = false;
        bool m_memReqCounted = false;
        bool m_prevAck = false;

        // Instruction tracking
        std::array<uint64_t, 5> m_stageInstructions{};
    };

} // namespace cpu_tlp