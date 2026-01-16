// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h
// ============================================================================

#pragma once

/**
 * @file SharedData.h
 * @brief Shared data structures for multi-threaded CPU simulation.
 *
 * Defines atomic structures for communication between processing
 * elements, caches, memory, and UI components.
 *
 * @note Follows:
 *   - SRP: Each struct has single communication purpose
 *   - Thread-safe: All fields use std::atomic
 */

#include <atomic>
#include <cstdint>

namespace cpu_tlp {

    // ============================================================================
    // Analysis Counters
    // ============================================================================

    /**
     * @struct AnalysisCounters
     * @brief Atomic counters for simulation analysis.
     */
    struct AnalysisCounters {
        std::atomic<uint64_t> traffic_pe[4];         ///< Per-PE traffic counts
        std::atomic<uint64_t> cache_misses{ 0 };     ///< Total cache misses
        std::atomic<uint64_t> invalidations{ 0 };    ///< Line invalidations
        std::atomic<uint64_t> transactions_mesi{ 0 }; ///< MESI transactions

        /**
         * @brief Resets all counters to zero.
         */
        void reset() {
            for (auto& a : traffic_pe) {
                a.store(0, std::memory_order_relaxed);
            }
            cache_misses.store(0, std::memory_order_relaxed);
            invalidations.store(0, std::memory_order_relaxed);
            transactions_mesi.store(0, std::memory_order_relaxed);
        }
    };

    // ============================================================================
    // Instruction Memory Connection
    // ============================================================================

    /**
     * @struct InstructionConnection
     * @brief PE <-> Instruction Memory interface.
     */
    struct InstructionConnection {
        std::atomic<uint64_t> PC_F{ 0 };         ///< Program counter (fetch stage)
        std::atomic<uint64_t> InstrF{ 0 };       ///< Fetched instruction
        std::atomic<bool> INS_READY{ false };    ///< Instruction ready flag
    };

    // ============================================================================
    // Cache Connection
    // ============================================================================

    /**
     * @struct CacheConnection
     * @brief PE <-> L1 Cache interface.
     */
    struct CacheConnection {
        std::atomic<uint64_t> ALUOut_M{ 0 };         ///< Memory address
        std::atomic<uint64_t> RD_Rm_Special_M{ 0 };  ///< Write data
        std::atomic<bool> C_WE_M{ false };           ///< Write enable
        std::atomic<bool> C_ISB_M{ false };          ///< Byte access mode
        std::atomic<bool> C_REQUEST_M{ false };      ///< Request active
        std::atomic<uint64_t> RD_C_out{ 0 };         ///< Read data output
        std::atomic<bool> C_READY{ false };          ///< Operation complete
        std::atomic<bool> C_READY_ACK{ false };      ///< CPU acknowledged
    };

    // ============================================================================
    // RAM Connection
    // ============================================================================

    /**
     * @struct RAMConnection
     * @brief Interconnect <-> Shared Memory interface.
     */
    struct RAMConnection {
        std::atomic<uint16_t> request_address{ 0 };  ///< Memory address
        std::atomic<uint64_t> write_data{ 0 };       ///< Write data
        std::atomic<bool> write_enable{ false };     ///< Write mode
        std::atomic<bool> request_active{ false };   ///< Request active
        std::atomic<uint64_t> read_data{ 0 };        ///< Read data output
        std::atomic<bool> response_ready{ false };   ///< Response available
    };

    // ============================================================================
    // PE Control
    // ============================================================================

    /**
     * @struct PEControl
     * @brief Processing element execution control.
     */
    struct PEControl {
        std::atomic<int> command{ 0 };       ///< Command code
        std::atomic<int> step_count{ 0 };    ///< Steps to execute
        std::atomic<bool> running{ false };  ///< Execution active
        std::atomic<bool> should_stop{ false }; ///< Stop request
    };

    // ============================================================================
    // PE Registers Snapshot
    // ============================================================================

    /**
     * @struct PERegistersSnapshot
     * @brief Atomic snapshot of PE register file for UI display.
     */
    struct PERegistersSnapshot {
        std::atomic<uint64_t> registers[12];

        PERegistersSnapshot() {
            for (int i = 0; i < 12; ++i) {
                registers[i].store(0, std::memory_order_relaxed);
            }
        }
    };

    // ============================================================================
    // PE Instruction Tracking
    // ============================================================================

    /**
     * @struct PEInstructionTracking
     * @brief Pipeline stage tracking using seqlock pattern.
     *
     * Version number is incremented before and after writes.
     * Readers retry if version is odd (write in progress) or changes.
     */
    struct PEInstructionTracking {
        std::atomic<uint64_t> version{ 0 };              ///< Seqlock version
        std::atomic<uint64_t> stage_instructions[5];     ///< Pipeline stages

        PEInstructionTracking() {
            version.store(0, std::memory_order_relaxed);
            for (int i = 0; i < 5; ++i) {
                stage_instructions[i].store(0, std::memory_order_relaxed);
            }
        }
    };

    // ============================================================================
    // UI Signals
    // ============================================================================

    /**
     * @struct UISignals
     * @brief Signals from PE to UI.
     */
    struct UISignals {
        std::atomic<uint32_t> swi_count{ 0 }; ///< Software interrupt counter
    };

    // ============================================================================
    // Main Shared Data Structure
    // ============================================================================

    /**
     * @struct CPUSystemSharedData
     * @brief Central shared data hub for all simulation components.
     *
     * Provides atomic communication channels between:
     * - Processing elements (PE0-PE3)
     * - L1 caches
     * - Instruction memory
     * - Shared RAM
     * - UI components
     */
    struct CPUSystemSharedData {
        std::atomic<bool> system_should_stop{ false }; ///< Global shutdown flag

        InstructionConnection instruction_connections[4]; ///< PE instruction fetches
        CacheConnection cache_connections[4];             ///< PE cache accesses
        RAMConnection ram_connection;                     ///< Memory requests

        PEControl pe_control[4];                    ///< PE execution control
        PERegistersSnapshot pe_registers[4];        ///< Register snapshots for UI
        PEInstructionTracking pe_instruction_tracking[4]; ///< Pipeline tracking
        UISignals ui_signals[4];                    ///< UI event signals

        AnalysisCounters analysis;                  ///< Simulation metrics
    };

} // namespace cpu_tlp