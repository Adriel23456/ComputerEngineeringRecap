// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.h
// ============================================================================

#pragma once

/**
 * @file InterconnectComponent.h
 * @brief Component wrapper for interconnect bus.
 *
 * Manages the bus interconnect lifecycle and provides a service thread
 * that continuously ticks the bus FSM.
 *
 * @note Follows:
 *   - SRP: Only manages bus lifecycle and threading
 *   - DIP: Uses Interconnect abstraction for bus operations
 */

#include <thread>
#include <memory>
#include <functional>
#include <atomic>
#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
#include "apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.h"

namespace cpu_tlp {

    /**
     * @class InterconnectComponent
     * @brief Service component for bus interconnect.
     *
     * Provides:
     * - Thread management for bus ticking
     * - Port access for L1 cache connections
     * - Snoop callback registration
     */
    class InterconnectComponent {
    public:
        InterconnectComponent() = default;
        ~InterconnectComponent() { shutdown(); }

        // Prevent copying
        InterconnectComponent(const InterconnectComponent&) = delete;
        InterconnectComponent& operator=(const InterconnectComponent&) = delete;

        // ========================================================================
        // Lifecycle
        // ========================================================================

        /**
         * @brief Initializes component and starts service thread.
         * @param sharedData Shared data for RAM connection.
         * @param masters Number of L1 cache masters.
         * @return true if initialization succeeded.
         */
        bool initialize(std::shared_ptr<CPUSystemSharedData> sharedData, int masters);

        /**
         * @brief Shuts down component and stops service thread.
         */
        void shutdown();

        // ========================================================================
        // Port Access
        // ========================================================================

        /**
         * @brief Gets L1->Bus port for cache.
         * @param id Cache ID.
         * @return Pointer to MasterToBus signals.
         */
        MasterToBus* portOut(int id);

        /**
         * @brief Gets Bus->L1 port for cache.
         * @param id Cache ID.
         * @return Pointer to BusToMaster signals.
         */
        BusToMaster* portIn(int id);

        /**
         * @brief Registers snoop callback for cache.
         * @param id Cache ID.
         * @param cb Callback function.
         */
        void setSnoopCallback(int id, std::function<SnoopResp(const SnoopReq&)> cb);

        /**
         * @brief Gets raw interconnect pointer.
         * @return Pointer to Interconnect.
         */
        Interconnect* raw() { return m_bus.get(); }

    private:
        /**
         * @brief Service thread main function.
         */
        void threadMain();

        std::shared_ptr<CPUSystemSharedData> m_shared{};
        std::unique_ptr<Interconnect> m_bus{};
        std::unique_ptr<std::thread> m_thread{};
        std::atomic<bool> m_running{ false };
    };

} // namespace cpu_tlp