#pragma once

/**
 * @file TomasuloRamTable.h
 * @brief Paged RAM visualization table widget for Tomasulo CPU.
 *
 * Pure renderer — reads from a bound TomasuloRAM data source.
 * Does NOT store its own copy of RAM data.
 *
 * The caller MUST hold the simulation mutex while calling render()
 * to ensure thread-safe access to the data source.
 *
 * @note
 *   - SRP: Only handles RAM table display and paging (no data ownership)
 *   - OCP: Page size and RAM size derived from TomasuloRAM constants
 *   - DIP: Depends on const TomasuloRAM* abstraction, not concrete storage
 */

#include <string>
#include <cstdint>

class TomasuloRAM;   // forward declaration — simulation layer

/**
 * @class TomasuloRamTable
 * @brief Paged table widget for rendering RAM contents.
 *
 * Features:
 * - Reads directly from TomasuloRAM (single source of truth)
 * - 512 rows per page, 13 pages
 * - Address, hex, decimal, double columns
 * - Page navigation with First/Prev/Next/Last
 */
class TomasuloRamTable {
public:
    static constexpr int kRowsPerPage = 512;

    TomasuloRamTable() = default;

    // ── Data source binding ─────────────────────────────────────

    /**
     * @brief Binds the RAM data source for rendering.
     *
     * The pointer must remain valid for the lifetime of this widget.
     * Typically points to TomasuloCPU::ram() inside the controller.
     *
     * @param ram  Pointer to simulation RAM (never null after binding).
     */
    void bindDataSource(const TomasuloRAM* ram);

    // ── Rendering ───────────────────────────────────────────────

    /**
     * @brief Renders the paged RAM table with navigation controls.
     *
     * PRECONDITION: The caller holds the simulation mutex.
     *
     * @param id  ImGui widget ID.
     */
    void render(const char* id);

    // ── Page control ────────────────────────────────────────────

    int  currentPage() const { return m_currentPage; }
    void setPage(int page);
    void resetPage();   ///< Resets pagination to page 0

    int  totalRows()  const;
    int  totalPages() const;

private:
    const TomasuloRAM* m_dataSource = nullptr;
    int                m_currentPage = 0;

    void renderPageNavigation(float availableWidth);
    void renderTable(const char* id, float tableHeight);

    static std::string formatAddress(uint64_t addr);
    static std::string formatHex(uint64_t v);
    static int         rowsOnPage(int page, int totalPages, int totalRows);
    static int         firstRowOfPage(int page);
};