// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.h
// ============================================================================

#pragma once

/**
 * @file SharedMemoryComponent.h
 * @brief Shared memory component with handshake-based access.
 *
 * Provides deterministic DRAM backend with rising-edge request
 * latching and handshake protocol for interconnect communication.
 *
 * @note Follows:
 *   - SRP: Only manages memory service thread
 *   - Encapsulation: Memory access through getMemory()
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
#include "apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.h"
#include <memory>
#include <thread>
#include <atomic>

namespace cpu_tlp {

    /**
     * @class SharedMemoryComponent
     * @brief DRAM backend with handshake protocol.
     *
     * Protocol:
     * - Latches request on rising edge of request_active
     * - Services request and publishes response_ready
     * - Waits for Interconnect to clear response_ready and request_active
     */
    class SharedMemoryComponent {
    public:
        SharedMemoryComponent();
        ~SharedMemoryComponent();

        // Prevent copying
        SharedMemoryComponent(const SharedMemoryComponent&) = delete;
        SharedMemoryComponent& operator=(const SharedMemoryComponent&) = delete;

        // ========================================================================
        // Lifecycle
        // ========================================================================

        /**
         * @brief Initializes component and starts service thread.
         * @param sharedData Shared data for inter-component communication.
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

        // ========================================================================
        // Memory Access
        // ========================================================================

        /**
         * @brief Gets mutable reference to shared memory.
         * @return Reference to SharedMemory instance.
         */
        SharedMemory& getMemory() { return m_memory; }

        /**
         * @brief Gets const reference to shared memory.
         * @return Const reference to SharedMemory instance.
         */
        const SharedMemory& getMemory() const { return m_memory; }

    private:
        /**
         * @brief Service thread main function.
         */
        void threadMain();

        std::shared_ptr<CPUSystemSharedData> m_sharedData;
        std::unique_ptr<std::thread> m_executionThread;

        SharedMemory m_memory;           ///< Underlying memory storage
        std::atomic<bool> m_isRunning;   ///< Thread running flag
    };

} // namespace cpu_tlp