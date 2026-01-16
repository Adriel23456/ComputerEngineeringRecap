// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/widgets/RegTable.h
// ============================================================================

#pragma once

/**
 * @file RegTable.h
 * @brief Register file visualization table widget.
 *
 * Displays PE register contents with multiple format views.
 *
 * @note Follows:
 *   - SRP: Only handles register display
 *   - Encapsulation: Write access controlled via friend classes
 */

#include <array>
#include <cstdint>
#include <string>

 // Forward declarations for friend access
class CpuTLPSharedCacheState;
class PERegView;  // <-- ADD THIS

/**
 * @class RegTable
 * @brief Table widget for displaying processor registers.
 *
 * Features:
 * - 12 registers (REG0-REG8, PEID, UPPER_REG, LOWER_REG)
 * - Read-only display with context menu copy
 * - Multiple format columns: hex, decimal, double
 */
class RegTable {
public:
    static constexpr int kRegCount = 12;

    /**
     * @enum RegIndex
     * @brief Register indices.
     */
    enum RegIndex : int {
        REG0 = 0, REG1, REG2, REG3, REG4, REG5, REG6, REG7, REG8,
        PEID,
        UPPER_REG,
        LOWER_REG
    };

    /**
     * @brief Constructs register table for specific PE.
     * @param peIndex Processing element ID (sets initial PEID register).
     */
    explicit RegTable(int peIndex = 0);

    // ========================================================================
    // Read-Only Public Access
    // ========================================================================

    /**
     * @brief Gets register value by index.
     * @param idx Register index (0-11).
     * @return Register value, or 0 if invalid index.
     */
    uint64_t getValueByIndex(int idx) const;

    /**
     * @brief Gets register hex text by index.
     * @param idx Register index (0-11).
     * @return Hex string representation.
     */
    const std::string& getHexTextByIndex(int idx) const;

    /**
     * @brief Renders the register table.
     * @param id ImGui widget ID.
     */
    void render(const char* id);

private:
    // Friend classes for write access
    friend class CpuTLPSharedCacheState;
    friend class PERegView;  // <-- ADD THIS

    // ========================================================================
    // Private Setters (Friend Access Only)
    // ========================================================================

    void setPEID(int peIndex);
    void setValueByIndex(int idx, uint64_t val);
    void setValueByName(const std::string& key, uint64_t val);

    // ========================================================================
    // Data Members
    // ========================================================================

    std::array<std::string, kRegCount> m_names{};      ///< Register names
    std::array<uint64_t, kRegCount> m_values{};        ///< Register values
    std::array<std::string, kRegCount> m_valueText{};  ///< Hex text cache

    // ========================================================================
    // Helpers
    // ========================================================================

    static std::string formatHex(uint64_t v);
    static int indexFromName(const std::string& keyIn);
};