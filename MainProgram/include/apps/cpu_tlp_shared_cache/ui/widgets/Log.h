// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/widgets/Log.h
// ============================================================================

#pragma once

/**
 * @file Log.h
 * @brief Thread-safe logging utilities.
 *
 * Provides mutex-protected console output for use across
 * multiple threads in the simulation.
 */

#include <mutex>
#include <sstream>
#include <iostream>

 /**
  * @brief Gets the global log mutex.
  * @return Reference to static mutex instance.
  */
inline std::mutex& log_mutex() {
    static std::mutex m;
    return m;
}

/**
 * @brief Thread-safe single line output.
 * @param s String to output.
 */
inline void log_line(const std::string& s) {
    std::lock_guard<std::mutex> lk(log_mutex());
    std::cout << s << std::flush;
}

/**
 * @brief Builds and outputs a log message atomically.
 *
 * Example:
 * @code
 * log_build_and_print([](std::ostringstream& oss) {
 *     oss << "[Module] Value = " << value << "\n";
 * });
 * @endcode
 *
 * @tparam Fn Builder function type.
 * @param builder Function that receives ostringstream reference.
 */
template <typename Fn>
inline void log_build_and_print(Fn&& builder) {
    std::ostringstream oss;
    builder(oss);
    log_line(oss.str());
}