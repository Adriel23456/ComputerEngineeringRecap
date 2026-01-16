// ============================================================================
// File: src/util/gl_debug/GLDebugFormatter.cpp
// ============================================================================

/**
 * @file GLDebugFormatter.cpp
 * @brief Implementation of GLDebugFormatter classes.
 */

#include "util/gl_debug/GLDebugFormatter.h"
#include <sstream>

namespace gl_debug {

    // ========================================================================
    // DetailedDebugFormatter
    // ========================================================================

    std::string DetailedDebugFormatter::format(GLenum source, GLenum type, unsigned int id,
        GLenum severity, const char* message) const {
        std::ostringstream oss;
        oss << "---------------\n"
            << "Debug message (" << id << "): " << message << "\n"
            << "Source: " << sourceToString(source) << "\n"
            << "Type: " << typeToString(type) << "\n"
            << "Severity: " << severityToString(severity) << "\n";
        return oss.str();
    }

    std::string DetailedDebugFormatter::sourceToString(GLenum source) {
        switch (source) {
        case GL_DEBUG_SOURCE_API:             return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
        case GL_DEBUG_SOURCE_OTHER:           return "Other";
        default:                              return "Unknown";
        }
    }

    std::string DetailedDebugFormatter::typeToString(GLenum type) {
        switch (type) {
        case GL_DEBUG_TYPE_ERROR:               return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour";
        case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
        case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
        case GL_DEBUG_TYPE_MARKER:              return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
        case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
        case GL_DEBUG_TYPE_OTHER:               return "Other";
        default:                                return "Unknown";
        }
    }

    std::string DetailedDebugFormatter::severityToString(GLenum severity) {
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         return "High";
        case GL_DEBUG_SEVERITY_MEDIUM:       return "Medium";
        case GL_DEBUG_SEVERITY_LOW:          return "Low";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
        default:                             return "Unknown";
        }
    }

    // ========================================================================
    // CompactDebugFormatter
    // ========================================================================

    std::string CompactDebugFormatter::format(GLenum /*source*/, GLenum type, unsigned int id,
        GLenum severity, const char* message) const {
        std::ostringstream oss;
        oss << "[GL:" << severityToChar(severity) << "] "
            << typeToShortString(type) << " (" << id << "): " << message;
        return oss.str();
    }

    char CompactDebugFormatter::severityToChar(GLenum severity) {
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         return 'H';
        case GL_DEBUG_SEVERITY_MEDIUM:       return 'M';
        case GL_DEBUG_SEVERITY_LOW:          return 'L';
        case GL_DEBUG_SEVERITY_NOTIFICATION: return 'N';
        default:                             return '?';
        }
    }

    std::string CompactDebugFormatter::typeToShortString(GLenum type) {
        switch (type) {
        case GL_DEBUG_TYPE_ERROR:               return "ERR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEP";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UND";
        case GL_DEBUG_TYPE_PORTABILITY:         return "PRT";
        case GL_DEBUG_TYPE_PERFORMANCE:         return "PRF";
        case GL_DEBUG_TYPE_MARKER:              return "MRK";
        case GL_DEBUG_TYPE_PUSH_GROUP:          return "PSH";
        case GL_DEBUG_TYPE_POP_GROUP:           return "POP";
        case GL_DEBUG_TYPE_OTHER:               return "OTH";
        default:                                return "???";
        }
    }

} // namespace gl_debug