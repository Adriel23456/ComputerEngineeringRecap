// ============================================================================
// File: include/util/gl_debug/GLDebugFormatter.h
// ============================================================================

#pragma once

/**
 * @file GLDebugFormatter.h
 * @brief Formatter interface and implementations for OpenGL debug messages.
 *
 * Converts raw debug message data into human-readable strings.
 *
 * @note Follows:
 *   - SRP: Only handles message formatting
 *   - OCP: New formatters can be added without modifying existing code
 *   - DIP: Clients depend on IGLDebugFormatter abstraction
 */

#include <glad/glad.h>
#include <string>

namespace gl_debug {

    /**
     * @class IGLDebugFormatter
     * @brief Abstract interface for debug message formatting.
     */
    class IGLDebugFormatter {
    public:
        virtual ~IGLDebugFormatter() = default;

        /**
         * @brief Formats a debug message.
         * @param source Message source.
         * @param type Message type.
         * @param id Message ID.
         * @param severity Message severity.
         * @param message Raw message text.
         * @return Formatted message string.
         */
        virtual std::string format(GLenum source, GLenum type, unsigned int id,
            GLenum severity, const char* message) const = 0;
    };

    /**
     * @class DetailedDebugFormatter
     * @brief Formats messages with full source, type, and severity information.
     */
    class DetailedDebugFormatter : public IGLDebugFormatter {
    public:
        std::string format(GLenum source, GLenum type, unsigned int id,
            GLenum severity, const char* message) const override;

    private:
        static std::string sourceToString(GLenum source);
        static std::string typeToString(GLenum type);
        static std::string severityToString(GLenum severity);
    };

    /**
     * @class CompactDebugFormatter
     * @brief Formats messages in a single-line compact format.
     */
    class CompactDebugFormatter : public IGLDebugFormatter {
    public:
        std::string format(GLenum source, GLenum type, unsigned int id,
            GLenum severity, const char* message) const override;

    private:
        static char severityToChar(GLenum severity);
        static std::string typeToShortString(GLenum type);
    };

} // namespace gl_debug