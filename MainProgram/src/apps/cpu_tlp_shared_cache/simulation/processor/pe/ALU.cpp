// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/processor/pe/ALU.cpp
// ============================================================================

/**
 * @file ALU.cpp
 * @brief Implementation of ALU.
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/ALU.h"
#include <cmath>
#include <cstring>

namespace cpu_tlp {

    // ========================================================================
    // Helper Functions
    // ========================================================================

    namespace {

        /**
         * @brief Computes NZCV flags for 64-bit result.
         */
        inline void computeFlags64(uint64_t result, uint64_t a, uint64_t b,
            bool isSubtraction, uint8_t& flags) {
            bool N = (result >> 63) & 1;
            bool Z = (result == 0);

            bool C;
            if (isSubtraction) {
                C = (a >= b);  // No borrow
            }
            else {
                C = (result < a);  // Carry
            }

            bool sa = (int64_t)a < 0;
            bool sb = (int64_t)b < 0;
            bool sr = (int64_t)result < 0;
            bool V;
            if (isSubtraction) {
                V = (sa != sb && sr != sa);
            }
            else {
                V = (sa == sb && sr != sa);
            }

            flags = (N ? 0x8 : 0) | (Z ? 0x4 : 0) | (C ? 0x2 : 0) | (V ? 0x1 : 0);
        }

        /**
         * @brief Converts bits to double.
         */
        inline double bitsToDouble(uint64_t bits) {
            double d;
            std::memcpy(&d, &bits, 8);
            return d;
        }

        /**
         * @brief Converts double to bits.
         */
        inline uint64_t doubleToBits(double d) {
            uint64_t bits;
            std::memcpy(&bits, &d, 8);
            return bits;
        }

    } // anonymous namespace

    // ========================================================================
    // ALU Implementation
    // ========================================================================

    ALUResult ALU::execute(uint8_t control, uint64_t A, uint64_t B, uint8_t flagsIn) const {
        ALUResult res;
        res.value = 0;
        res.flags = flagsIn;

        bool carry = (flagsIn & 0x2) != 0;

        switch (control) {
            // Integer operations
        case 0x00:  // ADD
            res.value = A + B;
            computeFlags64(res.value, A, B, false, res.flags);
            break;
        case 0x01:  // SUB
            res.value = A - B;
            computeFlags64(res.value, A, B, true, res.flags);
            break;
        case 0x02:  // ADC
            res.value = A + B + (carry ? 1 : 0);
            computeFlags64(res.value, A, B + (carry ? 1 : 0), false, res.flags);
            break;
        case 0x03:  // SBC
            res.value = A - B - (carry ? 0 : 1);
            computeFlags64(res.value, A, B + (carry ? 0 : 1), true, res.flags);
            break;
        case 0x04:  // MUL
            res.value = A * B;
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x05:  // DIV
            if (B != 0) res.value = (int64_t)A / (int64_t)B;
            else res.value = 0;
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x06:  // AND
            res.value = A & B;
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x07:  // ORR
            res.value = A | B;
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x08:  // EOR
            res.value = A ^ B;
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x09:  // BIC
            res.value = A & ~B;
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x0A:  // LSL
            res.value = A << (B & 0x3F);
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x0B:  // LSR
            res.value = A >> (B & 0x3F);
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x0C:  // ASR
            res.value = (uint64_t)((int64_t)A >> (B & 0x3F));
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x0D: {  // ROR
            uint8_t shift = B & 0x3F;
            res.value = (A >> shift) | (A << (64 - shift));
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        }

                 // Floating-point operations
        case 0x0E: {  // FADD
            double da = bitsToDouble(A);
            double db = bitsToDouble(B);
            double r = da + db;
            res.value = doubleToBits(r);
            if (std::isnan(da) || std::isnan(db) || std::isnan(r)) {
                res.flags = 0x1;
            }
            else {
                bool N = (r < 0.0);
                bool Z = (r == 0.0);
                res.flags = (N ? 0x8 : 0) | (Z ? 0x4 : 0);
            }
            break;
        }
        case 0x0F: {  // FSUB
            double da = bitsToDouble(A);
            double db = bitsToDouble(B);
            double r = da - db;
            res.value = doubleToBits(r);
            if (std::isnan(da) || std::isnan(db) || std::isnan(r)) {
                res.flags = 0x1;
            }
            else {
                bool N = (r < 0.0);
                bool Z = (r == 0.0);
                bool C = (da >= db);
                res.flags = (N ? 0x8 : 0) | (Z ? 0x4 : 0) | (C ? 0x2 : 0);
            }
            break;
        }
        case 0x10:  // FMUL
            res.value = doubleToBits(bitsToDouble(A) * bitsToDouble(B));
            break;
        case 0x11:  // FDIV
            res.value = doubleToBits(bitsToDouble(A) / bitsToDouble(B));
            break;
        case 0x12: {  // FCOPYSIGN
            double mag = std::fabs(bitsToDouble(A));
            double sign = bitsToDouble(B);
            res.value = doubleToBits(std::copysign(mag, sign));
            break;
        }
        case 0x13:  // FSQRT
            res.value = doubleToBits(std::sqrt(bitsToDouble(B)));
            break;
        case 0x14:  // FNEG
            res.value = doubleToBits(-bitsToDouble(B));
            break;
        case 0x15:  // FABS
            res.value = doubleToBits(std::fabs(bitsToDouble(B)));
            break;
        case 0x16:  // FCDTI (double to int)
            res.value = (uint64_t)(int64_t)bitsToDouble(B);
            break;
        case 0x17:  // FCDTD (int to double)
            res.value = doubleToBits((double)(int64_t)B);
            break;
        case 0x18:  // RTNR (round to nearest)
            res.value = doubleToBits(std::round(bitsToDouble(B)));
            break;
        case 0x19:  // RTZ (round to zero)
            res.value = doubleToBits(std::trunc(bitsToDouble(B)));
            break;
        case 0x1A:  // RTP (round to positive)
            res.value = doubleToBits(std::ceil(bitsToDouble(B)));
            break;
        case 0x1B:  // RTNE (round to negative)
            res.value = doubleToBits(std::floor(bitsToDouble(B)));
            break;

            // Special operations
        case 0x1C:  // MOV
            res.value = B;
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x1D:  // MVN
            res.value = ~B;
            res.flags = ((res.value >> 63) & 1) ? 0x8 : 0;
            if (res.value == 0) res.flags |= 0x4;
            break;
        case 0x1E:  // FMOVI
            res.value = B;
            break;
        case 0x1F:  // FMVNI
            res.value = ~B;
            break;
        case 0x20: {  // FCMPS
            double da = bitsToDouble(A);
            double db = bitsToDouble(B);

            if (std::isnan(da) || std::isnan(db)) {
                res.flags = 0x1;
                res.value = 0;
                break;
            }

            bool Z = (da == db);
            bool N = (da < db);
            bool C = (da >= db);

            res.flags = (N ? 0x8 : 0) | (Z ? 0x4 : 0) | (C ? 0x2 : 0);
            res.value = 0;
            break;
        }
        case 0x21:  // CRASH
            res.value = 0;
            break;
        case 0x22:  // NOTHING
            res.value = 0;
            break;
        default:
            res.value = A;
            break;
        }

        return res;
    }

} // namespace cpu_tlp