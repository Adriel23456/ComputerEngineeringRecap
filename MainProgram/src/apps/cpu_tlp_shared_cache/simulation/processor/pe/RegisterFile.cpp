// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/processor/pe/RegisterFile.cpp
// ============================================================================

/**
 * @file RegisterFile.cpp
 * @brief Implementation of RegisterFile.
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/RegisterFile.h"

namespace cpu_tlp {

    RegisterFile::RegisterFile() {
        reset();
    }

    void RegisterFile::reset() {
        // Reset all registers EXCEPT PEID (regs[9])
        for (int i = 0; i < 12; ++i) {
            if (i == REG_PEID) continue;  // Preserve PE ID
            m_regs[i] = 0ULL;
        }

        // Configure special registers
        m_regs[REG_UPPER] = 0x0000000000000000ULL;
        m_regs[REG_LOWER] = 0xFFFFFFFFFFFFFFFFULL;
    }

    uint64_t RegisterFile::read(uint8_t addr) const {
        if (addr >= 12) return 0ULL;
        return m_regs[addr];
    }

    void RegisterFile::write(uint8_t addr, uint64_t value, bool we) {
        if (!we) return;
        if (addr >= 12) return;

        // Block writes to R0 and PEID
        if (addr == REG_ZERO || addr == REG_PEID) {
            return;
        }

        m_regs[addr] = value;

        if (onRegisterWrite) {
            onRegisterWrite(addr, value);
        }
    }

} // namespace cpu_tlp