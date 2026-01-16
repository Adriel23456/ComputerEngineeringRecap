// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.h
// ============================================================================

#pragma once

/**
 * @file AnalysisDataView.h
 * @brief Analysis data visualization view.
 *
 * Displays simulation metrics including cache misses, invalidations,
 * MESI transactions, and per-PE traffic statistics.
 *
 * @note Follows:
 *   - SRP: Only handles analysis data display
 *   - OCP: New metrics can be added without modifying render logic
 */

#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include <cstdint>
#include <array>

 /**
  * @class AnalysisDataView
  * @brief View panel for simulation analysis metrics.
  *
  * Provides real-time display of cache coherency statistics
  * and per-PE traffic counters.
  */
class AnalysisDataView : public ICpuTLPView {
public:
    /**
     * @brief Renders the analysis data table.
     */
    void render() override;

    // ========================================================================
    // Metric Setters
    // ========================================================================

    /** @brief Sets cache miss count. */
    void setCacheMisses(uint64_t value) { m_cacheMisses = value; }

    /** @brief Sets invalidation count. */
    void setInvalidations(uint64_t value) { m_invalidations = value; }

    /** @brief Sets total read/write operation count. */
    void setReadWriteOps(uint64_t value) { m_readWriteOps = value; }

    /** @brief Sets MESI transaction count. */
    void setTransactionsMESI(uint64_t value) { m_transactionsMESI = value; }

    /** @brief Sets PE0 traffic counter. */
    void setTrafficPE0(uint64_t value) { m_trafficPE[0] = value; }

    /** @brief Sets PE1 traffic counter. */
    void setTrafficPE1(uint64_t value) { m_trafficPE[1] = value; }

    /** @brief Sets PE2 traffic counter. */
    void setTrafficPE2(uint64_t value) { m_trafficPE[2] = value; }

    /** @brief Sets PE3 traffic counter. */
    void setTrafficPE3(uint64_t value) { m_trafficPE[3] = value; }

    /**
     * @brief Sets traffic counter for a specific PE.
     * @param peIndex PE index (0-3).
     * @param value Traffic count.
     */
    void setTrafficPE(size_t peIndex, uint64_t value) {
        if (peIndex < 4) {
            m_trafficPE[peIndex] = value;
        }
    }

    /**
     * @brief Resets all metrics to zero.
     */
    void reset() {
        m_cacheMisses = 0;
        m_invalidations = 0;
        m_readWriteOps = 0;
        m_transactionsMESI = 0;
        m_trafficPE.fill(0);
    }

private:
    uint64_t m_cacheMisses = 0;      ///< Total cache misses
    uint64_t m_invalidations = 0;    ///< Line invalidation count
    uint64_t m_readWriteOps = 0;     ///< Total memory operations
    uint64_t m_transactionsMESI = 0; ///< MESI protocol transactions

    std::array<uint64_t, 4> m_trafficPE = { 0, 0, 0, 0 }; ///< Per-PE traffic
};