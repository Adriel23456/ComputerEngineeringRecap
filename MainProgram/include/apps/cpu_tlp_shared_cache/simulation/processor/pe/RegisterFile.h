// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/processor/pe/RegisterFile.h
// ============================================================================

#pragma once

/**
 * @file RegisterFile.h
 * @brief Register file for PE component.
 *
 * Provides 12 general-purpose registers with special handling for:
 * - R0: Always zero
 * - R9: PE ID (read-only after initialization)
 * - R10: Upper bound register
 * - R11: Lower bound register
 *
 * @note Follows:
 *   - SRP: Only manages register storage and access
 *   - OCP: Callback mechanism for external observation
 */

#include <array>
#include <cstdint>
#include <functional>

namespace cpu_tlp {

    /// @brief Zero register index
    static constexpr uint8_t REG_ZERO = 0;

    /// @brief PE ID register index
    static constexpr uint8_t REG_PEID = 9;

    /// @brief Upper bound register index
    static constexpr uint8_t REG_UPPER = 10;

    /// @brief Lower bound register index
    static constexpr uint8_t REG_LOWER = 11;

    /**
     * @class RegisterFile
     * @brief 12-register file with write protection for special registers.
     */
    class RegisterFile {
    public:
        RegisterFile();

        /**
         * @brief Resets all registers except PE ID.
         */
        void reset();

        /**
         * @brief Reads register value.
         * @param addr Register address (0-11).
         * @return Register value or 0 if out of bounds.
         */
        uint64_t read(uint8_t addr) const;

        /**
         * @brief Writes register value with protection for special registers.
         * @param addr Register address.
         * @param value Value to write.
         * @param we Write enable flag.
         */
        void write(uint8_t addr, uint64_t value, bool we);

        /**
         * @brief Gets upper bound register value.
         */
        uint64_t getUpper() const { return m_regs[REG_UPPER]; }

        /**
         * @brief Gets lower bound register value.
         */
        uint64_t getLower() const { return m_regs[REG_LOWER]; }

        /**
         * @brief Sets PE ID register.
         * @param pe_id PE identifier.
         */
        void setPEID(int pe_id) { m_regs[REG_PEID] = static_cast<uint64_t>(pe_id); }

        /// @brief Callback invoked on register write (for UI synchronization)
        std::function<void(uint8_t addr, uint64_t value)> onRegisterWrite;

    private:
        std::array<uint64_t, 12> m_regs;
    };

} // namespace cpu_tlp