

cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

#! ! ! ! ! ! !
#set this to true to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!
option(PRODUCTION_BUILD "Make this a production build!" OFF)
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!


set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

# Ensure SFML is built as static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static libraries" FORCE)

#for imgui-sfml
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui-docking/imgui")

if(MSVC) 
	add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
endif()

project(mygame LANGUAGES C CXX) 

#Await CUDA support for VS 2026
#CUDA)

#set(CMAKE_CUDA_STANDARD 17)
#set(CMAKE_CUDA_STANDARD_REQUIRED ON)
#set(CMAKE_CUDA_ARCHITECTURES native)

#find_package(CUDAToolkit REQUIRED)

# Enable OpenMP support
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found - Version: ${OpenMP_CXX_VERSION}")
    message(STATUS "OpenMP CXX flags: ${OpenMP_CXX_FLAGS}")
else()
    message(FATAL_ERROR "OpenMP not found! OpenMP is required for this project.")
endif()

#Add SFML, and here you would add other libraries if needed!
add_subdirectory(thirdparty/SFML-2.6.1)
add_subdirectory(thirdparty/imgui-docking)
add_subdirectory(thirdparty/imgui-sfml-2.6.x)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/nlohmann)
add_subdirectory(thirdparty/tinyfiledialogs)


# MY_SOURCES is defined to be a list of all the source files for my game 
# DON'T ADD THE SOURCES BY HAND, they are already added with this macro
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)



if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 

endif()

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
    # =========================== SYSTEMS =====================
    # ==========================================================
    "src/systems/audio/AudioManager.cpp"
    "include/systems/audio/AudioManager.h"

    # ==========================================================
    # ============================ STATES =====================
    # ==========================================================
    "src/states/MainMenuState.cpp"
    "src/states/ProgramState.cpp"
    "include/states/MainMenuState.h"
    "include/states/ProgramState.h"

    # ==========================================================
    # ============================ UI =========================
    # ==========================================================

    # ------------------------ OVERLAYS ------------------------
    "src/ui/overlays/SettingsOverlay.cpp"
    "include/ui/overlays/SettingsOverlay.h"

    # -------------------- OVERLAY PANELS ---------------------
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
    # ========================= SIMULATION ====================
    # ==========================================================

    # ------------------------ ASSEMBLER ----------------------
    "src/apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.h"

    # --------------------------- BUS -------------------------
    "src/apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.h"

    # -------------------------- CACHE ------------------------
    "src/apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/L1FSM.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
    "include/apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.h"
    "include/apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.h"

    # -------------------------- DEBUG ------------------------
    "include/apps/cpu_tlp_shared_cache/simulation/debug/TLPDebug.h"
    "include/apps/cpu_tlp_shared_cache/simulation/debug/TLPDebugHelpers.h"

    # -------------------------- MEMORY -----------------------
    "src/apps/cpu_tlp_shared_cache/simulation/memory/InstructionMemoryComponent.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/memory/InstructionMemoryComponent.h"
    "include/apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.h"

    # ------------------------- PROCESSOR ---------------------
    "src/apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/L1Component.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/PEComponent.cpp"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/L1Component.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/PEComponent.h"

    # -------------------- PROCESSOR - PE ---------------------
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
    # ============================ UI =========================
    # ==========================================================

    # --------------------------- VIEWS -----------------------
    "src/apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/CompilerView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/GeneralView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/RAMView.cpp"
    "include/apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/CompilerView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/GeneralView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/RAMView.h"

    # --------------------- VIEWS - PROCESSOR -----------------
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PECPUView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PERegView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PEMemView.cpp"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PECPUView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PERegView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PEMemView.h"

    # -------------------------- WIDGETS ----------------------
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
    # ============================ UTILS ======================
    # ==========================================================
    "include/util/ErrorReporting.h"

    # ----------------------- GL DEBUG ------------------------
    "src/util/gl_debug/GLDebugOutput.cpp"
    "src/util/gl_debug/GLDebugFilter.cpp"
    "src/util/gl_debug/GLDebugFormatter.cpp"
    "src/util/gl_debug/GLDebugCallback.cpp"
    "include/util/gl_debug/GLDebugOutput.h"
    "include/util/gl_debug/GLDebugFilter.h"
    "include/util/gl_debug/GLDebugFormatter.h"
    "include/util/gl_debug/GLDebugCallback.h"
 "include/apps/quicksort_visualizer/QuicksortVisualizerState.h" "src/apps/quicksort_visualizer/QuicksortVisualizerState.cpp" "src/apps/quicksort_visualizer/QuicksortVisualizerStateFactory.cpp" "include/apps/quicksort_visualizer/threading/LogicThreadController.h" "src/apps/quicksort_visualizer/threading/LogicThreadController.cpp" "include/apps/quicksort_visualizer/visualization/GridConfig.h" "include/apps/quicksort_visualizer/visualization/GridTransform.h" "src/apps/quicksort_visualizer/visualization/GridTransform.cpp" "include/apps/quicksort_visualizer/ui/GridPanel.h" "src/apps/quicksort_visualizer/ui/GridPanel.cpp" "include/apps/quicksort_visualizer/ui/ControlPanel.h" "src/apps/quicksort_visualizer/ui/ControlPanel.cpp" "include/apps/quicksort_visualizer/input/GridInputHandler.h" "src/apps/quicksort_visualizer/input/GridInputHandler.cpp")

if(MSVC) # If using the VS compiler...

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)


	#YOU CAN REMOVE THE CONSOLE WITH THIS LINE! YOU CAN EVEN DO AN IF PRODUCTION_BUILD TO REMOVE IT ONLY IN PRODUCTION BUILDS
	#set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console

endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Link SFML libraries statically, Here wou would add other libraries!
target_link_libraries("${CMAKE_PROJECT_NAME}" 
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
	#CUDA::cudart
)
