#ifndef FOUNDATION_MATH_COMPLEX_H
#define FOUNDATION_MATH_COMPLEX_H

/**
 * @file Foundation/Math/Complex/Complex.h
 * @brief Cartesian complex number template.
 */

    #include <stdint.h>

    namespace Foundation {
    namespace Math {

        /**
         * @class Complex
         * @ingroup Foundation_Math_Complex
         *
         * @brief Complex number stored as real and imaginary parts.
         *
         * @tparam T Arithmetic type used for both parts, such as `float`,
         * `double`, or a signed integer type.
         *
         * All arithmetic is performed directly in `T`, without widening or
         * overflow checks. Division by a complex value whose magnitude is zero
         * is not checked: it produces infinities or NaN for floating-point `T`
         * and is undefined behavior for integer `T`.
         */
        template <typename T>
        class Complex {
        public:

            /** @brief Real part. */
            T real;

            /** @brief Imaginary part. */
            T imag;

            /**
             * @brief Creates the complex value `0 + 0i`.
             */
            Complex();

            /**
             * @brief Creates a purely real complex value.
             *
             * @param real Real part. The imaginary part is set to zero.
             */
            Complex(T real);

            /**
             * @brief Creates a complex value from both parts.
             *
             * @param real Real part.
             * @param imag Imaginary part.
             */
            Complex(
                T real,
                T imag
            );

            /**
             * @brief Copy constructor.
             *
             * @param other Complex value to copy.
             */
            Complex(
                const Complex<T>& other
            );

            /**
             * @brief Assigns another complex value to this value.
             *
             * @param other Complex value to copy from.
             * @return Reference to this value.
             */
            Complex<T>& operator=(
                const Complex<T>& other
            );

            /**
             * @brief Unary plus.
             *
             * @return A copy of this value.
             */
            Complex<T> operator+() const;

            /**
             * @brief Unary minus.
             *
             * @return The value with both parts negated.
             */
            Complex<T> operator-() const;

            /**
             * @brief Adds two complex values.
             *
             * @param other Right-hand operand.
             * @return The component-wise sum.
             */
            Complex<T> operator+(
                const Complex<T>& other
            ) const;

            /**
             * @brief Subtracts two complex values.
             *
             * @param other Right-hand operand.
             * @return The component-wise difference.
             */
            Complex<T> operator-(
                const Complex<T>& other
            ) const;

            /**
             * @brief Multiplies two complex values.
             *
             * @param other Right-hand operand.
             * @return `(ac - bd) + (ad + bc)i` for `(a + bi)(c + di)`.
             */
            Complex<T> operator*(
                const Complex<T>& other
            ) const;

            /**
             * @brief Divides this value by another complex value.
             *
             * @param other Divisor. Its magnitude must be nonzero; see the
             * class description for the unchecked zero case.
             * @return The complex quotient.
             */
            Complex<T> operator/(
                const Complex<T>& other
            ) const;

            /**
             * @brief Adds another complex value to this value.
             *
             * @param other Value to add.
             * @return Reference to this value.
             */
            Complex<T>& operator+=(
                const Complex<T>& other
            );

            /**
             * @brief Subtracts another complex value from this value.
             *
             * @param other Value to subtract.
             * @return Reference to this value.
             */
            Complex<T>& operator-=(
                const Complex<T>& other
            );

            /**
             * @brief Multiplies this value by another complex value.
             *
             * @param other Multiplier.
             * @return Reference to this value.
             */
            Complex<T>& operator*=(
                const Complex<T>& other
            );

            /**
             * @brief Divides this value by another complex value.
             *
             * @param other Divisor. Its magnitude must be nonzero; see the
             * class description for the unchecked zero case.
             * @return Reference to this value.
             */
            Complex<T>& operator/=(
                const Complex<T>& other
            );

            /**
             * @brief Compares two complex values for exact equality.
             *
             * @param other Value to compare with.
             * @return `true` when both parts compare equal with `T`'s `==`.
             * Floating-point values are compared exactly, without tolerance.
             */
            bool operator==(
                const Complex<T>& other
            ) const;

            /**
             * @brief Compares two complex values for inequality.
             *
             * @param other Value to compare with.
             * @return The negation of operator==().
             */
            bool operator!=(
                const Complex<T>& other
            ) const;

            /**
             * @brief Returns the complex conjugate.
             *
             * @return `real - imag i`.
             */
            Complex<T> Conjugate() const;

            /**
             * @brief Returns the squared magnitude.
             *
             * @return `real * real + imag * imag`. Computing the squared
             * magnitude avoids a square root and floating-point dependencies.
             */
            T MagnitudeSquared() const;
        };

    } // namespace Math
    } // namespace Foundation

    #include <Foundation/Math/Complex/Complex.tpp>

#endif // FOUNDATION_MATH_COMPLEX_H