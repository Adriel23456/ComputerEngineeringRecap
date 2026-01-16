// ============================================================================
// File: include/util/gl_debug/GLDebugCallback.h
// ============================================================================

#pragma once

/**
 * @file GLDebugCallback.h
 * @brief OpenGL debug callback manager.
 *
 * Orchestrates filtering, formatting, and output of OpenGL debug messages.
 * Uses composition to combine filter, formatter, and output strategies.
 *
 * @note Follows:
 *   - SRP: Only manages callback registration and message routing
 *   - OCP: Behavior extensible via injected dependencies
 *   - DIP: Depends on abstractions (IGLDebugFilter, IGLDebugFormatter, IGLDebugOutput)
 */

#include "util/gl_debug/GLDebugFilter.h"
#include "util/gl_debug/GLDebugFormatter.h"
#include "util/gl_debug/GLDebugOutput.h"
#include <memory>

namespace gl_debug {

    /**
     * @class GLDebugCallback
     * @brief Manages OpenGL debug callback with configurable behavior.
     *
     * Singleton pattern to ensure single callback registration.
     * Allows runtime configuration of filter, formatter, and output.
     */
    class GLDebugCallback {
    public:
        /**
         * @brief Gets singleton instance.
         * @return Reference to singleton instance.
         */
        static GLDebugCallback& instance();

        // Prevent copying
        GLDebugCallback(const GLDebugCallback&) = delete;
        GLDebugCallback& operator=(const GLDebugCallback&) = delete;

        // ====================================================================
        // Configuration
        // ====================================================================

        /**
         * @brief Sets the message filter.
         * @param filter Filter implementation.
         */
        void setFilter(std::unique_ptr<IGLDebugFilter> filter);

        /**
         * @brief Sets the message formatter.
         * @param formatter Formatter implementation.
         */
        void setFormatter(std::unique_ptr<IGLDebugFormatter> formatter);

        /**
         * @brief Sets the message output.
         * @param output Output implementation.
         */
        void setOutput(std::unique_ptr<IGLDebugOutput> output);

        // ====================================================================
        // Lifecycle
        // ====================================================================

        /**
         * @brief Enables OpenGL debug output with current configuration.
         */
        void enable();

        /**
         * @brief Disables OpenGL debug output.
         */
        void disable();

        /**
         * @brief Checks if debug output is enabled.
         * @return true if enabled.
         */
        bool isEnabled() const { return m_enabled; }

        // ====================================================================
        // Callback Handler
        // ====================================================================

        /**
         * @brief Handles debug message from OpenGL.
         * @note Called internally by OpenGL callback.
         */
        void handleMessage(GLenum source, GLenum type, unsigned int id,
            GLenum severity, const char* message);

    private:
        GLDebugCallback();

        /**
         * @brief Static callback function for OpenGL.
         */
        static void GLAPIENTRY debugCallback(GLenum source, GLenum type,
            unsigned int id, GLenum severity,
            GLsizei length, const char* message,
            const void* userParam);

        std::unique_ptr<IGLDebugFilter> m_filter;
        std::unique_ptr<IGLDebugFormatter> m_formatter;
        std::unique_ptr<IGLDebugOutput> m_output;
        bool m_enabled = false;
    };

} // namespace gl_debug