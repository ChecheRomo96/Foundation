#ifndef FOUNDATION_MATH_ARITHMETIC_H
#define FOUNDATION_MATH_ARITHMETIC_H

    #include <stdint.h>

    namespace Foundation {
        namespace Math {

            /**
             * @brief Computes the greatest common divisor of two unsigned values.
             * @ingroup Foundation_Math
             * @param a First value.
             * @param b Second value.
             * @return The largest value that divides both `a` and `b`. When one
             * argument is `0`, the other argument is returned, and
             * `GCD(0, 0)` is `0`.
             *
             * Uses the iterative Euclidean algorithm. The function is
             * `constexpr` and `noexcept` and accepts the full `uint32_t` range.
             */
            constexpr uint32_t GCD(uint32_t a, uint32_t b) noexcept {
                while (b != 0) {
                    const uint32_t temp = b;
                    b = a % b;
                    a = temp;
                }
                return a;
            }

            /**
             * @brief Computes the floored modulo of a signed 32-bit value.
             * @ingroup Foundation_Math
             * @param value Dividend. Every `int32_t` value, including
             * `INT32_MIN` and `INT32_MAX`, is accepted.
             * @param modulus Divisor. Must be positive for a meaningful result.
             * @return For `modulus > 0`, the value in `[0, modulus - 1]` that is
             * congruent to `value` modulo `modulus`, so `FloorMod(-1, 12)` is
             * `11` and `FloorMod(-12, 12)` is `0`. Returns `0` when
             * `modulus <= 0`.
             *
             * Unlike the built-in `%` operator, which truncates toward zero, the
             * result never takes the sign of a negative `value`. The function is
             * `constexpr` and `noexcept`, never overflows, and has no undefined
             * behavior for any input, including `INT32_MIN % -1`.
             */
            constexpr int32_t FloorMod(int32_t value, int32_t modulus) noexcept {
                if (modulus <= 0) {
                    return 0;
                }

                const int32_t remainder = value % modulus;
                return (remainder < 0) ? (remainder + modulus) : remainder;
            }

            /**
             * @brief Computes the floored quotient of a signed 32-bit value.
             * @ingroup Foundation_Math
             * @param value Dividend. Every `int32_t` value, including
             * `INT32_MIN` and `INT32_MAX`, is accepted.
             * @param divisor Divisor. Must be positive for a meaningful result.
             * @return For `divisor > 0`, the quotient rounded toward negative
             * infinity, so `FloorDiv(-1, 12)` is `-1`, `FloorDiv(-12, 12)` is
             * `-1`, and `FloorDiv(-13, 12)` is `-2`. Returns `0` when
             * `divisor <= 0`.
             *
             * Unlike the built-in `/` operator, which truncates toward zero,
             * this function rounds a non-integral negative quotient downward.
             * Together with FloorMod(), it satisfies
             * `FloorDiv(value, divisor) * divisor + FloorMod(value, divisor) == value`
             * for every positive `divisor`. Widen intermediate arithmetic when
             * checking that identity at the limits of `int32_t`. The function
             * is `constexpr` and `noexcept`, never overflows, and has no
             * undefined behavior for any input. Returning before division when
             * `divisor <= 0` also covers division by zero and `INT32_MIN / -1`.
             */
            constexpr int32_t FloorDiv(int32_t value, int32_t divisor) noexcept {
                if (divisor <= 0) {
                    return 0;
                }

                const int32_t quotient = value / divisor;
                const int32_t remainder = value % divisor;
                return (remainder < 0) ? (quotient - 1) : quotient;
            }

        }
    }

#endif
