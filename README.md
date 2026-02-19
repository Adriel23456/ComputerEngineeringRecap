# Computer Engineering Recap (CE Recap)

A **comprehensive C++ recap project** that consolidates and reimplements the core concepts, systems, and architectural patterns studied throughout a **Computer Engineering (CE)** degree.

This repository is designed as a **modular, extensible simulation framework** built with **C++17**, **CMake**, and **Visual Studio 2026**, following modern software engineering practices such as **MVCC**, strict separation of concerns, and clean architectural layering.

The long-term objective is to serve as:

- A **technical portfolio**
- A **knowledge consolidation project**
- A **base platform** for experimenting with advanced computer architecture, algorithms, and visualization systems

---

## 🚀 Current & Planned Projects

### Implemented / In Progress

- **TLP CPU Simulator with Shared Cache**
  - Multi-core CPU simulation
  - 2-way set associative shared L1 cache
  - Instruction pipeline, hazards, snooping, and interconnect bus
  - Full graphical UI with real-time visualization and inspection

- **Tomasulo Algorithm CPU Simulator** ✅
  - Full out-of-order execution datapath simulation
  - Reorder Buffer (ROB), Reservation Stations, CDB broadcast
  - Integer ALU (×2), FP ALU, Integer MUL/DIV, FP MUL/DIV/SQRT execution units
  - Load/Store Buffers with AGU and Memory Arbiter
  - Speculative branch execution with misprediction recovery and flush
  - 4-way set-associative Instruction Cache with prefetch + Data Cache
  - In-order commit with architectural register file and flags unit
  - Custom assembler and binary loader
  - Full graphical UI: pipeline view, compiler, RAM, I-Cache, D-Cache, registers, ROB, and data analysis panels
  - Background worker thread with async step/run/stop control

- **Quicksort Visualizer** ✅
  - Step-by-step algorithm visualization
  - Performance and partitioning analysis
  - Animated swap rendering with sound generation
  - Interactive element count input and real-time control

### Planned

- **Additional Computer Architecture & Algorithms Projects**
  - To be announced

---

## 🛠️ Technology Stack

- **Language:** C++17  
- **Build System:** CMake (≥ 3.16)  
- **Compiler / IDE:** Visual Studio 2026  

### Graphics & UI
- SFML (statically linked)
- ImGui (Docking branch)
- ImGui-SFML

### Utilities & Dependencies
- GLAD
- nlohmann/json
- tinyfiledialogs

### Architectural Principles
- Modular app-based design
- State-driven application core
- MVCC-inspired data handling
- Strict separation between **core engine**, **simulation logic**, and **UI**

---

## 📁 Project Structure

The structure below reflects the **actual layout enforced by `CMakeLists.txt`** and represents the authoritative organization of the codebase.

```
project-root/
├── CMakeLists.txt
│
├── include/
│   ├── core/
│   │   ├── Application.h
│   │   ├── config/
│   │   │   └── ConfigManager.h
│   │   ├── fsm/
│   │   │   ├── IState.h
│   │   │   ├── State.h
│   │   │   └── StateManager.h
│   │   └── resources/
│   │       └── TextureCache.h
│   │
│   ├── systems/
│   │   └── audio/
│   │       └── AudioManager.h
│   │
│   ├── states/
│   │   ├── MainMenuState.h
│   │   └── ProgramState.h
│   │
│   ├── ui/
│   │   └── overlays/
│   │       ├── SettingsOverlay.h
│   │       └── panels/
│   │           ├── VideoSettingsPanel.h
│   │           ├── AudioSettingsPanel.h
│   │           └── CreditsPanel.h
│   │
│   ├── apps/
│   │   ├── cpu_tlp_shared_cache/
│   │   │   ├── CpuTLPControlAPI.h
│   │   │   ├── CpuTLPSharedCacheState.h
│   │   │   ├── simulation/
│   │   │   │   ├── assembler/
│   │   │   │   │   └── Assembler.h
│   │   │   │   ├── bus/
│   │   │   │   │   └── InterconnectBus.h
│   │   │   │   ├── cache/
│   │   │   │   │   ├── L1Cache.h
│   │   │   │   │   ├── L1Snoop.h
│   │   │   │   │   └── L1Utils.h
│   │   │   │   ├── debug/
│   │   │   │   │   ├── TLPDebug.h
│   │   │   │   │   └── TLPDebugHelpers.h
│   │   │   │   ├── memory/
│   │   │   │   │   ├── InstructionMemoryComponent.h
│   │   │   │   │   └── SharedMemory.h
│   │   │   │   └── processor/
│   │   │   │       ├── InterconnectComponent.h
│   │   │   │       ├── L1Component.h
│   │   │   │       ├── SharedMemoryComponent.h
│   │   │   │       ├── PEComponent.h
│   │   │   │       └── pe/
│   │   │   │           ├── PipelineRegisters.h
│   │   │   │           ├── RegisterFile.h
│   │   │   │           ├── ALU.h
│   │   │   │           ├── ControlUnit.h
│   │   │   │           └── HazardUnit.h
│   │   │   └── ui/
│   │   │       ├── views/
│   │   │       │   ├── ICpuTLPView.h
│   │   │       │   ├── GeneralView.h
│   │   │       │   ├── CompilerView.h
│   │   │       │   ├── RAMView.h
│   │   │       │   ├── AnalysisDataView.h
│   │   │       │   └── processor/
│   │   │       │       ├── PECPUView.h
│   │   │       │       ├── PERegView.h
│   │   │       │       └── PEMemView.h
│   │   │       └── widgets/
│   │   │           ├── CacheMemTable.h
│   │   │           ├── MemCacheTable.h
│   │   │           ├── RamTable.h
│   │   │           ├── RegTable.h
│   │   │           ├── InstructionDisassembler.h
│   │   │           ├── ZoomPanImage.h
│   │   │           └── Log.h
│   │   │
│   │   ├── cpu_tomasulo/
│   │   │   ├── CpuTomasuloState.h
│   │   │   ├── simulation/
│   │   │   │   ├── TomasuloCPU.h
│   │   │   │   ├── TomasuloSimController.h
│   │   │   │   ├── PipelineTracker.h
│   │   │   │   ├── assembler/
│   │   │   │   │   └── TomasuloAssembler.h
│   │   │   │   ├── memory/
│   │   │   │   │   ├── TomasuloRAM.h
│   │   │   │   │   └── TomasuloRegisterFile.h
│   │   │   │   └── pipeline/
│   │   │   │       ├── TomasuloCycleCoordinator.h
│   │   │   │       ├── core/
│   │   │   │       │   ├── ITomasuloComponent.h
│   │   │   │       │   └── TomasuloBus.h
│   │   │   │       ├── fetch/
│   │   │   │       │   ├── PC_C.h
│   │   │   │       │   ├── PC_MUX.h
│   │   │   │       │   ├── PC_Adder.h
│   │   │   │       │   └── I_Cache.h
│   │   │   │       ├── decode/
│   │   │   │       │   ├── Instruction_Decoder.h
│   │   │   │       │   ├── Extends.h
│   │   │   │       │   └── Control_Unit.h
│   │   │   │       ├── issue/
│   │   │   │       │   ├── RegisterFilePipeline.h
│   │   │   │       │   ├── ROB.h
│   │   │   │       │   └── Flags_Unit.h
│   │   │   │       ├── buffers/
│   │   │   │       │   ├── StoreBuffer.h
│   │   │   │       │   ├── SB0.h
│   │   │   │       │   ├── SB1.h
│   │   │   │       │   ├── LoadBuffer.h
│   │   │   │       │   ├── LB0.h
│   │   │   │       │   ├── LB1.h
│   │   │   │       │   └── LB2.h
│   │   │   │       ├── reservation/
│   │   │   │       │   ├── ReservationStation.h
│   │   │   │       │   ├── RS_IntALU0.h
│   │   │   │       │   ├── RS_IntALU1.h
│   │   │   │       │   ├── RS_FPALU0.h
│   │   │   │       │   ├── RS_IntMUL0.h
│   │   │   │       │   └── RS_FPMUL0.h
│   │   │   │       ├── agu/
│   │   │   │       │   ├── AGU.h
│   │   │   │       │   ├── AGU_0.h
│   │   │   │       │   ├── AGU_1.h
│   │   │   │       │   └── AGU_Arbiter.h
│   │   │   │       ├── execution/
│   │   │   │       │   ├── IntALU.h
│   │   │   │       │   ├── FPALU.h
│   │   │   │       │   ├── IntMUL.h
│   │   │   │       │   ├── FPMUL.h
│   │   │   │       │   ├── IntALU_Arbiter.h
│   │   │   │       │   ├── FPALU_Arbiter.h
│   │   │   │       │   ├── IntMUL_Arbiter.h
│   │   │   │       │   └── FPMUL_Arbiter.h
│   │   │   │       ├── branch/
│   │   │   │       │   ├── RS_Branch0.h
│   │   │   │       │   ├── Branch_Arbiter.h
│   │   │   │       │   └── Branch_Executor.h
│   │   │   │       ├── cdb/
│   │   │   │       │   ├── CDB.h
│   │   │   │       │   └── CDB_Arbiter.h
│   │   │   │       └── commit/
│   │   │   │           ├── Memory_Arbiter.h
│   │   │   │           ├── D_Cache.h
│   │   │   │           └── Commit_Unit.h
│   │   │   └── ui/
│   │   │       ├── views/
│   │   │       │   ├── ITomasuloView.h
│   │   │       │   ├── TomasuloMainView.h
│   │   │       │   ├── TomasuloCompilerView.h
│   │   │       │   ├── TomasuloRAMView.h
│   │   │       │   ├── TomasuloICacheView.h
│   │   │       │   ├── TomasuloDCacheView.h
│   │   │       │   ├── TomasuloRegistersView.h
│   │   │       │   ├── TomasuloROBView.h
│   │   │       │   └── TomasuloAnalysisView.h
│   │   │       ├── widgets/
│   │   │       │   ├── TomasuloRamTable.h
│   │   │       │   ├── CacheTable.h
│   │   │       │   ├── TomasuloRegTable.h
│   │   │       │   ├── ROBTable.h
│   │   │       │   └── TomasuloDisassembler.h
│   │   │       └── components/
│   │   │           ├── Camera2D.h
│   │   │           ├── ShaderBackground.h
│   │   │           ├── BorderRenderer.h
│   │   │           └── SlotOverlayRenderer.h
│   │   │
│   │   └── quicksort_visualizer/
│   │       ├── QuicksortVisualizerState.h
│   │       ├── algorithm/
│   │       │   └── QuicksortAlgorithm.h
│   │       ├── animation/
│   │       │   └── SwapAnimator.h
│   │       ├── audio/
│   │       │   └── SwapSoundGenerator.h
│   │       ├── data/
│   │       │   ├── SortElement.h
│   │       │   ├── ElementCollection.h
│   │       │   ├── SwapOperation.h
│   │       │   └── SwapQueue.h
│   │       ├── input/
│   │       │   └── GridInputHandler.h
│   │       ├── threading/
│   │       │   └── LogicThreadController.h
│   │       ├── visualization/
│   │       │   ├── GridConfig.h
│   │       │   └── GridTransform.h
│   │       └── ui/
│   │           ├── GridPanel.h
│   │           ├── ControlPanel.h
│   │           ├── ElementRenderer.h
│   │           └── AmountInputPopup.h
│   │
│   └── util/
│       ├── ErrorReporting.h
│       └── gl_debug/
│           ├── GLDebugOutput.h
│           ├── GLDebugFilter.h
│           ├── GLDebugFormatter.h
│           └── GLDebugCallback.h
│
├── src/
│   ├── Main.cpp
│   ├── core/
│   ├── systems/
│   ├── states/
│   ├── ui/
│   └── apps/
│       ├── cpu_tlp_shared_cache/
│       │   ├── CpuTLPSharedCacheState.cpp
│       │   ├── CpuTLPStateFactory.cpp
│       │   ├── simulation/
│       │   └── ui/
│       │
│       ├── cpu_tomasulo/
│       │   ├── CpuTomasuloState.cpp
│       │   ├── CpuTomasuloStateFactory.cpp
│       │   ├── simulation/
│       │   │   ├── TomasuloCPU.cpp
│       │   │   ├── TomasuloSimController.cpp
│       │   │   ├── assembler/
│       │   │   │   └── TomasuloAssembler.cpp
│       │   │   ├── memory/
│       │   │   │   ├── TomasuloRAM.cpp
│       │   │   │   └── TomasuloRegisterFile.cpp
│       │   │   └── pipeline/
│       │   │       ├── TomasuloCycleCoordinator.cpp
│       │   │       ├── fetch/
│       │   │       │   ├── PC_C.cpp
│       │   │       │   ├── PC_MUX.cpp
│       │   │       │   ├── PC_Adder.cpp
│       │   │       │   └── I_Cache.cpp
│       │   │       ├── decode/
│       │   │       │   ├── Instruction_Decoder.cpp
│       │   │       │   ├── Extends.cpp
│       │   │       │   └── Control_Unit.cpp
│       │   │       ├── issue/
│       │   │       │   ├── RegisterFilePipeline.cpp
│       │   │       │   ├── ROB.cpp
│       │   │       │   └── Flags_Unit.cpp
│       │   │       ├── buffers/
│       │   │       │   ├── StoreBuffer.cpp
│       │   │       │   ├── SB0.cpp
│       │   │       │   ├── SB1.cpp
│       │   │       │   ├── LoadBuffer.cpp
│       │   │       │   ├── LB0.cpp
│       │   │       │   ├── LB1.cpp
│       │   │       │   └── LB2.cpp
│       │   │       ├── reservation/
│       │   │       │   ├── ReservationStation.cpp
│       │   │       │   ├── RS_IntALU0.cpp
│       │   │       │   ├── RS_IntALU1.cpp
│       │   │       │   ├── RS_FPALU0.cpp
│       │   │       │   ├── RS_IntMUL0.cpp
│       │   │       │   └── RS_FPMUL0.cpp
│       │   │       ├── agu/
│       │   │       │   ├── AGU.cpp
│       │   │       │   ├── AGU_0.cpp
│       │   │       │   ├── AGU_1.cpp
│       │   │       │   └── AGU_Arbiter.cpp
│       │   │       ├── execution/
│       │   │       │   ├── IntALU.cpp
│       │   │       │   ├── FPALU.cpp
│       │   │       │   ├── IntMUL.cpp
│       │   │       │   ├── FPMUL.cpp
│       │   │       │   ├── IntALU_Arbiter.cpp
│       │   │       │   ├── FPALU_Arbiter.cpp
│       │   │       │   ├── IntMUL_Arbiter.cpp
│       │   │       │   └── FPMUL_Arbiter.cpp
│       │   │       ├── branch/
│       │   │       │   ├── RS_Branch0.cpp
│       │   │       │   ├── Branch_Arbiter.cpp
│       │   │       │   └── Branch_Executor.cpp
│       │   │       ├── cdb/
│       │   │       │   ├── CDB.cpp
│       │   │       │   └── CDB_Arbiter.cpp
│       │   │       └── commit/
│       │   │           ├── Memory_Arbiter.cpp
│       │   │           ├── D_Cache.cpp
│       │   │           └── Commit_Unit.cpp
│       │   └── ui/
│       │       ├── views/
│       │       │   ├── TomasuloMainView.cpp
│       │       │   ├── TomasuloCompilerView.cpp
│       │       │   ├── TomasuloRAMView.cpp
│       │       │   ├── TomasuloICacheView.cpp
│       │       │   ├── TomasuloDCacheView.cpp
│       │       │   ├── TomasuloRegistersView.cpp
│       │       │   ├── TomasuloROBView.cpp
│       │       │   └── TomasuloAnalysisView.cpp
│       │       ├── widgets/
│       │       │   ├── TomasuloRamTable.cpp
│       │       │   ├── CacheTable.cpp
│       │       │   ├── TomasuloRegTable.cpp
│       │       │   └── ROBTable.cpp
│       │       └── components/
│       │           ├── Camera2D.cpp
│       │           ├── ShaderBackground.cpp
│       │           └── BorderRenderer.cpp
│       │
│       └── quicksort_visualizer/
│           ├── QuicksortVisualizerState.cpp
│           ├── QuicksortVisualizerStateFactory.cpp
│           ├── algorithm/
│           │   └── QuicksortAlgorithm.cpp
│           ├── animation/
│           │   └── SwapAnimator.cpp
│           ├── audio/
│           │   └── SwapSoundGenerator.cpp
│           ├── data/
│           │   ├── ElementCollection.cpp
│           │   └── SwapQueue.cpp
│           ├── input/
│           │   └── GridInputHandler.cpp
│           ├── threading/
│           │   └── LogicThreadController.cpp
│           ├── visualization/
│           │   └── GridTransform.cpp
│           └── ui/
│               ├── GridPanel.cpp
│               ├── ControlPanel.cpp
│               ├── ElementRenderer.cpp
│               └── AmountInputPopup.cpp
│
├── resources/
│   └── (textures, fonts, shaders, configuration files)
│
└── thirdparty/
    ├── SFML-2.6.1/
    ├── imgui-docking/
    ├── imgui-sfml-2.6.x/
    ├── glad/
    ├── nlohmann/
    └── tinyfiledialogs/
```

---

## ⚙️ Build Configuration

- **IDE:** Visual Studio 2026  
- **CMake:** ≥ 3.16  
- **SFML:** statically linked  

### Build Mode Toggle

```cmake
option(PRODUCTION_BUILD "Make this a production build!" OFF)
```

> ⚠️ After changing `PRODUCTION_BUILD`, **delete the `out/` build directory** to force a clean reconfiguration (Visual Studio caching limitation).

---

## 🎯 Purpose & Vision

This project is intentionally **over-engineered by design**.

Its purpose is not minimalism, but:

* Deep mastery of **computer architecture**
* Professional-grade **C++ systems design**
* Realistic simulation of CPU internals
* A long-term, extensible technical foundation

It represents **five years of accumulated Computer Engineering knowledge**, distilled into a single evolving codebase.

---

## 👤 Author

**Adriel Sebastian Chaves Salazar**
Computer Engineering

**Interests & Focus Areas:**

* Embedded Systems Engineering
* Low-Level Programming
* Systems & Computer Architecture

---

🚧 **Status:** Actively developed and evolving