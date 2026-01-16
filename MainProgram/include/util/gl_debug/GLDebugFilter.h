// ============================================================================
// File: include/util/gl_debug/GLDebugFilter.h
// ============================================================================

#pragma once

/**
 * @file GLDebugFilter.h
 * @brief Filter interface and implementations for OpenGL debug messages.
 *
 * Determines which debug messages should be processed or ignored.
 *
 * @note Follows:
 *   - SRP: Only handles message filtering logic
 *   - OCP: New filters can be added without modifying existing code
 *   - DIP: Clients depend on IGLDebugFilter abstraction
 */

#include <glad/glad.h>
#include <vector>
#include <cstdint>

namespace gl_debug {

    /**
     * @class IGLDebugFilter
     * @brief Abstract interface for debug message filtering.
     */
    class IGLDebugFilter {
    public:
        virtual ~IGLDebugFilter() = default;

        /**
         * @brief Determines if a message should be filtered (ignored).
         * @param source Message source.
         * @param type Message type.
         * @param id Message ID.
         * @param severity Message severity.
         * @return true if message should be ignored, false to process it.
         */
        virtual bool shouldFilter(GLenum source, GLenum type,
            unsigned int id, GLenum severity) const = 0;
    };

    /**
     * @class DefaultDebugFilter
     * @brief Default filter that ignores non-significant messages.
     *
     * Filters out:
     * - Known non-significant error IDs
     * - Performance messages
     */
    class DefaultDebugFilter : public IGLDebugFilter {
    public:
        DefaultDebugFilter();

        bool shouldFilter(GLenum source, GLenum type,
            unsigned int id, GLenum severity) const override;

        /**
         * @brief Adds an ID to the ignore list.
         * @param id Message ID to ignore.
         */
        void addIgnoredId(unsigned int id);

        /**
         * @brief Sets whether to filter performance messages.
         * @param filter true to filter performance messages.
         */
        void setFilterPerformance(bool filter);

    private:
        std::vector<unsigned int> m_ignoredIds;
        bool m_filterPerformance = true;
    };

    /**
     * @class SeverityDebugFilter
     * @brief Filters messages below a minimum severity level.
     */
    class SeverityDebugFilter : public IGLDebugFilter {
    public:
        /**
         * @brief Constructs filter with minimum severity.
         * @param minSeverity Minimum severity to allow (GL_DEBUG_SEVERITY_*).
         */
        explicit SeverityDebugFilter(GLenum minSeverity);

        bool shouldFilter(GLenum source, GLenum type,
            unsigned int id, GLenum severity) const override;

    private:
        GLenum m_minSeverity;

        static int severityToInt(GLenum severity);
    };

} // namespace gl_debug