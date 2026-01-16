// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/views/RAMView.h
// ============================================================================

#pragma once

/**
 * @file RAMView.h
 * @brief RAM visualization and control view.
 *
 * Displays shared memory contents with Reset and Load functionality.
 *
 * @note Follows:
 *   - SRP: Only handles RAM display and basic controls
 *   - DIP: Depends on SharedMemoryComponent abstraction
 */

#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include "apps/cpu_tlp_shared_cache/ui/widgets/RamTable.h"

 // Forward declaration
namespace cpu_tlp {
    class SharedMemoryComponent;
}

/**
 * @class RAMView
 * @brief View for displaying and managing shared RAM.
 */
class RAMView : public ICpuTLPView {
public:
    RAMView() : m_sharedMemoryComponent(nullptr) {}

    /**
     * @brief Sets the memory component reference.
     * @param component Pointer to SharedMemoryComponent.
     */
    void setSharedMemoryComponent(cpu_tlp::SharedMemoryComponent* component) {
        m_sharedMemoryComponent = component;
    }

    /**
     * @brief Renders RAM table and control buttons.
     */
    void render() override;

private:
    RamTable m_table;                                    ///< RAM visualization widget
    cpu_tlp::SharedMemoryComponent* m_sharedMemoryComponent; ///< Memory component reference
};