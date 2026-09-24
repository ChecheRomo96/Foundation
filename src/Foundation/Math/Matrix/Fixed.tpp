#ifndef FOUNDATION_MATH_MATRIX_FIXED_H
    #error "Include <Foundation/Math/Matrix/Fixed.h> instead of Fixed.tpp"
#elif !defined(FOUNDATION_MATH_MATRIX_FIXED_TPP)
#define FOUNDATION_MATH_MATRIX_FIXED_TPP

namespace Foundation::Math::Matrix {

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>::Fixed() {
    Zero();
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>::Fixed(T value) {
    Fill(value);
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>::Fixed(
    const T (&values)[Rows][Cols]
) {
    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            _data[i][j] = values[i][j];
        }
    }
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>::Fixed(
    const Fixed<T, Rows, Cols>& other
) {
    *this = other;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>& Fixed<T, Rows, Cols>::operator=(
    const Fixed<T, Rows, Cols>& other
) {
    if(this == &other) {
        return *this;
    }

    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            _data[i][j] = other._data[i][j];
        }
    }

    return *this;
}

template <typename T, unsigned int Rows, unsigned int Cols>
unsigned int Fixed<T, Rows, Cols>::RowCount() const {
    return Rows;
}

template <typename T, unsigned int Rows, unsigned int Cols>
unsigned int Fixed<T, Rows, Cols>::ColumnCount() const {
    return Cols;
}

template <typename T, unsigned int Rows, unsigned int Cols>
T* Fixed<T, Rows, Cols>::Data() {
    return &_data[0][0];
}

template <typename T, unsigned int Rows, unsigned int Cols>
const T* Fixed<T, Rows, Cols>::Data() const {
    return &_data[0][0];
}

template <typename T, unsigned int Rows, unsigned int Cols>
T& Fixed<T, Rows, Cols>::At(
    unsigned int row,
    unsigned int col
) {
    return _data[row][col];
}

template <typename T, unsigned int Rows, unsigned int Cols>
const T& Fixed<T, Rows, Cols>::At(
    unsigned int row,
    unsigned int col
) const {
    return _data[row][col];
}

template <typename T, unsigned int Rows, unsigned int Cols>
T* Fixed<T, Rows, Cols>::operator[](unsigned int row) {
    return _data[row];
}

template <typename T, unsigned int Rows, unsigned int Cols>
const T* Fixed<T, Rows, Cols>::operator[](unsigned int row) const {
    return _data[row];
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols> Fixed<T, Rows, Cols>::operator+(
    const Fixed<T, Rows, Cols>& other
) const {
    Fixed<T, Rows, Cols> result;

    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            result._data[i][j] = _data[i][j] + other._data[i][j];
        }
    }

    return result;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols> Fixed<T, Rows, Cols>::operator-(
    const Fixed<T, Rows, Cols>& other
) const {
    Fixed<T, Rows, Cols> result;

    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            result._data[i][j] = _data[i][j] - other._data[i][j];
        }
    }

    return result;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>& Fixed<T, Rows, Cols>::operator+=(
    const Fixed<T, Rows, Cols>& other
) {
    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            _data[i][j] += other._data[i][j];
        }
    }

    return *this;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>& Fixed<T, Rows, Cols>::operator-=(
    const Fixed<T, Rows, Cols>& other
) {
    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            _data[i][j] -= other._data[i][j];
        }
    }

    return *this;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols> Fixed<T, Rows, Cols>::operator*(
    T scalar
) const {
    Fixed<T, Rows, Cols> result;

    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            result._data[i][j] = _data[i][j] * scalar;
        }
    }

    return result;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols> Fixed<T, Rows, Cols>::operator/(
    T scalar
) const {
    Fixed<T, Rows, Cols> result;

    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            result._data[i][j] = _data[i][j] / scalar;
        }
    }

    return result;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>& Fixed<T, Rows, Cols>::operator*=(
    T scalar
) {
    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            _data[i][j] *= scalar;
        }
    }

    return *this;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols>& Fixed<T, Rows, Cols>::operator/=(
    T scalar
) {
    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            _data[i][j] /= scalar;
        }
    }

    return *this;
}

template <typename T, unsigned int Rows, unsigned int Cols>
bool Fixed<T, Rows, Cols>::operator==(
    const Fixed<T, Rows, Cols>& other
) const {
    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            if(_data[i][j] != other._data[i][j]) {
                return false;
            }
        }
    }

    return true;
}

template <typename T, unsigned int Rows, unsigned int Cols>
bool Fixed<T, Rows, Cols>::operator!=(
    const Fixed<T, Rows, Cols>& other
) const {
    return !(*this == other);
}

template <typename T, unsigned int Rows, unsigned int Cols>
void Fixed<T, Rows, Cols>::Fill(T value) {
    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            _data[i][j] = value;
        }
    }
}

template <typename T, unsigned int Rows, unsigned int Cols>
void Fixed<T, Rows, Cols>::Zero() {
    Fill(T());
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Cols, Rows> Fixed<T, Rows, Cols>::Transposed() const {
    Fixed<T, Cols, Rows> result;

    for(unsigned int i = 0; i < Rows; i++) {
        for(unsigned int j = 0; j < Cols; j++) {
            result[j][i] = _data[i][j];
        }
    }

    return result;
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols> Fixed<T, Rows, Cols>::Filled(T value) {
    return Fixed<T, Rows, Cols>(value);
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols> Fixed<T, Rows, Cols>::Zeros() {
    return Fixed<T, Rows, Cols>();
}

template <typename T, unsigned int Rows, unsigned int Cols>
Fixed<T, Rows, Cols> operator*(
    T scalar,
    const Fixed<T, Rows, Cols>& matrix
) {
    return matrix * scalar;
}

} // namespace Foundation::Math::Matrix

#endif // FOUNDATION_MATH_MATRIX_FIXED_TPP
