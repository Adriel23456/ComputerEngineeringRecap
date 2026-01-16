// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/views/processor/PERegView.h
// ============================================================================

#pragma once

/**
 * @file PERegView.h
 * @brief Generic register view for any Processing Element.
 *
 * Displays PE register contents. Single implementation for all PEs.
 *
 * @note Follows:
 *   - DRY: Single implementation for all PEs
 *   - SRP: Only handles register display
 */

#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/RegTable.h"
#include <cstdint>
#include <string>

 // Forward declaration for friend access
class CpuTLPSharedCacheState;

/**
 * @class PERegView
 * @brief Generic register file view.
 */
class PERegView : public ICpuTLPView {
public:
    /**
     * @brief Constructs view for specific PE.
     * @param peIndex Processing element index (0-3).
     */
    explicit PERegView(int peIndex);

    /**
     * @brief Renders the register table.
     */
    void render() override;

    /**
     * @brief Gets PE index.
     * @return PE index (0-3).
     */
    int getPEIndex() const { return m_peIndex; }

private:
    friend class CpuTLPSharedCacheState;

    // Private setters (friend access only)
    void setRegValueByIndex(int idx, uint64_t v) { m_table.setValueByIndex(idx, v); }
    void setRegValueByName(const std::string& name, uint64_t v) { m_table.setValueByName(name, v); }
    void setPEID(int peIndex) { m_table.setPEID(peIndex); }

    int m_peIndex;      ///< PE index (0-3)
    RegTable m_table;   ///< Register table widget
};