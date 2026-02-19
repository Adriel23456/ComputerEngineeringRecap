// ============================================================================
// File: include/apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.h
// ============================================================================

#pragma once

/**
 * @file TomasuloRamTable.h
 * @brief Paged RAM visualization widget for the Tomasulo CPU.
 *
 * Pure renderer — owns no RAM data. Reads directly from a bound
 * TomasuloRAM* (single source of truth).
 *
 * Layout:
 *   - 8-byte rows addressed from 0x0 to RAM top.
 *   - 512 rows per page (13 pages for the default 50 KB RAM).
 *   - Columns: Address | Value (hex) | Decimal (int64) | Double (IEEE 754).
 *   - Navigation bar: |<< < [page input] > >>|.
 *
 * Thread safety:
 *   The caller MUST hold the simulation mutex while calling render().
 *
 * @note
 *   - SRP: Only handles RAM table display and paging. No data ownership.
 *   - OCP: Page size derived from constants; no code change needed for
 *          different RAM sizes.
 *   - DIP: Depends on const TomasuloRAM*, not on concrete storage layout.
 */

#include <string>
#include <cstdint>

class TomasuloRAM;

class TomasuloRamTable {
public:
    static constexpr int kRowsPerPage = 512;  ///< Rows displayed per page.

    TomasuloRamTable() = default;

    // ── Data Source Binding ──────────────────────────────────────

    /**
     * @brief Binds the simulation RAM as the read-only data source.
     *
     * The pointer must remain valid for the lifetime of this widget.
     * Typically set once by CpuTomasuloState::bindDataSources().
     *
     * @param ram  Pointer to the simulation RAM instance.
     */
    void bindDataSource(const TomasuloRAM* ram);

    // ── Rendering ────────────────────────────────────────────────

    /**
     * @brief Renders the page navigation bar and the data table.
     *
     * @pre   The caller holds the simulation mutex.
     * @param id  ImGui widget ID string.
     */
    void render(const char* id);

    // ── Page Control ─────────────────────────────────────────────
    int  currentPage() const { return m_currentPage; }
    void setPage(int page);

    /** @brief Resets pagination to page 0 (called on RAM reset). */
    void resetPage();

    int totalRows()  const;
    int totalPages() const;

private:
    const TomasuloRAM* m_dataSource = nullptr;
    int                m_currentPage = 0;

    // ── Rendering Helpers ────────────────────────────────────────
    void renderPageNavigation(float availableWidth);
    void renderTable(const char* id, float tableHeight);

    // ── Formatting Helpers ───────────────────────────────────────
    static std::string formatAddress(uint64_t addr);  ///< "0x1A00" style.
    static std::string formatHex(uint64_t v);         ///< Full 16-digit hex.

    // ── Page Math Helpers ────────────────────────────────────────
    static int rowsOnPage(int page, int totalPages, int totalRows);
    static int firstRowOfPage(int page);
};