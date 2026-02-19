// ============================================================================
// File: src/Main.cpp
// ============================================================================

/**
 * @file Main.cpp
 * @brief Application entry point.
 *
 * Following SRP, main() only:
 * 1. Sets up platform-specific console (Windows)
 * 2. Creates and runs the Application
 *
 * All application logic is delegated to the Application class.
 */

#include "core/Application.h"
#include <iostream>
#include "cuda/CudaTest.cuh"

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

 /** @brief Path to application configuration file. */
static constexpr const char* CONFIG_PATH = RESOURCES_PATH "config.json";

/**
 * @brief Enables console output on Windows for debugging.
 *
 * Windows GUI applications don't have a console by default.
 * This function creates one and redirects stdout/stderr to it.
 */
static void enableConsole() {
#ifdef _WIN32
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    std::cout.clear();
    std::cerr.clear();
#endif
}

/**
 * @brief Application entry point.
 * @return Exit code (0 for success, non-zero for errors).
 */
int main() {
    enableConsole();

    runCudaTest();   // GPU proof

    Application app(CONFIG_PATH);

    if (!app.initialize()) {
        return -1;
    }

    return app.run();
}