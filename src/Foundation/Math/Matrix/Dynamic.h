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

    Dynamic();
    Dynamic(unsigned int rows, unsigned int cols);
    Dynamic(unsigned int rows, unsigned int cols, T value);
    Dynamic(unsigned int rows, unsigned int cols, T* externalData);
    Dynamic(const Dynamic<T>& other);
    Dynamic(Dynamic<T>&& other);

    ~Dynamic();

    Dynamic<T>& operator=(const Dynamic<T>& other);
    Dynamic<T>& operator=(Dynamic<T>&& other);

    bool Allocate(unsigned int rows, unsigned int cols);
    void Attach(unsigned int rows, unsigned int cols, T* externalData);
    void Clear();

    unsigned int RowsCount() const;
    unsigned int ColsCount() const;
    unsigned int Size() const;
    bool IsValid() const;
    bool OwnsData() const;

    T* Data();
    const T* Data() const;

    T& At(unsigned int row, unsigned int col);
    const T& At(unsigned int row, unsigned int col) const;

    Dynamic<T> operator+(const Dynamic<T>& other) const;
    Dynamic<T> operator-(const Dynamic<T>& other) const;

    Dynamic<T>& operator+=(const Dynamic<T>& other);
    Dynamic<T>& operator-=(const Dynamic<T>& other);

    Dynamic<T> operator*(T scalar) const;
    Dynamic<T> operator/(T scalar) const;

    Dynamic<T>& operator*=(T scalar);
    Dynamic<T>& operator/=(T scalar);

    bool operator==(const Dynamic<T>& other) const;
    bool operator!=(const Dynamic<T>& other) const;

    T* operator[](unsigned int row);
    const T* operator[](unsigned int row) const;

    void Fill(T value);
    void Zero();

    bool TransposeTo(Dynamic<T>& result) const;
    Dynamic<T> Transposed() const;
};

template <typename T>
Dynamic<T> operator*(T scalar, const Dynamic<T>& matrix);

} // namespace Foundation::Math::Matrix

#include <Foundation/Math/Matrix/Dynamic.tpp>

#endif // FOUNDATION_MATH_MATRIX_DYNAMIC_H
