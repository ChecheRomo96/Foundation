/**
 * @file Foundation/Math/Matrix/MatrixOperations.h
 * @brief Matrix arithmetic algorithms.
 */

#ifndef FOUNDATION_MATH_MATRIX_OPERATIONS_H
#define FOUNDATION_MATH_MATRIX_OPERATIONS_H

#include <Foundation/Math/Matrix/Fixed.h>
#include <Foundation/Math/Matrix/Dynamic.h>

namespace Foundation::Math::Matrix {

    /**
     * @ingroup Foundation_MatrixOperations
     * @brief Multiplies two fixed-size matrices.
     *
     * Computes:
     *
     * @code
     * result = a * b;
     * @endcode
     *
     * Matrix dimensions are checked at compile time through the template
     * parameters.
     *
     * @tparam T Element type.
     * @tparam RowsA Number of rows in matrix a.
     * @tparam ColsA Number of columns in matrix a and rows in matrix b.
     * @tparam ColsB Number of columns in matrix b.
     *
     * @param a Left-hand matrix.
     * @param b Right-hand matrix.
     * @param result Output matrix.
     */
    template <
        typename T,
        unsigned int RowsA,
        unsigned int ColsA,
        unsigned int ColsB
    >
    void Multiply(
        const Fixed<T, RowsA, ColsA>& a,
        const Fixed<T, ColsA, ColsB>& b,
        Fixed<T, RowsA, ColsB>& result
    ) {
        for(unsigned int i = 0; i < RowsA; i++) {
            for(unsigned int j = 0; j < ColsB; j++) {
                result[i][j] = T();

                for(unsigned int k = 0; k < ColsA; k++) {
                    result[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    /**
     * @ingroup Foundation_MatrixOperations
     * @brief Multiplies two dynamic matrices.
     *
     * The matrix dimensions are validated at runtime. The result matrix is
     * allocated or resized to match the resulting dimensions.
     *
     * @tparam T Element type.
     *
     * @param a Left-hand matrix.
     * @param b Right-hand matrix.
     * @param result Output matrix.
     *
     * @return true if the multiplication was successful.
     * @return false if the matrix dimensions are incompatible or allocation fails.
     */
    template <typename T>
    bool Multiply(
        const Dynamic<T>& a,
        const Dynamic<T>& b,
        Dynamic<T>& result
    ) {
        if(a.ColsCount() != b.RowsCount()) {
            return false;
        }

        if(!result.Allocate(a.RowsCount(), b.ColsCount())) {
            return false;
        }

        for(unsigned int i = 0; i < a.RowsCount(); i++) {
            for(unsigned int j = 0; j < b.ColsCount(); j++) {
                result.At(i, j) = T();

                for(unsigned int k = 0; k < a.ColsCount(); k++) {
                    result.At(i, j) += a.At(i, k) * b.At(k, j);
                }
            }
        }

        return true;
    }

    /**
     * @ingroup Foundation_MatrixOperations
     * @brief Multiplies a fixed-size matrix by a dynamic matrix.
     *
     * The number of rows in the dynamic matrix must match the number of columns
     * in the fixed-size matrix.
     *
     * The result matrix is allocated or resized to:
     *
     * @code
     * RowsA x b.ColsCount()
     * @endcode
     *
     * @tparam T Element type.
     * @tparam RowsA Number of rows in matrix a.
     * @tparam ColsA Number of columns in matrix a.
     * @param a Left-hand fixed-size matrix.
     * @param b Right-hand dynamic matrix.
     * @param result Output dynamic matrix.
     *
     * @return true if the multiplication was successful.
     * @return false if the matrix dimensions are incompatible or allocation fails.
     */
    template <
        typename T,
        unsigned int RowsA,
        unsigned int ColsA
    >
    bool Multiply(
        const Fixed<T, RowsA, ColsA>& a,
        const Dynamic<T>& b,
        Dynamic<T>& result
    ) {
        if(b.RowsCount() != ColsA) {
            return false;
        }

        if(!result.Allocate(RowsA, b.ColsCount())) {
            return false;
        }

        for(unsigned int i = 0; i < RowsA; i++) {
            for(unsigned int j = 0; j < b.ColsCount(); j++) {
                result.At(i, j) = T();

                for(unsigned int k = 0; k < ColsA; k++) {
                    result.At(i, j) += a[i][k] * b.At(k, j);
                }
            }
        }

        return true;
    }

    /**
     * @ingroup Foundation_MatrixOperations
     * @brief Multiplies a dynamic matrix by a fixed-size matrix.
     *
     * The number of columns in the dynamic matrix must match the number of rows
     * in the fixed-size matrix.
     *
     * The result matrix is allocated or resized to:
     *
     * @code
     * a.RowsCount() x ColsB
     * @endcode
     *
     * @tparam T Element type.
     * @tparam RowsB Number of rows in matrix b.
     * @tparam ColsB Number of columns in matrix b.
     *
     * @param a Left-hand dynamic matrix.
     * @param b Right-hand fixed-size matrix.
     * @param result Output dynamic matrix.
     *
     * @return true if the multiplication was successful.
     * @return false if the matrix dimensions are incompatible or allocation fails.
     */
    template <
        typename T,
        unsigned int RowsB,
        unsigned int ColsB
    >
    bool Multiply(
        const Dynamic<T>& a,
        const Fixed<T, RowsB, ColsB>& b,
        Dynamic<T>& result
    ) {
        if(a.ColsCount() != RowsB) {
            return false;
        }

        if(!result.Allocate(a.RowsCount(), ColsB)) {
            return false;
        }

        for(unsigned int i = 0; i < a.RowsCount(); i++) {
            for(unsigned int j = 0; j < ColsB; j++) {
                result.At(i, j) = T();

                for(unsigned int k = 0; k < RowsB; k++) {
                    result.At(i, j) += a.At(i, k) * b[k][j];
                }
            }
        }

        return true;
    }

} // namespace Foundation::Math::Matrix

#endif // FOUNDATION_MATH_MATRIX_OPERATIONS_H
