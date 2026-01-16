// ============================================================================
// File: src/util/gl_debug/GLDebugOutput.cpp
// ============================================================================

/**
 * @file GLDebugOutput.cpp
 * @brief Implementation of GLDebugOutput classes.
 */

#include "util/gl_debug/GLDebugOutput.h"
#include <iostream>

namespace gl_debug {

    void ConsoleDebugOutput::write(const std::string& message) {
        std::cout << message << std::endl;
    }

} // namespace gl_debug