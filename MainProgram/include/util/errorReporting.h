// ============================================================================
// File: include/util/ErrorReporting.h
// ============================================================================

#pragma once

/**
 * @file ErrorReporting.h
 * @brief Simplified facade for OpenGL debug error reporting.
 *
 * Provides backward-compatible API while using SOLID-compliant
 * internal implementation.
 *
 * @note Follows:
 *   - Facade Pattern: Simplifies access to gl_debug subsystem
 *   - Backward Compatibility: Maintains original function names
 */

#include "util/gl_debug/GLDebugCallback.h"

 /**
  * @brief Enables OpenGL debug error reporting with default configuration.
  *
  * Uses:
  * - DefaultDebugFilter (filters non-significant messages)
  * - DetailedDebugFormatter (full message details)
  * - ConsoleDebugOutput (stdout)
  */
inline void enableReportGlErrors() {
    gl_debug::GLDebugCallback::instance().enable();
}

/**
 * @brief Disables OpenGL debug error reporting.
 */
inline void disableReportGlErrors() {
    gl_debug::GLDebugCallback::instance().disable();
}

/**
 * @brief Enables OpenGL debug with compact single-line format.
 */
inline void enableReportGlErrorsCompact() {
    auto& callback = gl_debug::GLDebugCallback::instance();
    callback.setFormatter(std::make_unique<gl_debug::CompactDebugFormatter>());
    callback.enable();
}

/**
 * @brief Enables OpenGL debug filtering only high severity errors.
 */
inline void enableReportGlErrorsHighOnly() {
    auto& callback = gl_debug::GLDebugCallback::instance();
    callback.setFilter(std::make_unique<gl_debug::SeverityDebugFilter>(GL_DEBUG_SEVERITY_HIGH));
    callback.enable();
}

/**
 * @brief Gets reference to debug callback for advanced configuration.
 * @return Reference to GLDebugCallback singleton.
 */
inline gl_debug::GLDebugCallback& getGLDebugCallback() {
    return gl_debug::GLDebugCallback::instance();
}