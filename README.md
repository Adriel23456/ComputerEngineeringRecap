# ComputerScienceEngineeringRecap
Attempt at completing various projects that recap the totality of my learnings this past 5 years of CSE


Proyect Structure:
===============================================================================================================
project-root/
├── include/
│   ├── apps/
│   │   └── cpu_tlp_shared_cache/
│   │       ├── simulation/
│   │       │   ├── assembler/
│   │       │   │   └── Assembler.h
│   │       │   ├── bus/
│   │       │   │   └── interconnect_bus.h
│   │       │   ├── cache/
│   │       │   │   ├── l1_cache.h
│   │       │   │   ├── l1_snoop.h
│   │       │   │   └── l1_utils.h
│   │       │   ├── memory/
│   │       │   │   ├── InstructionMemoryComponent.h
│   │       │   │   └── SharedMemory.h
│   │       │   └── processor/
│   │       │       ├── InterconnectComponent.h
│   │       │       ├── L1Component.h
│   │       │       ├── PE0Component.h
│   │       │       ├── PE1Component.h
│   │       │       ├── PE2Component.h
│   │       │       ├── PE3Component.h
│   │       │       ├── SharedData.h
│   │       │       └── SharedMemoryComponent.h
│   │       ├── ui/
│   │       │   ├── views/
│   │       │   │   ├── processor/
│   │       │   │   │   ├── PE0CPUView.h
│   │       │   │   │   ├── PE0MemView.h
│   │       │   │   │   ├── PE0RegView.h
│   │       │   │   │   ├── PE1CPUView.h
│   │       │   │   │   ├── PE1MemView.h
│   │       │   │   │   ├── PE1RegView.h
│   │       │   │   │   ├── PE2CPUView.h
│   │       │   │   │   ├── PE2MemView.h
│   │       │   │   │   ├── PE2RegView.h
│   │       │   │   │   ├── PE3CPUView.h
│   │       │   │   │   ├── PE3MemView.h
│   │       │   │   │   └── PE3RegView.h
│   │       │   │   ├── AnalysisDataView.h
│   │       │   │   ├── CompilerView.h
│   │       │   │   ├── GeneralView.h
│   │       │   │   ├── ICpuTLPView.h
│   │       │   │   └── RAMView.h
│   │       │   └── widgets/
│   │       │       ├── CacheMemTable.h
│   │       │       ├── InstructionDisassembler.h
│   │       │       ├── Log.h
│   │       │       ├── MemCacheTable.h
│   │       │       ├── RamTable.h
│   │       │       ├── RegTable.h
│   │       │       └── ZoomPanImage.h
│   │       ├── CpuTLPControlAPI.h
│   │       ├── CpuTLPSharedCacheState.h
│   │       └── tlp_debug.h
│   ├── core/
│   │   ├── config/
│   │   │   └── ConfigManager.h
│   │   ├── fsm/
│   │   │   ├── IState.h
│   │   │   ├── State.h
│   │   │   └── StateManager.h
│   │   ├── resources/
│   │   │   └── TextureCache.h
│   │   └── Application.h
│   ├── states/
│   │   ├── MainMenuState.h
│   │   └── ProgramState.h
│   ├── systems/
│   │   └── audio/
│   │       └── AudioManager.h
│   ├── ui/
│   │   └── overlays/
│   │       ├── panels/
│   │       │   ├── AudioSettingsPanel.h
│   │       │   ├── CreditsPanel.h
│   │       │   └── VideoSettingsPanel.h
│   │       └── SettingsOverlay.h
│   └── util/
│       └── errorReporting.h
│
├── src/
│   ├── apps/
│   │   └── cpu_tlp_shared_cache/
│   │       ├── simulation/
│   │       │   ├── assembler/
│   │       │   │   └── Assembler.cpp
│   │       │   ├── bus/
│   │       │   │   └── interconnect_bus.cpp
│   │       │   ├── cache/
│   │       │   │   ├── l1_cache.cpp
│   │       │   │   ├── l1_fsm.cpp
│   │       │   │   ├── l1_snoop.cpp
│   │       │   │   └── l1_utils.cpp
│   │       │   ├── memory/
│   │       │   │   ├── InstructionMemoryComponent.cpp
│   │       │   │   └── SharedMemory.cpp
│   │       │   └── processor/
│   │       │       ├── InterconnectComponent.cpp
│   │       │       ├── L1Component.cpp
│   │       │       ├── PE0Component.cpp
│   │       │       ├── PE1Component.cpp
│   │       │       ├── PE2Component.cpp
│   │       │       ├── PE3Component.cpp
│   │       │       └── SharedMemoryComponent.cpp
│   │       ├── ui/
│   │       │   ├── views/
│   │       │   │   ├── processor/
│   │       │   │   │   ├── PE0CPUView.cpp
│   │       │   │   │   ├── PE0MemView.cpp
│   │       │   │   │   ├── PE0RegView.cpp
│   │       │   │   │   ├── PE1CPUView.cpp
│   │       │   │   │   ├── PE1MemView.cpp
│   │       │   │   │   ├── PE1RegView.cpp
│   │       │   │   │   ├── PE2CPUView.cpp
│   │       │   │   │   ├── PE2MemView.cpp
│   │       │   │   │   ├── PE2RegView.cpp
│   │       │   │   │   ├── PE3CPUView.cpp
│   │       │   │   │   ├── PE3MemView.cpp
│   │       │   │   │   └── PE3RegView.cpp
│   │       │   │   ├── AnalysisDataView.cpp
│   │       │   │   ├── CompilerView.cpp
│   │       │   │   ├── GeneralView.cpp
│   │       │   │   └── RAMView.cpp
│   │       │   └── widgets/
│   │       │       ├── CacheMemTable.cpp
│   │       │       ├── InstructionDisassembler.cpp
│   │       │       ├── MemCacheTable.cpp
│   │       │       ├── RamTable.cpp
│   │       │       ├── RegTable.cpp
│   │       │       └── ZoomPanImage.cpp
│   │       ├── CpuTLPSharedCacheState.cpp
│   │       └── CpuTLPStateFactory.cpp
│   ├── core/
│   │   ├── config/
│   │   │   └── ConfigManager.cpp
│   │   ├── fsm/
│   │   │   ├── State.cpp
│   │   │   └── StateManager.cpp
│   │   ├── resources/
│   │   │   └── TextureCache.cpp
│   │   └── Application.cpp
│   ├── states/
│   │   ├── MainMenuState.cpp
│   │   └── ProgramState.cpp
│   ├── systems/
│   │   └── audio/
│   │       └── AudioManager.cpp
│   ├── ui/
│   │   └── overlays/
│   │       ├── panels/
│   │       │   ├── AudioSettingsPanel.cpp
│   │       │   ├── CreditsPanel.cpp
│   │       │   └── VideoSettingsPanel.cpp
│   │       └── SettingsOverlay.cpp
│   └── main.cpp
===============================================================================================================



Ok decidi que la posicion adecuada de CpuTLPStateFactory.cpp era:

// File: src/apps/cpu_tlp_shared_cache/CpuTLPStateFactory.cpp



Para que lo consideres para tu contexto Y TAMBIEN: CpuTLPSharedCacheState.h NO ESTA EN '/ui/widgets/', si no que, esta en: #include "apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.h"



Ten cuidado con el contexto de archivos de nuestro proyecto, qeu creo qeu te lo habia pasado medio incorrecto, ya lo corregi y el actual es: