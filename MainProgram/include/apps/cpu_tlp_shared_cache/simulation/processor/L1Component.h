// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/L1Component.h
// ============================================================================

#pragma once

/**
 * @file L1Component.h
 * @brief L1 cache component with bus interface.
 *
 * Wraps L1Cache with service thread that bridges CPU requests
 * to cache operations and handles bus communication.
 *
 * @note Follows:
 *   - SRP: Only manages cache threading and bus attachment
 *   - DIP: Uses L1Cache and Interconnect abstractions
 */

#include <thread>
#include <memory>
#include <atomic>
#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
#include "apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.h"

namespace cpu_tlp {

    /**
     * @class L1Component
     * @brief Service component for L1 cache.
     *
     * Provides:
     * - Thread management for cache ticking
     * - CPU request/response bridging
     * - Bus port attachment
     */
    class L1Component {
    public:
        /**
         * @brief Constructs component for specific PE.
         * @param id Cache/PE ID (0-3).
         */
        explicit L1Component(int id) : m_id(id) {}

        ~L1Component() { shutdown(); }

        // Prevent copying
        L1Component(const L1Component&) = delete;
        L1Component& operator=(const L1Component&) = delete;

        // ========================================================================
        // Lifecycle
        // ========================================================================

        /**
         * @brief Initializes component and starts service thread.
         * @param sharedData Shared data for CPU connection.
         * @param ic Interconnect (must be already initialized).
         * @return true if initialization succeeded.
         */
        bool initialize(std::shared_ptr<CPUSystemSharedData> sharedData, Interconnect* ic);

        /**
         * @brief Shuts down component and stops service thread.
         */
        void shutdown();

        // ========================================================================
        // Access
        // ========================================================================

        /**
         * @brief Gets L1 cache pointer.
         * @return Pointer to L1Cache.
         */
        L1Cache* l1() { return &m_l1; }

    private:
        /**
         * @brief Service thread main function.
         */
        void threadMain();

        /**
         * @brief Reads CPU request from shared data.
         */
        CpuReq readCpuReq();

        /**
         * @brief Writes CPU response to shared data.
         */
        void writeCpuResp(const CpuResp& resp);

        int m_id{ -1 };
        std::shared_ptr<CPUSystemSharedData> m_shared{};
        std::unique_ptr<std::thread> m_thread{};
        std::atomic<bool> m_running{ false };

        L1Cache m_l1;
        MasterToBus* m_portOut{ nullptr };
        BusToMaster* m_portIn{ nullptr };
    };

} // namespace cpu_tlp