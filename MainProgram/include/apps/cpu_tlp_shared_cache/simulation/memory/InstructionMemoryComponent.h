// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/memory/InstructionMemoryComponent.h
// ============================================================================

#pragma once

/**
 * @file InstructionMemoryComponent.h
 * @brief Instruction memory component for CPU simulation.
 *
 * Manages the instruction memory file and serves fetch requests
 * from multiple processing elements asynchronously.
 *
 * @note Follows:
 *   - SRP: Only handles instruction memory access
 *   - Thread-safe: Runs in dedicated thread, uses atomic flags
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
#include <thread>
#include <memory>
#include <vector>
#include <string>
#include <atomic>
#include <array>

namespace cpu_tlp {

    /**
     * @class InstructionMemoryComponent
     * @brief Asynchronous instruction memory server.
     *
     * Loads instruction binary file and responds to PC requests
     * from all four processing elements in the system.
     */
    class InstructionMemoryComponent {
    public:
        InstructionMemoryComponent();
        ~InstructionMemoryComponent();

        // Prevent copying
        InstructionMemoryComponent(const InstructionMemoryComponent&) = delete;
        InstructionMemoryComponent& operator=(const InstructionMemoryComponent&) = delete;

        /**
         * @brief Initializes component and starts service thread.
         * @param sharedData Shared data structure for PE communication.
         * @return true if initialization succeeded.
         */
        bool initialize(std::shared_ptr<CPUSystemSharedData> sharedData);

        /**
         * @brief Shuts down component and stops service thread.
         */
        void shutdown();

        /**
         * @brief Checks if component is running.
         * @return true if service thread is active.
         */
        bool isRunning() const;

        /**
         * @brief Reloads instruction memory from file.
         *
         * Used after recompilation to pick up new instructions.
         *
         * @return true if reload succeeded.
         */
        bool reloadInstructionMemory();

        /**
         * @brief Pauses processing of fetch requests.
         */
        void pauseProcessing();

        /**
         * @brief Resumes processing of fetch requests.
         */
        void resumeProcessing();

    private:
        /**
         * @brief Main service thread function.
         */
        void threadMain();

        /**
         * @brief Reads instruction from memory at given address.
         * @param address Byte address in instruction memory.
         * @return 64-bit instruction word, or ERROR_INSTRUCTION on failure.
         */
        uint64_t readInstructionFromFile(uint64_t address);

        /**
         * @brief Loads instruction memory file into buffer.
         * @return true if load succeeded.
         */
        bool loadInstructionMemory();

        /**
         * @brief Processes fetch request from a specific PE.
         * @param peIndex Processing element index (0-3).
         */
        void processPERequest(int peIndex);

        /**
         * @brief Converts bytes to little-endian uint64.
         * @param bytes Pointer to 8 bytes.
         * @return Converted value.
         */
        uint64_t bytesToUint64LittleEndian(const uint8_t* bytes);

    private:
        std::shared_ptr<CPUSystemSharedData> m_sharedData;
        std::unique_ptr<std::thread> m_executionThread;

        std::vector<uint8_t> m_instructionMemory;   ///< Loaded instruction bytes
        size_t m_memorySize;                         ///< Size of loaded memory
        bool m_isRunning;                            ///< Component running flag
        std::atomic<bool> m_processingPaused;        ///< Pause flag for reload

        std::array<uint64_t, 4> m_lastPC;            ///< Last PC seen per PE

        /** @brief Error instruction returned on invalid access. */
        static constexpr uint64_t ERROR_INSTRUCTION = 0x4D00000000000000ULL;

        /** @brief Path to instruction memory file (relative to RESOURCES_PATH). */
        static constexpr const char* INSTRUCTION_FILE_PATH = "Assets/CPU_TLP/InstMem.bin";
    };

} // namespace cpu_tlp