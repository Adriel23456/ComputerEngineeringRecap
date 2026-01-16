// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.h
// ============================================================================

#pragma once

/**
 * @file L1Snoop.h
 * @brief Snoop channel definitions for cache coherency.
 *
 * Defines the bus commands and snoop request/response structures
 * used for MESI protocol communication between caches.
 */

#include "L1Cache.h"
#include <array>
#include <cstdint>

 // ============================================================================
 // Line Data Type
 // ============================================================================

 /** @brief Cache line data storage (32 bytes). */
using LineData = std::array<uint8_t, LINE_BYTES>;

// ============================================================================
// Bus Command Types
// ============================================================================

/**
 * @enum BusCmd
 * @brief Commands issued on the bus for cache coherency.
 */
enum class BusCmd : uint8_t {
    BusRd = 0b000,  ///< Read request (shared access OK)
    BusRdX = 0b001,  ///< Read-for-ownership (exclusive access)
    BusUpgr = 0b010,  ///< Upgrade S -> M (invalidate other copies)
    WriteBack = 0b011   ///< Writeback dirty data to memory
};

// ============================================================================
// Snoop Request/Response Structures
// ============================================================================

/**
 * @struct SnoopReq
 * @brief Broadcast request from bus to all L1 caches.
 */
struct SnoopReq {
    BusCmd   cmd;           ///< Bus command being broadcast
    uint64_t addr_line;     ///< Line-aligned address (offset bits = 0)
    bool     grant_data;    ///< This cache should provide data if dirty
    bool     from_self;     ///< Request originated from this cache (ignore)
};

/**
 * @struct SnoopResp
 * @brief Response from L1 cache to bus snoop.
 */
struct SnoopResp {
    bool     has_shared{ false };  ///< Cache has copy (S/E/M)
    bool     has_mod{ false };     ///< Cache has modified copy (M)
    bool     inv_ack{ false };     ///< Cache invalidated its copy
    bool     rvalid{ false };      ///< Response includes valid data
    LineData rdata{};              ///< Line data (if rvalid)
};

// ============================================================================
// Usage Notes
// ============================================================================
//
// The interconnect combines snoop responses using OR:
//   - OR(has_shared) -> B_SHARED_SEEN
//   - OR(has_mod)    -> B_HITM_SEEN
//
// If any cache responds with has_mod=1, the bus identifies it as the
// "dirty owner" and calls it again with grant_data=1 to get SNP_RDATA.
//
// inv_ack signals are used to confirm invalidation for BusUpgr.
// ============================================================================