// ============================================================================
// File: include/util/gl_debug/GLDebugOutput.h
// ============================================================================

#pragma once

/**
 * @file GLDebugOutput.h
 * @brief Abstract interface for OpenGL debug message output.
 *
 * Defines output strategy for debug messages, allowing different
 * implementations (console, file, logger, etc.).
 *
 * @note Follows:
 *   - SRP: Only defines output interface
 *   - OCP: Extensible via inheritance without modification
 *   - DIP: Clients depend on abstraction, not concrete output
 */

#include <string>

namespace gl_debug {

    /**
     * @class IGLDebugOutput
     * @brief Abstract interface for debug message output.
     */
    class IGLDebugOutput {
    public:
        virtual ~IGLDebugOutput() = default;

        /**
         * @brief Outputs a formatted debug message.
         * @param message Formatted message string.
         */
        virtual void write(const std::string& message) = 0;
    };

    /**
     * @class ConsoleDebugOutput
     * @brief Outputs debug messages to standard output.
     */
    class ConsoleDebugOutput : public IGLDebugOutput {
    public:
        void write(const std::string& message) override;
    };

} // namespace gl_debug