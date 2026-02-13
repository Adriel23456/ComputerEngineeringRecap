/**
 * @file TomasuloRegisterFile.cpp
 * @brief Implementation of TomasuloRegisterFile.
 */

#include "apps/cpu_tomasulo/simulation/processor/TomasuloRegisterFile.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

 // ============================================================================
 // Construction / Reset
 // ============================================================================

TomasuloRegisterFile::TomasuloRegisterFile() {
    reset();
}

void TomasuloRegisterFile::reset() {
    for (auto& r : m_regs) {
        r.value = 0x0000000000000000ULL;
        r.qi = 0;
        r.qi_valid = false;
    }
    m_regs[LOWER].value = 0xFFFFFFFFFFFFFFFFULL;
}

// ============================================================================
// Value access (backward-compatible)
// ============================================================================

uint64_t TomasuloRegisterFile::get(int idx) const {
    if (idx < 0 || idx >= kCount) return 0;
    return m_regs[idx].value;
}

void TomasuloRegisterFile::set(int idx, uint64_t value) {
    if (idx >= 0 && idx < kCount)
        m_regs[idx].value = value;
}

// ============================================================================
// Full entry access
// ============================================================================

const TomasuloRegEntry& TomasuloRegisterFile::getEntry(int idx) const {
    static const TomasuloRegEntry kEmpty{};
    if (idx < 0 || idx >= kCount) return kEmpty;
    return m_regs[idx];
}

TomasuloRegEntry& TomasuloRegisterFile::getEntry(int idx) {
    return m_regs[idx]; // caller is responsible for bounds
}

// ============================================================================
// Tag access
// ============================================================================

uint8_t TomasuloRegisterFile::getQi(int idx) const {
    if (idx < 0 || idx >= kCount) return 0;
    return m_regs[idx].qi;
}

bool TomasuloRegisterFile::getQiValid(int idx) const {
    if (idx < 0 || idx >= kCount) return false;
    return m_regs[idx].qi_valid;
}

void TomasuloRegisterFile::setQi(int idx, uint8_t qi, bool valid) {
    if (idx >= 0 && idx < kCount) {
        m_regs[idx].qi = qi;
        m_regs[idx].qi_valid = valid;
    }
}

// ============================================================================
// Named access
// ============================================================================

uint64_t TomasuloRegisterFile::getByName(const std::string& name) const {
    int idx = nameToIndex(name);
    if (idx < 0) throw std::runtime_error("Unknown register name: " + name);
    return m_regs[idx].value;
}

void TomasuloRegisterFile::setByName(const std::string& name, uint64_t value) {
    int idx = nameToIndex(name);
    if (idx < 0) throw std::runtime_error("Unknown register name: " + name);
    m_regs[idx].value = value;
}

// ============================================================================
// Name resolver
// ============================================================================

int TomasuloRegisterFile::nameToIndex(const std::string& name) {
    std::string k;
    k.reserve(name.size());
    for (char c : name)
        k.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));

    if (k == "UPPER") return UPPER;
    if (k == "LOWER") return LOWER;
    if (k == "PEID")  return PEID;

    if (k.rfind("REG", 0) == 0 && k.size() >= 4) {
        try {
            int n = std::stoi(k.substr(3));
            if (n >= 0 && n <= 12) return n;
        }
        catch (...) {}
    }
    return -1;
}