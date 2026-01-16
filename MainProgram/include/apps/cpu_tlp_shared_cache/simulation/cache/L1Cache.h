// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h
// ============================================================================

#ifndef L1_CACHE_H
#define L1_CACHE_H

/**
 * @file L1Cache.h
 * @brief L1 Cache implementation with MESI coherency protocol.
 *
 * Implements a 2-way set-associative L1 cache with 8 sets and 32-byte lines.
 * Uses MESI protocol for cache coherency in a multi-processor system.
 *
 * @note Follows:
 *   - SRP: Cache logic separated from bus and snoop handling
 *   - Thread-safe: All public methods use mutex protection
 */

#include <array>
#include <cstdint>
#include <cstddef>
#include <mutex>

 // ============================================================================
 // Cache Configuration Parameters
 // ============================================================================

 /** @brief Cache line size in bytes. */
constexpr std::size_t LINE_BYTES = 32;

/** @brief Number of ways (associativity). */
constexpr std::size_t WAYS = 2;

/** @brief Number of sets in the cache. */
constexpr std::size_t SETS = 8;

static_assert((SETS& (SETS - 1)) == 0, "SETS must be a power of 2");

/**
 * @brief Compile-time integer log base 2.
 * @param n Value to compute log2 of.
 * @return Floor of log2(n).
 */
constexpr int ilog2(std::size_t n) {
    return (n <= 1) ? 0 : 1 + ilog2(n >> 1);
}

/** @brief Number of bits for byte offset within a line. */
constexpr int OFFSET_BITS = ilog2(LINE_BYTES);

/** @brief Number of bits for set index. */
constexpr int INDEX_BITS = ilog2(SETS);

/** @brief Number of bits for tag (remaining address bits). */
constexpr int TAG_BITS = 64 - OFFSET_BITS - INDEX_BITS;

/** @brief Sentinel value for invalid tag (56 bits set to 1). */
static constexpr uint64_t INVALID_TAG_SENTINEL = 0x00FFFFFFFFFFFFFFULL;

// ============================================================================
// MESI Protocol States
// ============================================================================

/**
 * @enum Mesi
 * @brief MESI cache coherency protocol states.
 */
enum class Mesi : uint8_t {
    I = 0,  ///< Invalid - line not present or stale
    S = 1,  ///< Shared - clean copy, others may have copies
    E = 2,  ///< Exclusive - clean copy, no other caches have it
    M = 3   ///< Modified - dirty copy, must write back before eviction
};

// Include snoop channel definitions
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.h"

// Forward declarations for bus ports
struct MasterToBus;
struct BusToMaster;

// ============================================================================
// Cache Data Structures
// ============================================================================

/**
 * @struct CacheLine
 * @brief Represents a single cache line with tag, state, and data.
 */
struct CacheLine {
    uint64_t tag = INVALID_TAG_SENTINEL;  ///< Address tag
    Mesi     state = Mesi::I;               ///< MESI coherency state
    LineData data{};                        ///< Actual cached data (32 bytes)
    bool     valid = false;                 ///< Line validity flag
};

/**
 * @struct CacheSet
 * @brief A set containing multiple ways with LRU tracking.
 */
struct CacheSet {
    std::array<CacheLine, WAYS> ways{};  ///< Cache lines in this set
    uint8_t lru = 0;                      ///< LRU indicator (0 = way0 MRU)
};

/**
 * @struct AddrParts
 * @brief Decomposed address into tag, set index, and offset.
 */
struct AddrParts {
    uint64_t tag;           ///< Tag portion of address
    uint32_t set;           ///< Set index
    uint32_t off;           ///< Byte offset within line
    uint8_t  dw_in_line;    ///< Double-word index within line (0-3)
    uint8_t  byte_in_line;  ///< Byte index within line (0-31)
};

// ============================================================================
// CPU <-> L1 Cache Interface
// ============================================================================

/**
 * @struct CpuReq
 * @brief Request from CPU to L1 cache.
 */
struct CpuReq {
    bool     C_REQUEST_M{ false };      ///< Memory request active
    bool     C_WE_M{ false };           ///< Write enable (true = write)
    bool     C_ISB_M{ false };          ///< Byte access mode (vs word)
    uint64_t ALUOut_M{ 0 };             ///< Memory address
    uint64_t RD_Rm_Special_M{ 0 };      ///< Write data
    bool     C_READY_ACK{ false };      ///< CPU acknowledges ready signal
};

/**
 * @struct CpuResp
 * @brief Response from L1 cache to CPU.
 */
struct CpuResp {
    bool     C_READY{ false };   ///< Data/operation ready
    uint64_t RD_C_out{ 0 };      ///< Read data output
};

// ============================================================================
// L1 Cache FSM States
// ============================================================================

/**
 * @enum L1State
 * @brief Finite state machine states for L1 cache controller.
 */
enum class L1State : uint8_t {
    IDLE,        ///< Waiting for CPU request
    LOOKUP,      ///< Checking cache for hit/miss
    MISS,        ///< Cache miss detected (unused, kept for compatibility)
    WAIT_ACK,    ///< Waiting for CPU to acknowledge completion
    REQ_BUS,     ///< Requesting bus access
    WAIT_GRANT,  ///< Waiting for bus grant
    WAIT_DATA,   ///< Waiting for data from bus/memory
    FILL         ///< Filling cache line with received data
};

// ============================================================================
// Thread-Safe Cache Line Info Export
// ============================================================================

/**
 * @struct CacheLineInfo
 * @brief Snapshot of cache line state for external inspection.
 *
 * Used by UI to display cache contents without holding locks.
 */
struct CacheLineInfo {
    uint64_t tag;                        ///< Line tag
    Mesi state;                          ///< MESI state
    std::array<uint8_t, 32> data;        ///< Line data copy
    bool valid;                          ///< Validity flag
};

// ============================================================================
// L1Cache Class
// ============================================================================

/**
 * @class L1Cache
 * @brief L1 cache controller with MESI coherency.
 *
 * Implements a 2-way set-associative cache with:
 * - 8 sets, 32-byte lines = 512 bytes total capacity
 * - MESI protocol for multi-processor coherency
 * - Bus interface for memory and snoop transactions
 *
 * @note Thread-safe: internal mutex protects all state.
 */
class L1Cache {
public:
    /**
     * @brief Constructs and initializes the cache.
     */
    L1Cache();

    /** @brief Mutex for thread-safe access. */
    mutable std::mutex mtx_;

    /**
     * @brief Resets cache to initial empty state.
     */
    void reset();

    /**
     * @brief Initiates a new CPU memory access.
     * @param req CPU request parameters.
     */
    void beginAccess(const CpuReq& req);

    /**
     * @brief Advances the cache FSM by one cycle.
     */
    void tick();

    /**
     * @brief Gets current cache output to CPU.
     * @return Current response signals.
     */
    CpuResp output() const { return out_; }

    /**
     * @brief Attaches cache to bus interconnect.
     * @param out_port Outgoing signals to bus.
     * @param in_port Incoming signals from bus.
     * @param my_id This cache's master ID.
     */
    void attachBus(MasterToBus* out_port, BusToMaster* in_port, int my_id) {
        pm_out_ = out_port;
        pm_in_ = in_port;
        l1_id_ = my_id;
    }

    /**
     * @brief Handles incoming snoop request from bus.
     * @param s Snoop request details.
     * @return Snoop response indicating cache state.
     */
    SnoopResp onSnoop(const SnoopReq& s);

    /**
     * @brief Gets thread-safe snapshot of a cache line.
     * @param set Set index (0-7).
     * @param way Way index (0-1).
     * @return Copy of line info, or empty info if invalid indices.
     */
    CacheLineInfo getLineInfo(uint32_t set, uint32_t way) const {
        std::lock_guard<std::mutex> lk(mtx_);
        if (set >= SETS || way >= WAYS) {
            CacheLineInfo info;
            info.tag = 0;
            info.state = Mesi::I;
            info.data.fill(0);
            info.valid = false;
            return info;
        }
        const auto& line = sets_[set].ways[way];
        CacheLineInfo info;
        info.tag = line.tag;
        info.state = line.state;
        info.data = line.data;
        info.valid = line.valid;
        return info;
    }

    /** @brief Gets number of offset bits in address. */
    static constexpr int offsetBits() { return OFFSET_BITS; }

    /** @brief Gets number of index bits in address. */
    static constexpr int indexBits() { return INDEX_BITS; }

    /** @brief Gets number of tag bits in address. */
    static constexpr int tagBits() { return TAG_BITS; }

private:
    friend AddrParts splitAddress(uint64_t addr);

    /**
     * @brief Finds way containing specified tag in a set.
     * @param set Set index.
     * @param tag Tag to search for.
     * @return Way index if found, -1 otherwise.
     */
    int findWay(uint32_t set, uint64_t tag) const;

    /**
     * @brief Logs signal changes for debugging.
     */
    void logSignals_();

    // Request blocking flag
    bool pe_req_block_ = false;

    // Cache storage
    std::array<CacheSet, SETS> sets_{};

    // FSM state
    L1State fsm_{ L1State::IDLE };

    // Current request/response
    CpuReq  in_{};
    CpuResp out_{};

    // Bus port pointers
    MasterToBus* pm_out_{ nullptr };
    BusToMaster* pm_in_{ nullptr };
    int          l1_id_{ -1 };

    // Edge detection for request signal
    bool prev_req_{ false };

    // Debug state tracking
    L1State dbg_prev_fsm_{ L1State::IDLE };
    bool    dbg_prev_B_REQ_{ false };
    bool    dbg_prev_B_GRANT_{ false };
    bool    dbg_prev_B_RVALID_{ false };
    bool    dbg_prev_B_DONE_{ false };

    /**
     * @struct PendingTx
     * @brief Tracks state of pending bus transaction.
     */
    struct PendingTx {
        BusCmd   cmd{ BusCmd::BusRd };      ///< Bus command type
        uint64_t req_addr_line{ 0 };        ///< Requested line address
        uint32_t set{ 0 };                  ///< Target set index
        int      victim{ -1 };              ///< Selected victim way

        bool     need_wb{ false };          ///< Writeback needed before fill
        uint64_t victim_addr_line{ 0 };     ///< Victim's line address for WB
        LineData wb_line{};                 ///< Victim's data for WB

        bool     saw_shared{ false };       ///< Another cache has copy
        bool     saw_hitm{ false };         ///< Another cache has modified copy

        // Frozen offsets from original access
        uint8_t  byte_in_line{ 0 };         ///< Byte offset for access
        uint8_t  dw_in_line{ 0 };           ///< Double-word offset

        // Transaction tracking for proper snoop response
        bool     is_active{ false };        ///< Transaction in progress
        Mesi     target_state{ Mesi::I };   ///< Expected final MESI state
    } pend_;

    // Temporary storage for incoming line data
    LineData temp_fill_{};
};

#endif // L1_CACHE_H