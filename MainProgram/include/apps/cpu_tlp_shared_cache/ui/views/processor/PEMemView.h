// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/views/processor/PEMemView.h
// ============================================================================

#pragma once

/**
 * @file PEMemView.h
 * @brief Generic L1 cache memory view for any Processing Element.
 *
 * Displays PE's L1 cache contents. Single implementation for all PEs.
 *
 * @note Follows:
 *   - DRY: Single implementation for all PEs
 *   - SRP: Only handles cache display
 */

#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/CacheMemTable.h"
#include <array>
#include <cstdint>
#include <string>

 /**
  * @class PEMemView
  * @brief Generic L1 cache memory view.
  */
class PEMemView : public ICpuTLPView {
public:
    /**
     * @brief Constructs view for specific PE.
     * @param peIndex Processing element index (0-3).
     */
    explicit PEMemView(int peIndex);

    /**
     * @brief Renders the cache memory view.
     */
    void render() override;

    /**
     * @brief Sets cache line data by set and way.
     * @param setIndex Set index (0-7).
     * @param wayIndex Way index (0-1).
     * @param tag Tag string.
     * @param data Line data (32 bytes).
     * @param mesi MESI state string.
     */
    void setBySetWay(int setIndex, int wayIndex,
        const std::string& tag,
        const std::array<uint8_t, 32>& data,
        const std::string& mesi);

    /**
     * @brief Gets PE index.
     * @return PE index (0-3).
     */
    int getPEIndex() const { return m_peIndex; }

private:
    int m_peIndex;          ///< PE index (0-3)
    CacheMemTable m_table;  ///< Cache table widget
};