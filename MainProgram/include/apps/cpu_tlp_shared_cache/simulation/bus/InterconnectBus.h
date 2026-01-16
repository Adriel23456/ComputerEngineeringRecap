// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.h
// ============================================================================

#pragma once

/**
 * @file InterconnectBus.h
 * @brief Bus interconnect for MESI cache coherency protocol.
 *
 * Implements a shared bus that coordinates cache coherency between
 * multiple L1 caches with support for:
 * - Round-robin arbitration
 * - Snoop broadcasts
 * - Cache-to-cache transfers
 * - DRAM access via RAMConnection
 *
 * @note Follows:
 *   - SRP: Bus only handles arbitration and coherency, not cache logic
 *   - DIP: Uses callbacks for snoop responses
 */

#include <vector>
#include <functional>
#include <cstdint>
#include <atomic>
#include <mutex>
#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.h"

namespace cpu_tlp { struct RAMConnection; }

// ============================================================================
// L1 -> Interconnect Signals
// ============================================================================

/**
 * @struct MasterToBus
 * @brief Signals from L1 cache to interconnect.
 */
struct MasterToBus {
    std::atomic<bool> B_REQ{ false };    ///< Bus request (set true to request)
    BusCmd B_CMD{ BusCmd::BusRd };       ///< Command payload
    uint64_t B_ADDR{ 0 };                ///< Address payload (line-aligned)
    LineData B_WDATA{};                  ///< Write data (for WriteBack)
    std::atomic<bool> B_WVALID{ false }; ///< Write data valid flag
};

// ============================================================================
// Interconnect -> L1 Signals
// ============================================================================

/**
 * @struct BusToMaster
 * @brief Signals from interconnect to L1 cache.
 */
struct BusToMaster {
    std::atomic<bool> B_GRANT{ false };       ///< Bus grant
    std::atomic<bool> B_SHARED_SEEN{ false }; ///< Another cache has copy
    std::atomic<bool> B_HITM_SEEN{ false };   ///< Another cache had Modified copy
    std::atomic<bool> B_RVALID{ false };      ///< Read data valid (one-shot pulse)
    LineData B_RDATA{};                       ///< Read data payload
    std::atomic<bool> B_DONE{ false };        ///< Transaction complete (one-shot pulse)
    std::atomic<bool> B_WREADY{ true };       ///< Ready to accept write data
};

// ============================================================================
// Interconnect Bus
// ============================================================================

/**
 * @class Interconnect
 * @brief MESI cache coherency bus with DRAM backend.
 *
 * Handles:
 * - Bus arbitration (round-robin)
 * - Snoop broadcasts to all caches
 * - Cache-to-cache (C2C) transfers
 * - DRAM read/write via RAMConnection
 * - Metrics collection for analysis
 */
class Interconnect {
public:
    /**
     * @brief Constructs interconnect for given number of L1 caches.
     * @param numL1 Number of L1 cache masters (typically 4).
     */
    explicit Interconnect(int numL1);

    // ========================================================================
    // Port Access
    // ========================================================================

    /**
     * @brief Gets master-to-bus port for cache.
     * @param id Cache ID.
     * @return Pointer to MasterToBus signals.
     */
    MasterToBus* portM2B(int id) { return &m_m2b[id]; }

    /**
     * @brief Gets bus-to-master port for cache.
     * @param id Cache ID.
     * @return Pointer to BusToMaster signals.
     */
    BusToMaster* portB2M(int id) { return &m_b2m[id]; }

    // ========================================================================
    // Snoop Callback Registration
    // ========================================================================

    /**
     * @brief Attaches snoop callback for cache.
     * @param id Cache ID.
     * @param cb Callback function invoked on snoop requests.
     */
    void attachSnoopCallback(int id, std::function<SnoopResp(const SnoopReq&)> cb);

    // ========================================================================
    // Backend Binding
    // ========================================================================

    /**
     * @brief Binds DRAM backend.
     * @param ram Pointer to RAMConnection atomics.
     */
    void bindRAM(cpu_tlp::RAMConnection* ram) { m_ram = ram; }

    /**
     * @brief Binds shared data for metrics.
     * @param shared Pointer to CPUSystemSharedData.
     */
    void bindShared(cpu_tlp::CPUSystemSharedData* shared) { m_shared = shared; }

    // ========================================================================
    // Bus Cycle
    // ========================================================================

    /**
     * @brief Advances bus by one cycle.
     *
     * Handles arbitration, snoop broadcasts, C2C transfers,
     * and DRAM access state machine.
     */
    void tick();

private:
    // ========================================================================
    // Active Transaction State
    // ========================================================================

    struct ActiveTx {
        bool busy{ false };
        int owner{ -1 };
        BusCmd cmd{ BusCmd::BusRd };
        uint64_t addrLine{ 0 };

        bool seenShared{ false };
        bool seenHitm{ false };
        int mOwner{ -1 };

        int invAcksNeeded{ 0 };
        int invAcksGot{ 0 };

        bool haveRdata{ false };
        LineData rdata{};
        LineData wbLine{};

        enum class MemPhase { None, ReadReq, ReadWait, WriteReq, WriteWait } mem{ MemPhase::None };
        int segment{ 0 };

        bool signaledRvalid{ false };
        bool signaledDone{ false };

        void clear() { *this = ActiveTx{}; }
    };

    // ========================================================================
    // Data Members
    // ========================================================================

    std::vector<MasterToBus> m_m2b;
    std::vector<BusToMaster> m_b2m;
    std::vector<std::function<SnoopResp(const SnoopReq&)>> m_snoopCallbacks;
    mutable std::mutex m_cbMutex;

    int m_rrPointer{ 0 };
    ActiveTx m_tx{};

    cpu_tlp::RAMConnection* m_ram{ nullptr };
    cpu_tlp::CPUSystemSharedData* m_shared{ nullptr };

    std::vector<bool> m_reqEdgeBlock;

    // ========================================================================
    // Internal Methods
    // ========================================================================

    int pickOwnerRR();
    void clearOutputs();
    static uint16_t idx64(uint64_t addrLine);

    void startMemReadLine();
    bool stepMemReadLine();

    void startMemWriteLine();
    bool stepMemWriteLine();
};