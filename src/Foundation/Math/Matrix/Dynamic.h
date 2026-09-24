#ifndef FOUNDATION_MATH_MATRIX_DYNAMIC_H
#define FOUNDATION_MATH_MATRIX_DYNAMIC_H

namespace Foundation::Math::Matrix {

/**
 * @class Dynamic
 * @ingroup Foundation_Matrix_Dynamic
 * @brief Matrix whose dimensions and storage are selected at runtime.
 * @tparam T Element type.
 */
template <typename T>
class Dynamic {
private:

    T* _data;
    unsigned int _rows;
    unsigned int _cols;
    bool _ownsData;

    void Release();
    unsigned int Index(unsigned int row, unsigned int col) const;

public:

    /** @brief Creates an empty matrix with no storage. */
    Dynamic();

    /**
     * @brief Creates an owning matrix with the requested dimensions.
     * @details The matrix remains empty when storage cannot be allocated.
     */
    Dynamic(unsigned int rows, unsigned int cols);

    /**
     * @brief Creates an owning matrix filled with @p value.
     * @details The matrix remains empty when storage cannot be allocated.
     */
    Dynamic(unsigned int rows, unsigned int cols, T value);

    /**
     * @brief Creates a matrix attached to caller-owned storage.
     * @details The matrix never releases @p externalData. The caller must keep
     * the buffer alive for the complete attachment lifetime.
     */
    Dynamic(unsigned int rows, unsigned int cols, T* externalData);

    /**
     * @brief Creates an owning deep copy.
     * @details The new matrix remains empty if its storage cannot be allocated.
     */
    Dynamic(const Dynamic<T>& other);

    /** @brief Transfers owned or attached storage without allocating. */
    Dynamic(Dynamic<T>&& other) noexcept;

    /** @brief Releases owned storage or detaches external storage. */
    ~Dynamic();

    /**
     * @brief Replaces this matrix with an owning deep copy.
     * @details If allocation fails, this matrix remains unchanged. Assigning an
     * empty matrix clears this matrix.
     */
    Dynamic<T>& operator=(const Dynamic<T>& other);

    /** @brief Transfers owned or attached storage without allocating. */
    Dynamic<T>& operator=(Dynamic<T>&& other) noexcept;

    /**
     * @brief Ensures valid storage with the requested dimensions.
     *
     * Existing storage, including attached storage, is reused when its
     * dimensions already match. Otherwise a new owning buffer is allocated.
     * Invalid dimensions, size overflow, or allocation failure return false
     * and leave the matrix unchanged.
     */
    bool Allocate(unsigned int rows, unsigned int cols);

    /**
     * @brief Attaches caller-owned storage without allocating.
     *
     * The caller retains ownership and must keep the buffer alive. Invalid
     * dimensions, a null pointer, size overflow, or an attempt to reattach the
     * matrix's own allocation return false and leave the matrix unchanged.
     */
    bool Attach(unsigned int rows, unsigned int cols, T* externalData);

    /** @brief Releases owned storage or detaches external storage. */
    void Clear();

    /** @brief Returns the number of rows, or zero for an empty matrix. */
    unsigned int RowsCount() const;

    /** @brief Returns the number of columns, or zero for an empty matrix. */
    unsigned int ColsCount() const;

    /** @brief Returns the validated element count, or zero when empty. */
    unsigned int Size() const;

    /** @brief Reports whether storage and non-zero dimensions are present. */
    bool IsValid() const;

    /** @brief Reports whether this matrix must release its current storage. */
    bool OwnsData() const;

    /** @brief Returns the storage pointer, or null when empty. */
    T* Data();

    /** @brief Returns the storage pointer, or null when empty. */
    const T* Data() const;

    /** @brief Returns an element without performing bounds checks. */
    T& At(unsigned int row, unsigned int col);

    /** @brief Returns an element without performing bounds checks. */
    const T& At(unsigned int row, unsigned int col) const;

    /**
     * @brief Adds matrices with matching dimensions.
     * @return An owning result, or an empty matrix for invalid inputs,
     * mismatched dimensions, or allocation failure.
     */
    Dynamic<T> operator+(const Dynamic<T>& other) const;

    /**
     * @brief Subtracts matrices with matching dimensions.
     * @return An owning result, or an empty matrix for invalid inputs,
     * mismatched dimensions, or allocation failure.
     */
    Dynamic<T> operator-(const Dynamic<T>& other) const;

    /** @brief Adds a compatible matrix; otherwise leaves this matrix unchanged. */
    Dynamic<T>& operator+=(const Dynamic<T>& other);

    /** @brief Subtracts a compatible matrix; otherwise leaves this matrix unchanged. */
    Dynamic<T>& operator-=(const Dynamic<T>& other);

    /** @brief Returns an owning scalar product, or an empty matrix on failure. */
    Dynamic<T> operator*(T scalar) const;

    /** @brief Returns an owning scalar quotient, or an empty matrix on failure. */
    Dynamic<T> operator/(T scalar) const;

    /** @brief Multiplies every element by @p scalar. */
    Dynamic<T>& operator*=(T scalar);

    /** @brief Divides every element by @p scalar. */
    Dynamic<T>& operator/=(T scalar);

    /** @brief Compares dimensions and every element. */
    bool operator==(const Dynamic<T>& other) const;

    /** @brief Reports whether dimensions or any element differ. */
    bool operator!=(const Dynamic<T>& other) const;

    /** @brief Returns a row pointer without performing bounds checks. */
    T* operator[](unsigned int row);

    /** @brief Returns a row pointer without performing bounds checks. */
    const T* operator[](unsigned int row) const;

    /** @brief Replaces every element with @p value. */
    void Fill(T value);

    /** @brief Replaces every element with its default value. */
    void Zero();

    /**
     * @brief Writes the transpose into @p result.
     * @return false for an invalid source, allocation failure, or overlapping
     * source and result storage. On failure, @p result remains unchanged.
     */
    bool TransposeTo(Dynamic<T>& result) const;

    /** @brief Returns an owning transpose, or an empty matrix on failure. */
    Dynamic<T> Transposed() const;
};

/** @cond FOUNDATION_INTERNAL */
namespace Detail {

    template <typename T>
    bool DynamicStorageOverlaps(
        const Dynamic<T>& first,
        const Dynamic<T>& second
    );

}
/** @endcond */

/** @brief Returns an owning scalar product, or an empty matrix on failure. */
template <typename T>
Dynamic<T> operator*(T scalar, const Dynamic<T>& matrix);

} // namespace Foundation::Math::Matrix

#include <Foundation/Math/Matrix/Dynamic.tpp>

#endif // FOUNDATION_MATH_MATRIX_DYNAMIC_H
