// ============================================================================
// File: src/util/gl_debug/GLDebugCallback.cpp
// ============================================================================

/**
 * @file GLDebugCallback.cpp
 * @brief Implementation of GLDebugCallback.
 */

#include "util/gl_debug/GLDebugCallback.h"

namespace gl_debug {

    GLDebugCallback& GLDebugCallback::instance() {
        static GLDebugCallback instance;
        return instance;
    }

    GLDebugCallback::GLDebugCallback()
        : m_filter(std::make_unique<DefaultDebugFilter>())
        , m_formatter(std::make_unique<DetailedDebugFormatter>())
        , m_output(std::make_unique<ConsoleDebugOutput>())
    {
    }

    void GLDebugCallback::setFilter(std::unique_ptr<IGLDebugFilter> filter) {
        if (filter) {
            m_filter = std::move(filter);
        }
    }

    void GLDebugCallback::setFormatter(std::unique_ptr<IGLDebugFormatter> formatter) {
        if (formatter) {
            m_formatter = std::move(formatter);
        }
    }

    void GLDebugCallback::setOutput(std::unique_ptr<IGLDebugOutput> output) {
        if (output) {
            m_output = std::move(output);
        }
    }

    void GLDebugCallback::enable() {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        m_enabled = true;
    }

    void GLDebugCallback::disable() {
        glDisable(GL_DEBUG_OUTPUT);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(nullptr, nullptr);
        m_enabled = false;
    }

    void GLDebugCallback::handleMessage(GLenum source, GLenum type, unsigned int id,
        GLenum severity, const char* message) {
        // Apply filter
        if (m_filter && m_filter->shouldFilter(source, type, id, severity)) {
            return;
        }

        // Format message
        std::string formatted;
        if (m_formatter) {
            formatted = m_formatter->format(source, type, id, severity, message);
        }
        else {
            formatted = message;
        }

        // Output message
        if (m_output) {
            m_output->write(formatted);
        }
    }

    void GLAPIENTRY GLDebugCallback::debugCallback(GLenum source, GLenum type,
        unsigned int id, GLenum severity,
        GLsizei /*length*/, const char* message,
        const void* /*userParam*/) {
        instance().handleMessage(source, type, id, severity, message);
    }

} // namespace gl_debug