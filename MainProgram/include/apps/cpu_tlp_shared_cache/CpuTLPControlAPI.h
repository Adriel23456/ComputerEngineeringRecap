// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/CpuTLPControlAPI.h
// ============================================================================

#pragma once

/**
 * @file CpuTLPControlAPI.h
 * @brief Callback interface for PE control commands.
 *
 * Provides a decoupled way for UI views to control processing
 * elements without direct dependencies on the state class.
 *
 * @note Follows:
 *   - DIP: Views depend on abstract callbacks, not concrete implementations
 *   - ISP: Callbacks are segregated by PE and command type
 */

#include <functional>

namespace cpu_tlp_ui {

	// ============================================================================
	// PE0 Control Callbacks
	// ============================================================================

	/** @brief Resets PE0 to initial state. */
	inline std::function<void()> onResetPE0;

	/** @brief Executes single instruction on PE0. */
	inline std::function<void()> onStepPE0;

	/** @brief Executes N instructions on PE0. */
	inline std::function<void(int)> onStepUntilPE0;

	/** @brief Starts continuous execution on PE0. */
	inline std::function<void()> onStepIndefinitelyPE0;

	/** @brief Stops PE0 execution. */
	inline std::function<void()> onStopPE0;

	// ============================================================================
	// PE1 Control Callbacks
	// ============================================================================

	inline std::function<void()> onResetPE1;
	inline std::function<void()> onStepPE1;
	inline std::function<void(int)> onStepUntilPE1;
	inline std::function<void()> onStepIndefinitelyPE1;
	inline std::function<void()> onStopPE1;

	// ============================================================================
	// PE2 Control Callbacks
	// ============================================================================

	inline std::function<void()> onResetPE2;
	inline std::function<void()> onStepPE2;
	inline std::function<void(int)> onStepUntilPE2;
	inline std::function<void()> onStepIndefinitelyPE2;
	inline std::function<void()> onStopPE2;

	// ============================================================================
	// PE3 Control Callbacks
	// ============================================================================

	inline std::function<void()> onResetPE3;
	inline std::function<void()> onStepPE3;
	inline std::function<void(int)> onStepUntilPE3;
	inline std::function<void()> onStepIndefinitelyPE3;
	inline std::function<void()> onStopPE3;

	// ============================================================================
	// Global Analysis Callbacks
	// ============================================================================

	/** @brief Resets all analysis counters to zero. */
	inline std::function<void()> onResetAnalysis;

} // namespace cpu_tlp_ui