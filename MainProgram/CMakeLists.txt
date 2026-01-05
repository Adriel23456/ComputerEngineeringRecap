

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

find_package(CUDAToolkit REQUIRED)

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

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} 
    # =========================
    # Entry point
    # =========================
    "src/main.cpp"

    # =========================
    # Core
    # =========================
    "src/core/Application.cpp"

    # =========================
    # Core - FSM
    # =========================
    "src/core/fsm/State.cpp"
    "src/core/fsm/StateManager.cpp"

    # =========================
    # Core - Config
    # =========================
    "src/core/config/ConfigManager.cpp"

    # =========================
    # Core - Resources
    # =========================
    "src/core/resources/TextureCache.cpp"

    # =========================
    # Systems - Audio
    # =========================
    "src/systems/audio/AudioManager.cpp"

    # =========================
    # States
    # =========================
    "src/states/MainMenuState.cpp"
    "src/states/ProgramState.cpp"

    # =========================
    # UI - Overlays
    # =========================
    "src/ui/overlays/SettingsOverlay.cpp"

    # =========================
    # =========================
    # CPU TLP SHARED CACHE APP
    # =========================
    # =========================

    # ---- App root ----
    "src/apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.cpp"
    "src/apps/cpu_tlp_shared_cache/factory.cpp"

    # =========================
    # Simulation - Assembler
    # =========================
    "src/apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.cpp"

    # =========================
    # Simulation - Bus
    # =========================
    "src/apps/cpu_tlp_shared_cache/simulation/bus/interconnect_bus.cpp"

    # =========================
    # Simulation - Cache
    # =========================
    "src/apps/cpu_tlp_shared_cache/simulation/cache/l1_cash.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/l1_fsm.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/l1_snoop.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/cache/l1_utils.cpp"

    # =========================
    # Simulation - Memory
    # =========================
    "src/apps/cpu_tlp_shared_cache/simulation/memory/InstructionMemoryComponent.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.cpp"

    # =========================
    # Simulation - Processor
    # =========================
    "src/apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/L1Component.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/PE0Component.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/PE1Component.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/PE2Component.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/PE3Component.cpp"
    "src/apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.cpp"

    # =========================
    # UI - Views
    # =========================
    "src/apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/CompilerView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/GeneralView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/RAMView.cpp"

    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE0CPUView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE0MemView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE0RegView.cpp"

    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE1CPUView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE1MemView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE1RegView.cpp"

    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE2CPUView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE2MemView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE2RegView.cpp"

    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE3CPUView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE3MemView.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/views/processor/PE3RegView.cpp"

    # =========================
    # UI - Widgets
    # =========================
    "src/apps/cpu_tlp_shared_cache/ui/widgets/CacheMemTable.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/InstructionDisassembler.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/MemCacheTable.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/RamTable.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/RegTable.cpp"
    "src/apps/cpu_tlp_shared_cache/ui/widgets/ZoomPanImage.cpp"


    # ==========================================================
    # ========================= HEADERS ========================
    # ==========================================================

    # ---- Core ----
    "include/core/Application.h"
    "include/core/fsm/IState.h"
    "include/core/fsm/State.h"
    "include/core/fsm/StateManager.h"
    "include/core/config/ConfigManager.h"
    "include/core/resources/TextureCache.h"

    # ---- Systems ----
    "include/systems/audio/AudioManager.h"

    # ---- States ----
    "include/states/MainMenuState.h"
    "include/states/ProgramState.h"

    # ---- UI ----
    "include/ui/overlays/SettingsOverlay.h"

    # ---- Utils ----
    "include/util/errorReporting.h"

    # ---- CPU TLP SHARED CACHE ----
    "include/apps/cpu_tlp_shared_cache/CpuTLPControlAPI.h"
    "include/apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.h"
    "include/apps/cpu_tlp_shared_cache/tlp_debug.h"

    # Simulation - Assembler
    "include/apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.h"

    # Simulation - Bus
    "include/apps/cpu_tlp_shared_cache/simulation/bus/interconnect_bus.h"

    # Simulation - Cache
    "include/apps/cpu_tlp_shared_cache/simulation/cache/l1_cash.h"
    "include/apps/cpu_tlp_shared_cache/simulation/cache/l1_snoop.h"
    "include/apps/cpu_tlp_shared_cache/simulation/cache/l1_utils.h"

    # Simulation - Memory
    "include/apps/cpu_tlp_shared_cache/simulation/memory/InstructionMemoryComponent.h"
    "include/apps/cpu_tlp_shared_cache/simulation/memory/SharedMemory.h"

    # Simulation - Processor
    "include/apps/cpu_tlp_shared_cache/simulation/processor/InterconnectComponent.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/L1Component.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/PE0Component.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/PE1Component.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/PE2Component.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/PE3Component.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
    "include/apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.h"

    # UI - Views
    "include/apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/CompilerView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/GeneralView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/RAMView.h"

    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE0CPUView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE0MemView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE0RegView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE1CPUView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE1MemView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE1RegView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE2CPUView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE2MemView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE2RegView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE3CPUView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE3MemView.h"
    "include/apps/cpu_tlp_shared_cache/ui/views/processor/PE3RegView.h"

    # UI - Widgets
    "include/apps/cpu_tlp_shared_cache/ui/widgets/CacheMemTable.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/InstructionDisassembler.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/Log.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/MemCacheTable.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/RamTable.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/RegTable.h"
    "include/apps/cpu_tlp_shared_cache/ui/widgets/ZoomPanImage.h"
)

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
	#CUDA::cudart
)
