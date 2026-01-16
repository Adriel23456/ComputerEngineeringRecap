// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.h
// ============================================================================

#pragma once

/**
 * @file SharedMemory.h
 * @brief Shared RAM for CPU simulation.
 *
 * Provides thread-safe access to a 4KB shared memory space
 * used as main memory in the multi-processor simulation.
 *
 * @note Follows:
 *   - SRP: Only handles memory storage and access
 *   - Thread-safe: All operations use mutex protection
 */

#include <cstdint>
#include <vector>
#include <string>
#include <mutex>
#include <cassert>

 /**
  * @class SharedMemory
  * @brief Thread-safe shared RAM implementation.
  *
  * Implements a 4096-byte (512 x 64-bit) shared memory space
  * with optional access logging for debugging.
  */
class SharedMemory {
public:
    /** @brief Total memory size in bytes. */
    static constexpr uint16_t MEM_SIZE_BYTES = 4096;

    /** @brief Number of 64-bit words. */
    static constexpr uint16_t MEM_SIZE_WORDS = 512;

    /**
     * @struct AccessLog
     * @brief Record of a memory access for debugging.
     */
    struct AccessLog {
        std::string type;    ///< "READ", "WRITE", or "LOAD"
        uint16_t address;    ///< Byte address accessed
        uint64_t value;      ///< Value read/written
    };

    SharedMemory();

    // ========================================================================
    // Thread-Safe Operations (with logging)
    // ========================================================================

    /**
     * @brief Reads 64-bit value from memory (logged).
     * @param address Byte address (0-4088, 8-byte aligned).
     * @return Value at address.
     */
    uint64_t read(uint16_t address);

    /**
     * @brief Writes 64-bit value to memory (logged).
     * @param address Byte address (0-4088, 8-byte aligned).
     * @param value Value to write.
     */
    void write(uint16_t address, uint64_t value);

    // ========================================================================
    // Direct Access (without logging)
    // ========================================================================

    /**
     * @brief Gets value without logging.
     * @param address Byte address.
     * @return Value at address.
     */
    uint64_t get(uint16_t address) const;

    /**
     * @brief Sets value without logging.
     * @param address Byte address.
     * @param value Value to set.
     */
    void set(uint16_t address, uint64_t value);

    // ========================================================================
    // File Operations
    // ========================================================================

    /**
     * @brief Loads memory contents from binary file.
     * @param path Path to binary file.
     * @param startAddr Starting byte address for load.
     * @param align Alignment requirement (1, 2, 4, or 8).
     * @return true if load succeeded.
     */
    bool loadFromFile(const std::string& path, uint16_t startAddr = 0, size_t align = 8);

    // ========================================================================
    // Access Log Management
    // ========================================================================

    /** @brief Clears the access log. */
    void clearLog();

    /**
     * @brief Gets copy of access log.
     * @return Vector of access records.
     */
    std::vector<AccessLog> getLog() const;

    /** @brief Prints access log to console. */
    void printLog() const;

    // ========================================================================
    // Utility Functions
    // ========================================================================

    /** @brief Resets all memory to zero. */
    void reset();

    /**
     * @brief Initializes first 4 words with test pattern.
     *
     * Sets: [0x00]=0, [0x08]=1, [0x10]=2, [0x18]=3
     */
    void initTestPattern_0_1_2_3();

private:
    /**
     * @brief Clamps address to valid range.
     * @param addr Input address.
     * @return Address wrapped to [0, MEM_SIZE_BYTES-1].
     */
    static inline uint16_t clampAddr(uint16_t addr) {
        return static_cast<uint16_t>(addr % MEM_SIZE_BYTES);
    }

    mutable std::mutex memMutex;         ///< Access mutex
    std::vector<uint64_t> memory;        ///< Memory storage (512 words)
    std::vector<AccessLog> accessLog;    ///< Access history
};