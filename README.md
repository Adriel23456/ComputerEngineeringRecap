# Computer Science Engineering Recap (CSE Recap)

A **comprehensive C++ recap project** that consolidates and reimplements the core concepts, systems, and architectural patterns studied throughout my **Computer Engineering (CE / CSE)** degree.

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

### Planned
- **Tomasulo Algorithm Simulator**
  - Dynamic scheduling
  - Reservation stations
  - Complete CPU datapath simulation *(Coming soon)*

- **Quicksort Visualizer**
  - Step-by-step algorithm visualization
  - Performance and partitioning analysis *(Coming soon)*

- **Additional Computer Architecture & Algorithms Projects**
  - To be announced

---

## 🛠️ Technology Stack

- **Language:** C++17  
- **Build System:** CMake (≥ 3.16)  
- **Compiler:** MSVC (Visual Studio 2026)  

### Graphics & UI
- SFML (static)
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
│
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
│   │   └── cpu_tlp_shared_cache/
│   │       ├── CpuTLPControlAPI.h
│   │       ├── CpuTLPSharedCacheState.h
│   │       ├── simulation/
│   │       │   ├── assembler/
│   │       │   │   └── Assembler.h
│   │       │   ├── bus/
│   │       │   │   └── InterconnectBus.h
│   │       │   ├── cache/
│   │       │   │   ├── L1Cache.h
│   │       │   │   ├── L1Snoop.h
│   │       │   │   └── L1Utils.h
│   │       │   ├── debug/
│   │       │   │   ├── TLPDebug.h
│   │       │   │   └── TLPDebugHelpers.h
│   │       │   ├── memory/
│   │       │   │   ├── InstructionMemoryComponent.h
│   │       │   │   └── SharedMemory.h
│   │       │   └── processor/
│   │       │       ├── InterconnectComponent.h
│   │       │       ├── L1Component.h
│   │       │       ├── SharedMemoryComponent.h
│   │       │       ├── PEComponent.h
│   │       │       └── pe/
│   │       │           ├── PipelineRegisters.h
│   │       │           ├── RegisterFile.h
│   │       │           ├── ALU.h
│   │       │           ├── ControlUnit.h
│   │       │           └── HazardUnit.h
│   │       └── ui/
│   │           ├── views/
│   │           │   ├── ICpuTLPView.h
│   │           │   ├── GeneralView.h
│   │           │   ├── CompilerView.h
│   │           │   ├── RAMView.h
│   │           │   └── AnalysisDataView.h
│   │           └── widgets/
│   │               ├── CacheMemTable.h
│   │               ├── MemCacheTable.h
│   │               ├── RamTable.h
│   │               ├── RegTable.h
│   │               ├── InstructionDisassembler.h
│   │               ├── ZoomPanImage.h
│   │               └── Log.h
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
│       └── cpu_tlp_shared_cache/
│           ├── CpuTLPSharedCacheState.cpp
│           ├── CpuTLPStateFactory.cpp
│           ├── simulation/
│           └── ui/
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

````

---

## 🧠 Architectural Notes

- Single executable hosting **multiple independent applications**
- Each simulator runs as a **self-contained application state**
- Simulation logic is fully **decoupled from UI**
- Advanced inspection via **ImGui Docking**
- Designed with **future CUDA integration** in mind
- AVX2 optimizations enabled on MSVC

---

## ⚙️ Build Configuration

- **IDE:** Visual Studio 2026
- **CMake:** ≥ 3.16
- **SFML:** statically linked
- **Build mode toggle:**
  ```cmake
  option(PRODUCTION_BUILD "Make this a production build!" OFF)
````

## 🎯 Purpose & Vision

This project is intentionally **over-engineered by design**.

Its purpose is not minimalism, but:

* Deep mastery of **computer architecture**
* Professional-grade **C++ systems design**
* Realistic simulation of CPU internals
* A long-term, extensible technical foundation

It represents **five years of accumulated Computer Engineering knowledge**, distilled into a single evolving codebase.

---

## 📄 License

This project is licensed under the **MIT License**.

```
MIT License

Copyright (c) 2026 Adriel23456

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 👤 Author

**Adriel Sebastian Chaves Salazar**
Computer Engineering
C++ / Systems & Architecture Enthusiast

---

🚧 **Status:** Actively developed and evolving