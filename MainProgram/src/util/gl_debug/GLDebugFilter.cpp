// ============================================================================
// File: src/util/gl_debug/GLDebugFilter.cpp
// ============================================================================

/**
 * @file GLDebugFilter.cpp
 * @brief Implementation of GLDebugFilter classes.
 */

#include "util/gl_debug/GLDebugFilter.h"
#include <algorithm>

namespace gl_debug {

    // ========================================================================
    // DefaultDebugFilter
    // ========================================================================

    DefaultDebugFilter::DefaultDebugFilter() {
        // Default ignored IDs (non-significant messages)
        m_ignoredIds = {
            131169,  // Framebuffer detailed info
            131185,  // Buffer detailed info
            131218,  // Program/shader state performance warning
            131204,  // Texture state usage warning
            131222,  // Program/shader state performance warning
            13       // Generic non-significant
        };
    }

    bool DefaultDebugFilter::shouldFilter(GLenum /*source*/, GLenum type,
        unsigned int id, GLenum /*severity*/) const {
        // Filter performance messages if enabled
        if (m_filterPerformance && type == GL_DEBUG_TYPE_PERFORMANCE) {
            return true;
        }

        // Filter ignored IDs
        auto it = std::find(m_ignoredIds.begin(), m_ignoredIds.end(), id);
        return it != m_ignoredIds.end();
    }

    void DefaultDebugFilter::addIgnoredId(unsigned int id) {
        if (std::find(m_ignoredIds.begin(), m_ignoredIds.end(), id) == m_ignoredIds.end()) {
            m_ignoredIds.push_back(id);
        }
    }

    void DefaultDebugFilter::setFilterPerformance(bool filter) {
        m_filterPerformance = filter;
    }

    // ========================================================================
    // SeverityDebugFilter
    // ========================================================================

    SeverityDebugFilter::SeverityDebugFilter(GLenum minSeverity)
        : m_minSeverity(minSeverity)
    {
    }

    bool SeverityDebugFilter::shouldFilter(GLenum /*source*/, GLenum /*type*/,
        unsigned int /*id*/, GLenum severity) const {
        return severityToInt(severity) < severityToInt(m_minSeverity);
    }

    int SeverityDebugFilter::severityToInt(GLenum severity) {
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         return 4;
        case GL_DEBUG_SEVERITY_MEDIUM:       return 3;
        case GL_DEBUG_SEVERITY_LOW:          return 2;
        case GL_DEBUG_SEVERITY_NOTIFICATION: return 1;
        default:                             return 0;
        }
    }

} // namespace gl_debug