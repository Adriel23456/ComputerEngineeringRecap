// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/views/processor/PERegView.cpp
// ============================================================================

/**
 * @file PERegView.cpp
 * @brief Implementation of PERegView.
 */

#include "apps/cpu_tlp_shared_cache/ui/views/processor/PERegView.h"
#include <cstdio>

PERegView::PERegView(int peIndex)
    : m_peIndex(peIndex)
    , m_table(peIndex)
{
}

void PERegView::render() {
    char tableId[32];
    std::snprintf(tableId, sizeof(tableId), "##PE%d_RegTable", m_peIndex);
    m_table.render(tableId);
}