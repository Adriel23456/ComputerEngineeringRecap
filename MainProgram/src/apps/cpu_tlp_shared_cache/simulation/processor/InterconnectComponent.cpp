// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.cpp
// ============================================================================

/**
 * @file InterconnectComponent.cpp
 * @brief Implementation of InterconnectComponent.
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.h"
#include <chrono>
#include <iostream>

namespace cpu_tlp {

    bool InterconnectComponent::initialize(std::shared_ptr<CPUSystemSharedData> sharedData,
        int masters) {
        if (m_running.load()) {
            std::cerr << "[Interconnect] Already running, cannot initialize again\n";
            return false;
        }
        if (!sharedData) {
            std::cerr << "[Interconnect] Invalid sharedData parameter\n";
            return false;
        }
        if (masters < 1 || masters > 4) {
            std::cerr << "[Interconnect] Invalid number of masters: " << masters << "\n";
            return false;
        }

        m_shared = std::move(sharedData);

        // Construct bus with N masters
        m_bus = std::make_unique<Interconnect>(masters);
        m_bus->bindShared(m_shared.get());
        m_bus->bindRAM(&m_shared->ram_connection);

        // Start service thread
        m_running.store(true, std::memory_order_release);
        m_thread = std::make_unique<std::thread>(&InterconnectComponent::threadMain, this);

        std::cout << "[Interconnect] Initialized with " << masters << " masters\n";
        return true;
    }

    void InterconnectComponent::shutdown() {
        if (!m_running.exchange(false)) {
            return;  // Already stopped
        }

        std::cout << "[Interconnect] Shutting down...\n";

        if (m_thread && m_thread->joinable()) {
            m_thread->join();
        }
        m_thread.reset();
        m_bus.reset();

        std::cout << "[Interconnect] Shutdown complete\n";
    }

    MasterToBus* InterconnectComponent::portOut(int id) {
        return m_bus ? m_bus->portM2B(id) : nullptr;
    }

    BusToMaster* InterconnectComponent::portIn(int id) {
        return m_bus ? m_bus->portB2M(id) : nullptr;
    }

    void InterconnectComponent::setSnoopCallback(int id,
        std::function<SnoopResp(const SnoopReq&)> cb) {
        if (m_bus) {
            m_bus->attachSnoopCallback(id, std::move(cb));
        }
    }

    void InterconnectComponent::threadMain() {
        using namespace std::chrono_literals;

        while (m_running.load(std::memory_order_acquire) &&
            !m_shared->system_should_stop.load(std::memory_order_acquire)) {
            if (m_bus) {
                m_bus->tick();
            }
            std::this_thread::sleep_for(1us);  // Reduce CPU usage
        }
    }

} // namespace cpu_tlp