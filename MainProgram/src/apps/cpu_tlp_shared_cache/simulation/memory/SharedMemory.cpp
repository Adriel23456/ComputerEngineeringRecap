// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.cpp
// ============================================================================

/**
 * @file SharedMemory.cpp
 * @brief Implementation of SharedMemory class.
 */

#include "apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstring>

 // ============================================================================
 // Construction
 // ============================================================================

SharedMemory::SharedMemory()
    : memory(MEM_SIZE_WORDS, 0)
{
}

// ============================================================================
// Logged Operations
// ============================================================================

uint64_t SharedMemory::read(uint16_t address) {
    std::lock_guard<std::mutex> lock(memMutex);

    address = clampAddr(address);

    // Read 8 bytes from the specified address
    uint64_t result = 0;
    for (int i = 0; i < 8; ++i) {
        const uint16_t byteAddr = static_cast<uint16_t>(address + i);
        const uint16_t clamped = clampAddr(byteAddr);

        const uint16_t wordIndex = static_cast<uint16_t>(clamped / 8);
        const uint8_t byteOffset = static_cast<uint8_t>(clamped % 8);

        const uint8_t byteValue = static_cast<uint8_t>(
            (memory[wordIndex] >> (byteOffset * 8)) & 0xFF);
        result |= (static_cast<uint64_t>(byteValue) << (i * 8));
    }

    accessLog.push_back({ "READ", address, result });
    return result;
}

void SharedMemory::write(uint16_t address, uint64_t value) {
    std::lock_guard<std::mutex> lock(memMutex);

    address = clampAddr(address);

    // Write 8 bytes to the specified address
    for (int i = 0; i < 8; ++i) {
        const uint16_t byteAddr = static_cast<uint16_t>(address + i);
        const uint16_t clamped = clampAddr(byteAddr);

        const uint16_t wordIndex = static_cast<uint16_t>(clamped / 8);
        const uint8_t byteOffset = static_cast<uint8_t>(clamped % 8);

        const uint8_t byteValue = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
        const uint64_t mask = ~(0xFFULL << (byteOffset * 8));

        memory[wordIndex] = (memory[wordIndex] & mask) |
            (static_cast<uint64_t>(byteValue) << (byteOffset * 8));
    }

    accessLog.push_back({ "WRITE", address, value });
}

// ============================================================================
// Direct Access (Unlogged)
// ============================================================================

uint64_t SharedMemory::get(uint16_t address) const {
    std::lock_guard<std::mutex> lock(memMutex);

    address = clampAddr(address);

    uint64_t result = 0;
    for (int i = 0; i < 8; ++i) {
        const uint16_t byteAddr = static_cast<uint16_t>(address + i);
        const uint16_t clamped = clampAddr(byteAddr);

        const uint16_t wordIndex = static_cast<uint16_t>(clamped / 8);
        const uint8_t byteOffset = static_cast<uint8_t>(clamped % 8);

        const uint8_t byteValue = static_cast<uint8_t>(
            (memory[wordIndex] >> (byteOffset * 8)) & 0xFF);
        result |= (static_cast<uint64_t>(byteValue) << (i * 8));
    }

    return result;
}

void SharedMemory::set(uint16_t address, uint64_t value) {
    std::lock_guard<std::mutex> lock(memMutex);

    address = clampAddr(address);

    for (int i = 0; i < 8; ++i) {
        const uint16_t byteAddr = static_cast<uint16_t>(address + i);
        const uint16_t clamped = clampAddr(byteAddr);

        const uint16_t wordIndex = static_cast<uint16_t>(clamped / 8);
        const uint8_t byteOffset = static_cast<uint8_t>(clamped % 8);

        const uint8_t byteValue = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
        const uint64_t mask = ~(0xFFULL << (byteOffset * 8));

        memory[wordIndex] = (memory[wordIndex] & mask) |
            (static_cast<uint64_t>(byteValue) << (byteOffset * 8));
    }
}

// ============================================================================
// File Loading
// ============================================================================

bool SharedMemory::loadFromFile(const std::string& path, uint16_t startAddr, size_t align) {
    // Validate alignment parameter
    if (align != 1 && align != 2 && align != 4 && align != 8) {
        std::cerr << "[SharedMemory] Invalid alignment: " << align << "\n";
        return false;
    }

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[SharedMemory] Could not open file: " << path << "\n";
        return false;
    }

    std::streamsize fileSize = file.tellg();
    if (fileSize <= 0) {
        std::cerr << "[SharedMemory] Empty or invalid file: " << path << "\n";
        return false;
    }
    file.seekg(0, std::ios::beg);

    // Calculate available capacity from start address
    if (startAddr >= MEM_SIZE_BYTES) {
        std::cerr << "[SharedMemory] startAddr out of range\n";
        return false;
    }

    const size_t capacityFromStart = MEM_SIZE_BYTES - startAddr;
    const size_t bytesToRead = static_cast<size_t>(
        std::min<std::streamsize>(fileSize, static_cast<std::streamsize>(capacityFromStart)));

    std::vector<uint8_t> buffer(bytesToRead);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), bytesToRead)) {
        std::cerr << "[SharedMemory] Error reading file: " << path << "\n";
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(memMutex);

        // Clear memory and log
        std::fill(memory.begin(), memory.end(), 0);
        accessLog.clear();

        // Write bytes in little-endian 64-bit word layout
        for (size_t i = 0; i < bytesToRead; ++i) {
            const uint16_t byteAddr = static_cast<uint16_t>(startAddr + i);
            const uint16_t clamped = clampAddr(byteAddr);

            const uint16_t wordIndex = static_cast<uint16_t>(clamped / 8);
            const uint8_t byteOff = static_cast<uint8_t>(clamped % 8);

            const uint64_t mask = ~(0xFFULL << (byteOff * 8));
            const uint64_t v = static_cast<uint64_t>(buffer[i]) << (byteOff * 8);
            memory[wordIndex] = (memory[wordIndex] & mask) | v;

            // Log completed words
            const bool completedWord = (byteOff == 7) || (i + 1 == bytesToRead);
            if (completedWord) {
                accessLog.push_back({
                    "LOAD",
                    static_cast<uint16_t>(wordIndex * 8),
                    memory[wordIndex]
                    });
            }
        }

        std::cout << "[SharedMemory] Loaded " << bytesToRead << " bytes from " << path
            << " at byte address " << startAddr;

        if (static_cast<size_t>(fileSize) > bytesToRead) {
            std::cout << " (truncated " << (static_cast<size_t>(fileSize) - bytesToRead)
                << " bytes)";
        }
        else if (startAddr > 0 || bytesToRead < MEM_SIZE_BYTES) {
            const size_t zeroFilledBytes = MEM_SIZE_BYTES - bytesToRead - startAddr;
            if (zeroFilledBytes > 0) {
                std::cout << " (filled " << zeroFilledBytes << " remaining bytes with 0x0)";
            }
        }
        std::cout << "\n";
    }

    return true;
}

// ============================================================================
// Access Log Management
// ============================================================================

void SharedMemory::clearLog() {
    std::lock_guard<std::mutex> lock(memMutex);
    accessLog.clear();
}

std::vector<SharedMemory::AccessLog> SharedMemory::getLog() const {
    std::lock_guard<std::mutex> lock(memMutex);
    return accessLog;
}

void SharedMemory::printLog() const {
    std::lock_guard<std::mutex> lock(memMutex);

    std::cout << "---- Memory Access Log ----\n";
    for (const auto& e : accessLog) {
        std::cout << e.type << " [Byte Addr " << e.address << "] = 0x"
            << std::hex << e.value << std::dec << "\n";
    }
    std::cout << "---------------------------\n";
}

// ============================================================================
// Utility Functions
// ============================================================================

void SharedMemory::reset() {
    std::lock_guard<std::mutex> lock(memMutex);
    std::fill(memory.begin(), memory.end(), 0);
    accessLog.clear();
}

void SharedMemory::initTestPattern_0_1_2_3() {
    std::lock_guard<std::mutex> lock(memMutex);

    // Set: [0x00]=0, [0x08]=1, [0x10]=2, [0x18]=3
    memory.assign(MEM_SIZE_WORDS, 0);
    memory[0] = 0;  // Bytes [0..7]
    memory[1] = 1;  // Bytes [8..15]
    memory[2] = 2;  // Bytes [16..23]
    memory[3] = 3;  // Bytes [24..31]

    accessLog.clear();
    accessLog.push_back({ "LOAD", 0x00, 0 });
    accessLog.push_back({ "LOAD", 0x08, 1 });
    accessLog.push_back({ "LOAD", 0x10, 2 });
    accessLog.push_back({ "LOAD", 0x18, 3 });
}