

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

project(mygame)




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
	"src/main.cpp"
	
	"src/core/State.cpp"
	"src/core/StateManager.cpp"

	"src/programs/cpu_tlp_shared_cache/CpuTLPSharedCacheState.cpp"
	"src/programs/cpu_tlp_shared_cache/factory.cpp"
	"src/programs/cpu_tlp_shared_cache/components/Assembler.cpp"
	"src/programs/cpu_tlp_shared_cache/components/InstructionMemoryComponent.cpp"
	"src/programs/cpu_tlp_shared_cache/components/PE0Component.cpp"
	"src/programs/cpu_tlp_shared_cache/components/PE1Component.cpp"
	"src/programs/cpu_tlp_shared_cache/components/PE2Component.cpp"
	"src/programs/cpu_tlp_shared_cache/components/PE3Component.cpp"
	"src/programs/cpu_tlp_shared_cache/components/SharedMemory.cpp"
	"src/programs/cpu_tlp_shared_cache/components/SharedMemoryComponent.cpp"
	"src/programs/cpu_tlp_shared_cache/components/bus/InterconnectComponent.cpp"
	"src/programs/cpu_tlp_shared_cache/components/bus/interconnect_bus.cpp"
	"src/programs/cpu_tlp_shared_cache/components/cash/L1Component.cpp"
	"src/programs/cpu_tlp_shared_cache/components/cash/l1_cash.cpp"
	"src/programs/cpu_tlp_shared_cache/components/cash/l1_fsm.cpp"
	"src/programs/cpu_tlp_shared_cache/components/cash/l1_snoop.cpp"
	"src/programs/cpu_tlp_shared_cache/components/cash/l1_utils.cpp"
	"src/programs/cpu_tlp_shared_cache/utils/TextureCache.cpp"
	"src/programs/cpu_tlp_shared_cache/views/AnalysisDataView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/CompilerView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/GeneralView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE0CPUView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE0MemView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE0RegView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE1CPUView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE1MemView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE1RegView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE2CPUView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE2MemView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE2RegView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE3CPUView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE3MemView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/PE3RegView.cpp"
	"src/programs/cpu_tlp_shared_cache/views/RAMView.cpp"
	"src/programs/cpu_tlp_shared_cache/widgets/CacheMemTable.cpp"
	"src/programs/cpu_tlp_shared_cache/widgets/InstructionDisassembler.cpp"
	"src/programs/cpu_tlp_shared_cache/widgets/MemCacheTable.cpp"
	"src/programs/cpu_tlp_shared_cache/widgets/RamTable.cpp"
	"src/programs/cpu_tlp_shared_cache/widgets/RegTable.cpp"
	"src/programs/cpu_tlp_shared_cache/widgets/ZoomPanImage.cpp"

	"src/states/MainMenuState.cpp"
	"src/states/ProgramState.cpp"

	"src/systems/AudioManager.cpp"
	"src/systems/ConfigManager.cpp"

	"src/ui/SettingsOverlay.cpp"

	
	
	
	
	
	
	
	
	"include/ui/SettingsOverlay.h"

	"include/core/State.h"
	"include/core/StateManager.h"

	"include/programs/cpu_tlp_shared_cache/CpuTLPControlAPI.h"
	"include/programs/cpu_tlp_shared_cache/CpuTLPSharedCacheState.h"
	"include/programs/cpu_tlp_shared_cache/tlp_debug.h"
	"include/programs/cpu_tlp_shared_cache/components/Assembler.h"
	"include/programs/cpu_tlp_shared_cache/components/InstructionMemoryComponent.h"
	"include/programs/cpu_tlp_shared_cache/components/PE0Component.h"
	"include/programs/cpu_tlp_shared_cache/components/PE1Component.h"
	"include/programs/cpu_tlp_shared_cache/components/PE2Component.h"
	"include/programs/cpu_tlp_shared_cache/components/PE3Component.h"
	"include/programs/cpu_tlp_shared_cache/components/SharedData.h"
	"include/programs/cpu_tlp_shared_cache/components/SharedMemory.h"
	"include/programs/cpu_tlp_shared_cache/components/SharedMemoryComponent.h"
	"include/programs/cpu_tlp_shared_cache/components/bus/InterconnectComponent.h"
	"include/programs/cpu_tlp_shared_cache/components/bus/interconnect_bus.h"
	"include/programs/cpu_tlp_shared_cache/components/cash/L1Component.h"
	"include/programs/cpu_tlp_shared_cache/components/cash/l1_cash.h"
	"include/programs/cpu_tlp_shared_cache/components/cash/l1_snoop.h"
	"include/programs/cpu_tlp_shared_cache/components/cash/l1_utils.h"
	"include/programs/cpu_tlp_shared_cache/utils/TextureCache.h"
	"include/programs/cpu_tlp_shared_cache/views/AnalysisDataView.h"
	"include/programs/cpu_tlp_shared_cache/views/CompilerView.h"
	"include/programs/cpu_tlp_shared_cache/views/GeneralView.h"
	"include/programs/cpu_tlp_shared_cache/views/ICpuTLPView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE0CPUView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE0MemView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE0RegView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE1CPUView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE1MemView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE1RegView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE2CPUView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE2MemView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE2RegView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE3CPUView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE3MemView.h"
	"include/programs/cpu_tlp_shared_cache/views/PE3RegView.h"
	"include/programs/cpu_tlp_shared_cache/views/RAMView.h"
	"include/programs/cpu_tlp_shared_cache/widgets/CacheMemTable.h"
	"include/programs/cpu_tlp_shared_cache/widgets/InstructionDisassembler.h"
	"include/programs/cpu_tlp_shared_cache/widgets/Log.h"
	"include/programs/cpu_tlp_shared_cache/widgets/MemCacheTable.h"
	"include/programs/cpu_tlp_shared_cache/widgets/RamTable.h"
	"include/programs/cpu_tlp_shared_cache/widgets/RegTable.h"
	"include/programs/cpu_tlp_shared_cache/widgets/ZoomPanImage.h"

	"include/states/MainMenuState.h"
	"include/states/ProgramState.h"

	"include/systems/AudioManager.h"
	"include/systems/ConfigManager.h"

	"include/util/errorReporting.h"
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
)
