cmake_minimum_required(VERSION 3.25) # Bumped to 3.25 for better CUDA 13 support

# ============================================================================
# 1. CUDA PRE-CONFIGURATION (Critical for VS 2026 and CUDA v13.1)
# ============================================================================
set(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v13.1/bin/nvcc.exe")
set(CUDAToolkit_ROOT "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v13.1")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -allow-unsupported-compiler")

# ============================================================================
# 2. PROJECT DEFINITION
# ============================================================================
project(mygame LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES native)

# ============================================================================
# 3. GLOBAL OPTIONS AND LIBRARIES
# ============================================================================
option(PRODUCTION_BUILD "Make this a production build!" OFF)

# Fixed: Single line to handle both runtime profiles
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:Release>")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static libraries" FORCE)
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui-docking/imgui")

if(MSVC)
    # We use a generator expression so /arch:AVX2 only affects C++ and not CUDA
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/arch:AVX2>")
endif()

# Find required packages
find_package(CUDAToolkit REQUIRED)
find_package(OpenMP REQUIRED)

# ============================================================================
# 4. SUBDIRECTORIES (Third-party libraries)
# ============================================================================
add_subdirectory(thirdparty/SFML-2.6.1)
add_subdirectory(thirdparty/imgui-docking)
add_subdirectory(thirdparty/imgui-sfml-2.6.x)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/nlohmann)
add_subdirectory(thirdparty/tinyfiledialogs)

# ============================================================================
# 5. EXECUTABLE CONFIGURATION
# ============================================================================
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}")

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE
    ${MY_SOURCES}

    # ==========================================================
    # ========================= ENTRY POINT ====================
    # ==========================================================
    "src/Main.cpp"

    # ==========================================================
    # ============================ CORE ========================
    # ==========================================================
    "src/core/Application.cpp"
    "include/core/Application.h"

    # ------------------------- FSM ----------------------------
    "src/core/fsm/State.cpp"
    "src/core/fsm/StateManager.cpp"
    "include/core/fsm/IState.h"
    "include/core/fsm/State.h"
    "include/core/fsm/StateManager.h"

    # ------------------------ CONFIG --------------------------
    "src/core/config/ConfigManager.cpp"
    "include/core/config/ConfigManager.h"

    # ----------------------- RESOURCES ------------------------
    "src/core/resources/TextureCache.cpp"
    "include/core/resources/TextureCache.h"

    # ==========================================================
    # =========================== SYSTEMS ======================
    # ==========================================================
    "src/systems/audio/AudioManager.cpp"
    "include/systems/audio/AudioManager.h"

    # ==========================================================
    # ============================ STATES ======================
    # ==========================================================
    "src/states/MainMenuState.cpp"
    "src/states/ProgramState.cpp"
    "include/states/MainMenuState.h"
    "include/states/ProgramState.h"

    # ==========================================================
    # ============================ UI ==========================
    # ==========================================================

    # ------------------------ OVERLAYS ------------------------
    "src/ui/overlays/SettingsOverlay.cpp"
    "include/ui/overlays/SettingsOverlay.h"

    # -------------------- OVERLAY PANELS ----------------------
    "src/ui/overlays/panels/VideoSettingsPanel.cpp"
    "src/ui/overlays/panels/AudioSettingsPanel.cpp"
    "src/ui/overlays/panels/CreditsPanel.cpp"
    "include/ui/overlays/panels/VideoSettingsPanel.h"
    "include/ui/overlays/panels/AudioSettingsPanel.h"
    "include/ui/overlays/panels/CreditsPanel.h"

    # ==========================================================
    # ============ CPU TLP SHARED CACHE APPLICATION ============
    # ==========================================================

    # ------------------------- APP ROOT -----------------------
    "src/apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.cpp"
    "src/apps/cpu_tlp_shared_cache/CpuTLPStateFactory.cpp"
    "include/apps/cpu_tlp_shared_cache/CpuTLPControlAPI.h"
    "include/apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.h"

    # ==========================================================
    # ========================= SIMULATION =====================
    # ==========================================================

    # ------------------------ ASSEMBLER -----------------------
    "src/apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.h"

    # --------------------------- BUS --------------------------
    "src/apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.h"

    # -------------------------- CACHE -------------------------
    "src/apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/L1FSM.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
    "include/apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.h"
    "include/apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.h"

    # -------------------------- DEBUG -------------------------
    "include/apps/cpu_tlp_shared_cache/simulation/debug/TLPDebug.h"
    "include/apps/cpu_tlp_shared_cache/simulation/debug/TLPDebugHelpers.h"

    # -------------------------- MEMORY ------------------------
    "src/apps/cpu_tlp_shared_cache/simulation/memory/InstructionMemoryComponent.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/memory/InstructionMemoryComponent.h"
    "include/apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.h"

    # ------------------------- PROCESSOR ----------------------
    "src/apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/L1Component.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/PEComponent.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/L1Component.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/PEComponent.h"

    # -------------------- PROCESSOR - PE ----------------------
    "src/apps/cpu_tlp_shared_cache/simulation/processor/pe/RegisterFile.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/pe/ALU.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/pe/ControlUnit.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/pe/HazardUnit.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/pe/PipelineRegisters.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/pe/RegisterFile.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/pe/ALU.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/pe/ControlUnit.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/pe/HazardUnit.h"

    # ==========================================================
    # ============================ UI ==========================
    # ==========================================================

    # --------------------------- VIEWS ------------------------
    "src/apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/CompilerView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/GeneralView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/RAMView.cpp"
    "include/apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/CompilerView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/GeneralView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/RAMView.h"

    # --------------------- VIEWS - PROCESSOR ------------------
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PECPUView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PERegView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PEMemView.cpp"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PECPUView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PERegView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PEMemView.h"

    # -------------------------- WIDGETS -----------------------
    "src/apps/cpu_tlp_shared_cache/ui/widgets/CacheMemTable.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/InstructionDisassembler.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/MemCacheTable.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/RamTable.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/RegTable.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/ZoomPanImage.cpp"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/CacheMemTable.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/InstructionDisassembler.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/Log.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/MemCacheTable.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/RamTable.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/RegTable.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/ZoomPanImage.h"

    # ==========================================================
    # ============================ UTILS =======================
    # ==========================================================
    "include/util/ErrorReporting.h"

    # ----------------------- GL DEBUG -------------------------
    "src/util/gl_debug/GLDebugOutput.cpp"
    "src/util/gl_debug/GLDebugFilter.cpp"
    "src/util/gl_debug/GLDebugFormatter.cpp"
    "src/util/gl_debug/GLDebugCallback.cpp"
    "include/util/gl_debug/GLDebugOutput.h"
    "include/util/gl_debug/GLDebugFilter.h"
    "include/util/gl_debug/GLDebugFormatter.h"
    "include/util/gl_debug/GLDebugCallback.h"

    # ==========================================================
    # ============ QUICKSORT VISUALIZER APPLICATION ============
    # ==========================================================

    # ------------------------- APP ROOT -----------------------
    "src/apps/quicksort_visualizer/QuicksortVisualizerState.cpp"
    "src/apps/quicksort_visualizer/QuicksortVisualizerStateFactory.cpp"
    "include/apps/quicksort_visualizer/QuicksortVisualizerState.h"

    # ----------------------- THREADING ------------------------
    "src/apps/quicksort_visualizer/threading/LogicThreadController.cpp"
    "include/apps/quicksort_visualizer/threading/LogicThreadController.h"

    # -------------------- VISUALIZATION CORE ------------------
    "include/apps/quicksort_visualizer/visualization/GridConfig.h"
    "src/apps/quicksort_visualizer/visualization/GridTransform.cpp"
    "include/apps/quicksort_visualizer/visualization/GridTransform.h"

    # --------------------------- INPUT ------------------------
    "src/apps/quicksort_visualizer/input/GridInputHandler.cpp"
    "include/apps/quicksort_visualizer/input/GridInputHandler.h"

    # ---------------------------- DATA ------------------------
    "include/apps/quicksort_visualizer/data/SortElement.h"
    "src/apps/quicksort_visualizer/data/ElementCollection.cpp"
    "include/apps/quicksort_visualizer/data/ElementCollection.h"
    "include/apps/quicksort_visualizer/data/SwapOperation.h"
    "src/apps/quicksort_visualizer/data/SwapQueue.cpp"
    "include/apps/quicksort_visualizer/data/SwapQueue.h"

    # -------------------------- ALGORITHM ---------------------
    "src/apps/quicksort_visualizer/algorithm/QuicksortAlgorithm.cpp"
    "include/apps/quicksort_visualizer/algorithm/QuicksortAlgorithm.h"

    # ------------------------- ANIMATION ----------------------
    "src/apps/quicksort_visualizer/animation/SwapAnimator.cpp"
    "include/apps/quicksort_visualizer/animation/SwapAnimator.h"

    # ---------------------------- AUDIO -----------------------
    "src/apps/quicksort_visualizer/audio/SwapSoundGenerator.cpp"
    "include/apps/quicksort_visualizer/audio/SwapSoundGenerator.h"

    # ----------------------------- UI -------------------------
    "src/apps/quicksort_visualizer/ui/GridPanel.cpp"
    "include/apps/quicksort_visualizer/ui/GridPanel.h"
    "src/apps/quicksort_visualizer/ui/ControlPanel.cpp"
    "include/apps/quicksort_visualizer/ui/ControlPanel.h"
    "src/apps/quicksort_visualizer/ui/ElementRenderer.cpp"
    "include/apps/quicksort_visualizer/ui/ElementRenderer.h"
    "src/apps/quicksort_visualizer/ui/AmountInputPopup.cpp"
    "include/apps/quicksort_visualizer/ui/AmountInputPopup.h"

    # ----------------------------- CUDA ------------------------
    "src/cuda/CudaTest.cu"
    "include/cuda/CudaTest.cuh"

    # ==========================================================
    # ===================== CPU TOMASULO UI ====================
    # ==========================================================
    "include/apps/cpu_tomasulo/ui/views/ITomasuloView.h"
    "include/apps/cpu_tomasulo/CpuTomasuloState.h"
    "src/apps/cpu_tomasulo/CpuTomasuloState.cpp"
    "src/apps/cpu_tomasulo/CpuTomasuloStateFactory.cpp"
    "include/apps/cpu_tomasulo/ui/views/TomasuloMainView.h"
    "src/apps/cpu_tomasulo/ui/views/TomasuloMainView.cpp"
    "include/apps/cpu_tomasulo/ui/views/TomasuloCompilerView.h"
    "src/apps/cpu_tomasulo/ui/views/TomasuloCompilerView.cpp"
)

# Build definitions depending on build type
if(PRODUCTION_BUILD)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1)
else()
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)
endif()

if(MSVC)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

# ============================================================================
# 6. INCLUDES AND FINAL LINKING
# ============================================================================
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_link_libraries("${CMAKE_PROJECT_NAME}"
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
    imgui
    ImGui-SFML::ImGui-SFML
    glad
    nlohmann
    tinyfiledialogs
    OpenMP::OpenMP_CXX
    CUDA::cudart
)
