/**
 * @file Foundation/Math/Matrix/FixedMatrix/FixedMatrix.h
 * @brief Fixed-size matrix type.
 */

#ifndef FOUNDATION_MATH_FIXED_MATRIX_H
#define FOUNDATION_MATH_FIXED_MATRIX_H

namespace Foundation {
namespace Math {

    /**
     * @class FixedMatrix
     * @ingroup Foundation_Math
     *
     * @brief Represents a statically allocated matrix with fixed dimensions.
     *
     * @tparam T Element type.
     * @tparam Rows Number of rows.
     * @tparam Cols Number of columns.
     */
    template <typename T, unsigned int Rows, unsigned int Cols>
    class FixedMatrix {
    private:

        T _data[Rows][Cols];

    public:

        /**
         * @brief Creates a matrix initialized with default values.
         */
        FixedMatrix();

        /**
         * @brief Creates a matrix filled with a single value.
         *
         * @param value Value used to initialize all elements.
         */
        FixedMatrix(T value);

        /**
         * @brief Creates a matrix from a fixed-size 2D array.
         *
         * @param values Source array.
         */
        FixedMatrix(const T (&values)[Rows][Cols]);

        /**
         * @brief Copy constructor.
         *
         * @param other Matrix to copy.
         */
        FixedMatrix(const FixedMatrix<T, Rows, Cols>& other);

        /**
         * @brief Assigns another matrix to this matrix.
         *
         * @param other Matrix to copy from.
         * @return Reference to this matrix.
         */
        FixedMatrix<T, Rows, Cols>& operator=(
            const FixedMatrix<T, Rows, Cols>& other
        );

        /**
         * @brief Returns the number of rows.
         */
        unsigned int RowsCount() const;

        /**
         * @brief Returns the number of columns.
         */
        unsigned int ColsCount() const;

        /**
         * @brief Returns a pointer to the internal data buffer.
         */
        T* Data();

        /**
         * @brief Returns a const pointer to the internal data buffer.
         */
        const T* Data() const;

        /**
         * @brief Accesses an element by row and column.
         */
        T& At(unsigned int row, unsigned int col);

        /**
         * @brief Accesses an element by row and column.
         */
        const T& At(unsigned int row, unsigned int col) const;

        /**
         * @brief Accesses a matrix row.
         */
        T* operator[](unsigned int row);

        /**
         * @brief Accesses a matrix row.
         */
        const T* operator[](unsigned int row) const;

        /**
         * @brief Adds two matrices.
         */
        FixedMatrix<T, Rows, Cols> operator+(
            const FixedMatrix<T, Rows, Cols>& other
        ) const;

        /**
         * @brief Subtracts another matrix from this matrix.
         */
        FixedMatrix<T, Rows, Cols> operator-(
            const FixedMatrix<T, Rows, Cols>& other
        ) const;

        /**
         * @brief Adds another matrix to this matrix.
         */
        FixedMatrix<T, Rows, Cols>& operator+=(
            const FixedMatrix<T, Rows, Cols>& other
        );

        /**
         * @brief Subtracts another matrix from this matrix.
         */
        FixedMatrix<T, Rows, Cols>& operator-=(
            const FixedMatrix<T, Rows, Cols>& other
        );

        /**
         * @brief Multiplies the matrix by a scalar.
         */
        FixedMatrix<T, Rows, Cols> operator*(T scalar) const;

        /**
         * @brief Divides the matrix by a scalar.
         */
        FixedMatrix<T, Rows, Cols> operator/(T scalar) const;

        /**
         * @brief Multiplies this matrix by a scalar.
         */
        FixedMatrix<T, Rows, Cols>& operator*=(T scalar);

        /**
         * @brief Divides this matrix by a scalar.
         */
        FixedMatrix<T, Rows, Cols>& operator/=(T scalar);

        /**
         * @brief Checks whether two matrices are equal.
         */
        bool operator==(
            const FixedMatrix<T, Rows, Cols>& other
        ) const;

        /**
         * @brief Checks whether two matrices are different.
         */
        bool operator!=(
            const FixedMatrix<T, Rows, Cols>& other
        ) const;

        /**
         * @brief Fills the matrix with a value.
         */
        void Fill(T value);

        /**
         * @brief Sets all matrix elements to zero.
         */
        void Zero();

        /**
         * @brief Returns the transposed matrix.
         */
        FixedMatrix<T, Cols, Rows> Transposed() const;

        /**
         * @brief Creates a matrix filled with a value.
         */
        static FixedMatrix<T, Rows, Cols> Filled(T value);

        /**
         * @brief Creates a zero-filled matrix.
         */
        static FixedMatrix<T, Rows, Cols> Zeros();
    };

    /**
     * @brief Multiplies a matrix by a scalar.
     *
     * Allows scalar multiplication in the form:
     *
     * @code
     * result = scalar * matrix;
     * @endcode
     */
    template <typename T, unsigned int Rows, unsigned int Cols>
    FixedMatrix<T, Rows, Cols> operator*(
        T scalar,
        const FixedMatrix<T, Rows, Cols>& matrix
    );

    } // namespace Math
    } // namespace Foundation

    #include <Foundation/Math/Matrix/FixedMatrix/FixedMatrix.tpp>

#endif// FOUNDATION_MATH_FIXED_MATRIX_H
