// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/processor/L1Component.cpp
// ============================================================================

/**
 * @file L1Component.cpp
 * @brief Implementation of L1Component.
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/L1Component.h"
#include <chrono>
#include <iostream>

namespace cpu_tlp {

    bool L1Component::initialize(std::shared_ptr<CPUSystemSharedData> sharedData,
        Interconnect* ic) {
        if (m_running.load()) {
            std::cerr << "[L1Cache" << m_id << "] Already running\n";
            return false;
        }
        if (!sharedData || !ic) {
            std::cerr << "[L1Cache" << m_id << "] Invalid parameters\n";
            return false;
        }

        m_shared = std::move(sharedData);

        // Connect bus ports
        m_portOut = ic->portM2B(m_id);
        m_portIn = ic->portB2M(m_id);
        if (!m_portOut || !m_portIn) {
            std::cerr << "[L1Cache" << m_id << "] Failed to get bus ports\n";
            return false;
        }

        // Attach to cache and register snoop callback
        m_l1.attachBus(m_portOut, m_portIn, m_id);
        ic->attachSnoopCallback(m_id, [this](const SnoopReq& req) {
            return m_l1.onSnoop(req);
            });

        m_l1.reset();

        // Start service thread
        m_running.store(true, std::memory_order_release);
        m_thread = std::make_unique<std::thread>(&L1Component::threadMain, this);

        std::cout << "[L1Cache" << m_id << "] Initialized\n";
        return true;
    }

    void L1Component::shutdown() {
        if (!m_running.exchange(false)) {
            return;
        }

        std::cout << "[L1Cache" << m_id << "] Shutting down...\n";

        if (m_thread && m_thread->joinable()) {
            m_thread->join();
        }
        m_thread.reset();

        std::cout << "[L1Cache" << m_id << "] Shutdown complete\n";
    }

    CpuReq L1Component::readCpuReq() {
        CpuReq req{};
        auto& conn = m_shared->cache_connections[m_id];

        req.ALUOut_M = conn.ALUOut_M.load(std::memory_order_acquire);
        req.RD_Rm_Special_M = conn.RD_Rm_Special_M.load(std::memory_order_acquire);
        req.C_WE_M = conn.C_WE_M.load(std::memory_order_acquire);
        req.C_ISB_M = conn.C_ISB_M.load(std::memory_order_acquire);
        req.C_REQUEST_M = conn.C_REQUEST_M.load(std::memory_order_acquire);
        req.C_READY_ACK = conn.C_READY_ACK.load(std::memory_order_acquire);
        return req;
    }

    void L1Component::writeCpuResp(const CpuResp& resp) {
        auto& conn = m_shared->cache_connections[m_id];
        conn.RD_C_out.store(resp.RD_C_out, std::memory_order_release);
        conn.C_READY.store(resp.C_READY, std::memory_order_release);
    }

    void L1Component::threadMain() {
        using namespace std::chrono_literals;

        while (m_running.load(std::memory_order_acquire) &&
            !m_shared->system_should_stop.load(std::memory_order_acquire)) {
            // Sample CPU request
            const CpuReq req = readCpuReq();

            // Run cache FSM
            m_l1.beginAccess(req);
            m_l1.tick();

            // Publish response
            writeCpuResp(m_l1.output());

            std::this_thread::sleep_for(100us);  // Balance responsiveness vs CPU
        }
    }

} // namespace cpu_tlp