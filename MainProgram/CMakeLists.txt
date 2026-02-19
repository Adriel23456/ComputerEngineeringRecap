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
    # ======================= CPU TOMASULO =====================
    # ==========================================================

    # ------------------------- APP ROOT -----------------------
    "src/apps/cpu_tomasulo/CpuTomasuloState.cpp"
    "src/apps/cpu_tomasulo/CpuTomasuloStateFactory.cpp"
    "include/apps/cpu_tomasulo/CpuTomasuloState.h"

    # ==========================================================
    # ========================= SIMULATION =====================
    # ==========================================================

    # ------------------------ ASSEMBLER -----------------------
    "src/apps/cpu_tomasulo/simulation/assembler/TomasuloAssembler.cpp"
    "include/apps/cpu_tomasulo/simulation/assembler/TomasuloAssembler.h"

    # -------------------------- MEMORY ------------------------
    "src/apps/cpu_tomasulo/simulation/memory/TomasuloRAM.cpp"
    "include/apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"

    "src/apps/cpu_tomasulo/simulation/memory/TomasuloRegisterFile.cpp"
    "include/apps/cpu_tomasulo/simulation/memory/TomasuloRegisterFile.h"

    # --------------------------- CORE -------------------------
    "src/apps/cpu_tomasulo/simulation/TomasuloCPU.cpp"
    "src/apps/cpu_tomasulo/simulation/TomasuloSimController.cpp"
    "include/apps/cpu_tomasulo/simulation/TomasuloCPU.h"
    "include/apps/cpu_tomasulo/simulation/TomasuloSimController.h"

    # ==========================================================
    # ========================= PIPELINE =======================
    # ==========================================================

    "include/apps/cpu_tomasulo/simulation/pipeline/TomasuloCycleCoordinator.h"
    "src/apps/cpu_tomasulo/simulation/pipeline/TomasuloCycleCoordinator.cpp"

    # ------------------- pipeline/core ------------------------
    "include/apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

    # ------------------- pipeline/fetch -----------------------
    "src/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_C.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_MUX.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_Adder.cpp"
    "include/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_C.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_MUX.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_Adder.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/fetch/I_Cache.cpp"
    "include/apps/cpu_tomasulo/simulation/pipeline/fetch/I_Cache.h"

    # ------------------- pipeline/decode ----------------------
    "src/apps/cpu_tomasulo/simulation/pipeline/decode/Instruction_Decoder.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/decode/Extends.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/decode/Control_Unit.cpp"
    "include/apps/cpu_tomasulo/simulation/pipeline/decode/Instruction_Decoder.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/decode/Extends.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/decode/Control_Unit.h"

    # ------------------- pipeline/issue -----------------------
    "src/apps/cpu_tomasulo/simulation/pipeline/issue/RegisterFilePipeline.cpp"
    "include/apps/cpu_tomasulo/simulation/pipeline/issue/RegisterFilePipeline.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/issue/ROB.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/issue/Flags_Unit.cpp"
    "include/apps/cpu_tomasulo/simulation/pipeline/issue/ROB.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/issue/Flags_Unit.h"

    # ------------------- pipeline/buffers ---------------------
    "include/apps/cpu_tomasulo/simulation/pipeline/buffers/StoreBuffer.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/buffers/SB0.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/buffers/SB1.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/buffers/LoadBuffer.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/buffers/LB0.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/buffers/LB1.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/buffers/LB2.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/buffers/StoreBuffer.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/buffers/SB0.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/buffers/SB1.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/buffers/LoadBuffer.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/buffers/LB0.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/buffers/LB1.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/buffers/LB2.cpp"

    # --------------- pipeline/reservation ---------------------
    "include/apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU0.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU1.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPALU0.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntMUL0.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPMUL0.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU0.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU1.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPALU0.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntMUL0.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPMUL0.cpp"

    # ------------------- pipeline/agu -------------------------
    "include/apps/cpu_tomasulo/simulation/pipeline/agu/AGU.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_0.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_1.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_Arbiter.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/agu/AGU.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_0.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_1.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_Arbiter.cpp"

    # ----------------- pipeline/execution ---------------------
    "include/apps/cpu_tomasulo/simulation/pipeline/execution/IntALU.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/execution/FPALU.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/execution/IntALU.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/execution/FPALU.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL.cpp"

    "include/apps/cpu_tomasulo/simulation/pipeline/execution/IntALU_Arbiter.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/execution/FPALU_Arbiter.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL_Arbiter.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL_Arbiter.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/execution/IntALU_Arbiter.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/execution/FPALU_Arbiter.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL_Arbiter.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL_Arbiter.cpp"

    # ------------------- pipeline/branch ----------------------
    "include/apps/cpu_tomasulo/simulation/pipeline/branch/RS_Branch0.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Arbiter.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Executor.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/branch/RS_Branch0.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Arbiter.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Executor.cpp"

    # ------------------- pipeline/cdb -------------------------
    "include/apps/cpu_tomasulo/simulation/pipeline/cdb/CDB.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/cdb/CDB_Arbiter.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/cdb/CDB.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/cdb/CDB_Arbiter.cpp"

    # ------------------- pipeline/commit ----------------------
    "include/apps/cpu_tomasulo/simulation/pipeline/commit/Memory_Arbiter.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/commit/D_Cache.h"
    "include/apps/cpu_tomasulo/simulation/pipeline/commit/Commit_Unit.h"

    "src/apps/cpu_tomasulo/simulation/pipeline/commit/Memory_Arbiter.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/commit/D_Cache.cpp"
    "src/apps/cpu_tomasulo/simulation/pipeline/commit/Commit_Unit.cpp"

    # ==========================================================
    # ============================ UI ==========================
    # ==========================================================

    # --------------------------- VIEWS ------------------------
    "src/apps/cpu_tomasulo/ui/views/TomasuloMainView.cpp"
    "src/apps/cpu_tomasulo/ui/views/TomasuloCompilerView.cpp"
    "src/apps/cpu_tomasulo/ui/views/TomasuloRAMView.cpp"
    "src/apps/cpu_tomasulo/ui/views/TomasuloICacheView.cpp"
    "src/apps/cpu_tomasulo/ui/views/TomasuloDCacheView.cpp"
    "src/apps/cpu_tomasulo/ui/views/TomasuloRegistersView.cpp"
    "src/apps/cpu_tomasulo/ui/views/TomasuloROBView.cpp"
    "src/apps/cpu_tomasulo/ui/views/TomasuloAnalysisView.cpp"

    "include/apps/cpu_tomasulo/ui/views/ITomasuloView.h"
    "include/apps/cpu_tomasulo/ui/views/TomasuloMainView.h"
    "include/apps/cpu_tomasulo/ui/views/TomasuloCompilerView.h"
    "include/apps/cpu_tomasulo/ui/views/TomasuloRAMView.h"
    "include/apps/cpu_tomasulo/ui/views/TomasuloICacheView.h"
    "include/apps/cpu_tomasulo/ui/views/TomasuloDCacheView.h"
    "include/apps/cpu_tomasulo/ui/views/TomasuloRegistersView.h"
    "include/apps/cpu_tomasulo/ui/views/TomasuloROBView.h"
    "include/apps/cpu_tomasulo/ui/views/TomasuloAnalysisView.h"

    # -------------------------- WIDGETS -----------------------
    "src/apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.cpp"
    "src/apps/cpu_tomasulo/ui/widgets/CacheTable.cpp"
    "src/apps/cpu_tomasulo/ui/widgets/TomasuloRegTable.cpp"
    "src/apps/cpu_tomasulo/ui/widgets/ROBTable.cpp"

    "include/apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.h"
    "include/apps/cpu_tomasulo/ui/widgets/CacheTable.h"
    "include/apps/cpu_tomasulo/ui/widgets/TomasuloRegTable.h"
    "include/apps/cpu_tomasulo/ui/widgets/ROBTable.h"
    "include/apps/cpu_tomasulo/ui/widgets/TomasuloDisassembler.h"

    # ------------------------- COMPONENTS ---------------------
    "src/apps/cpu_tomasulo/ui/components/Camera2D.cpp"
    "src/apps/cpu_tomasulo/ui/components/ShaderBackground.cpp"
    "src/apps/cpu_tomasulo/ui/components/BorderRenderer.cpp"

    "include/apps/cpu_tomasulo/ui/components/Camera2D.h"
    "include/apps/cpu_tomasulo/ui/components/ShaderBackground.h"
    "include/apps/cpu_tomasulo/ui/components/BorderRenderer.h"
    "include/apps/cpu_tomasulo/ui/components/SlotOverlayRenderer.h"

    # ----------------------- TRACKING -------------------------
    "include/apps/cpu_tomasulo/simulation/PipelineTracker.h"
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
