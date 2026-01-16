// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.cpp
// ============================================================================

/**
 * @file SharedMemoryComponent.cpp
 * @brief Implementation of SharedMemoryComponent.
 *
 * Provides DRAM backend with handshake-based access protocol.
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedMemoryComponent.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/Log.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace cpu_tlp {

    namespace {

        /**
         * @brief Debug output for RAM operations.
         */
        void debugRam(const char* msg, uint16_t addr, bool we, uint64_t writeData, uint64_t readData) {
#if 0  // Enable for debugging
            std::cout << "[DRAM] " << msg
                << " addr=0x" << std::hex << static_cast<unsigned>(addr)
                << " we=" << std::dec << (we ? 1 : 0)
                << " w=0x" << std::hex << writeData
                << " r=0x" << std::hex << readData
                << std::dec << "\n";
#else
            (void)msg; (void)addr; (void)we; (void)writeData; (void)readData;
#endif
        }

    } // anonymous namespace

    // ============================================================================
    // Construction / Destruction
    // ============================================================================

    SharedMemoryComponent::SharedMemoryComponent()
        : m_sharedData(nullptr)
        , m_executionThread(nullptr)
        , m_isRunning(false)
    {
    }

    SharedMemoryComponent::~SharedMemoryComponent() {
        shutdown();
    }

    // ============================================================================
    // Lifecycle
    // ============================================================================

    bool SharedMemoryComponent::initialize(std::shared_ptr<CPUSystemSharedData> sharedData) {
        if (m_isRunning) {
            std::cerr << "[SharedMemory] Component already running!\n";
            return false;
        }

        m_sharedData = std::move(sharedData);

        log_build_and_print([&](std::ostringstream& oss) {
            oss << "[SharedMemory] Initializing RAM with "
                << SharedMemory::MEM_SIZE_BYTES << " bytes ("
                << SharedMemory::MEM_SIZE_WORDS << " words of 64 bits)\n";
            });

        // Initialize memory with deterministic pattern
        m_memory.reset();
        m_memory.initTestPattern_0_1_2_3();

        // Initialize RAMConnection to known state
        if (m_sharedData) {
            auto& R = m_sharedData->ram_connection;

            for (int pe = 0; pe < 4; ++pe) {
                auto& C = m_sharedData->cache_connections[pe];
                C.RD_C_out.store(0, std::memory_order_release);
                C.C_READY.store(false, std::memory_order_release);
            }

            R.request_active.store(false, std::memory_order_release);
            R.response_ready.store(false, std::memory_order_release);
            R.write_enable.store(false, std::memory_order_release);
            R.request_address.store(0, std::memory_order_release);
            R.write_data.store(0, std::memory_order_release);
            R.read_data.store(0, std::memory_order_release);
        }

        m_isRunning = true;
        m_sharedData->system_should_stop.store(false, std::memory_order_release);

        m_executionThread = std::make_unique<std::thread>(&SharedMemoryComponent::threadMain, this);

        std::cout << "[SharedMemory] Initialized successfully\n";
        return true;
    }

    void SharedMemoryComponent::shutdown() {
        if (!m_isRunning) return;

        std::cout << "[SharedMemory] Shutting down...\n";

        if (m_sharedData) {
            m_sharedData->system_should_stop.store(true, std::memory_order_release);
        }

        if (m_executionThread && m_executionThread->joinable()) {
            m_executionThread->join();
        }
        m_isRunning = false;
        m_executionThread.reset();

        std::cout << "[SharedMemory] Shutdown complete\n";
    }

    bool SharedMemoryComponent::isRunning() const {
        return m_isRunning;
    }

    // ============================================================================
    // Service Thread
    // ============================================================================

    void SharedMemoryComponent::threadMain() {
        using namespace std::chrono_literals;

        std::cout << "[SharedMemory] Thread started\n";

        auto& R = m_sharedData->ram_connection;

        while (!m_sharedData->system_should_stop.load(std::memory_order_acquire)) {
            // Level-triggered handshake:
            // - Serve when request_active=1 and response_ready=0
            // - Interconnect will clear response_ready=0 + request_active=0 after consuming

            const bool req = R.request_active.load(std::memory_order_acquire);
            const bool rsp = R.response_ready.load(std::memory_order_acquire);

            if (req && !rsp) {
                const uint16_t addr = R.request_address.load(std::memory_order_acquire);
                const bool we = R.write_enable.load(std::memory_order_acquire);
                const uint64_t writeData = R.write_data.load(std::memory_order_acquire);

                uint64_t readData = 0;
                if (we) {
                    m_memory.write(addr, writeData);
                    readData = writeData;  // Echo for debugging
                }
                else {
                    readData = m_memory.read(addr);
                }

                // Publish payload then flag (release ordering)
                R.read_data.store(readData, std::memory_order_release);
                R.response_ready.store(true, std::memory_order_release);

                debugRam("SERVE", addr, we, writeData, readData);
            }
            else {
                std::this_thread::sleep_for(3us);
            }
        }

        std::cout << "[SharedMemory] Thread ending\n";
    }

} // namespace cpu_tlp