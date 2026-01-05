#pragma once
#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/RamTable.h"

// Forward declaration
namespace cpu_tlp { class SharedMemoryComponent; }

class RAMView : public ICpuTLPView {
public:
    RAMView() : m_sharedMemoryComponent(nullptr) {}

    void setSharedMemoryComponent(cpu_tlp::SharedMemoryComponent* component) {
        m_sharedMemoryComponent = component;
    }

    void render() override;

private:
    RamTable m_table;
    cpu_tlp::SharedMemoryComponent* m_sharedMemoryComponent;
};