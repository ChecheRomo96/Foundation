#ifndef FOUNDATION_MATH_MATRIX_DYNAMIC_H
    #error "Include <Foundation/Math/Matrix/Dynamic.h> instead of Dynamic.tpp"
#elif !defined(FOUNDATION_MATH_MATRIX_DYNAMIC_TPP)
#define FOUNDATION_MATH_MATRIX_DYNAMIC_TPP

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__has_include)
    #if __has_include(<new>)
        #include <new>
        #define FOUNDATION_DETAIL_MATRIX_HAS_NOTHROW_NEW 1
    #endif
#endif

#ifndef FOUNDATION_DETAIL_MATRIX_HAS_NOTHROW_NEW
    #define FOUNDATION_DETAIL_MATRIX_HAS_NOTHROW_NEW 0
#endif

namespace Foundation::Math::Matrix {

namespace Detail {

    template <typename T>
    bool DynamicDimensionsFit(
        unsigned int rows,
        unsigned int cols,
        unsigned int& count
    ) {
        if(rows == 0 || cols == 0) {
            return false;
        }

        const unsigned int maxCount = static_cast<unsigned int>(-1);
        if(rows > maxCount / cols) {
            return false;
        }

        count = rows * cols;

        const size_t maxBytes = static_cast<size_t>(-1);
        const size_t arrayMetadataAllowance = sizeof(size_t);
        return maxBytes > arrayMetadataAllowance &&
            static_cast<size_t>(count) <=
                (maxBytes - arrayMetadataAllowance) / sizeof(T);
    }

    template <typename T>
    T* AllocateDynamicStorage(unsigned int count) {
#if FOUNDATION_DETAIL_MATRIX_HAS_NOTHROW_NEW
        return new (std::nothrow) T[count];
#else
        /*
         * Bare-metal targets without <new> must provide a null-returning array
         * allocation backend. Their toolchain must preserve null checks (for
         * example with GCC's -fcheck-new) and compile without exceptions.
         */
        return new T[count];
#endif
    }

    template <typename T>
    bool DynamicStorageOverlaps(
        const Dynamic<T>& first,
        const Dynamic<T>& second
    ) {
        if(!first.IsValid() || !second.IsValid()) {
            return false;
        }

        const uintptr_t firstAddress =
            reinterpret_cast<uintptr_t>(first.Data());
        const uintptr_t secondAddress =
            reinterpret_cast<uintptr_t>(second.Data());
        const uintptr_t firstBytes =
            static_cast<uintptr_t>(first.Size()) * sizeof(T);
        const uintptr_t secondBytes =
            static_cast<uintptr_t>(second.Size()) * sizeof(T);

        if(firstAddress <= secondAddress) {
            return secondAddress - firstAddress < firstBytes;
        }

        return firstAddress - secondAddress < secondBytes;
    }

}

template <typename T>
Dynamic<T>::Dynamic()
    : _data(0),
      _rows(0),
      _cols(0),
      _ownsData(false) {}

template <typename T>
Dynamic<T>::Dynamic(unsigned int rows, unsigned int cols)
    : _data(0),
      _rows(0),
      _cols(0),
      _ownsData(false) {
    Allocate(rows, cols);
    Zero();
}

template <typename T>
Dynamic<T>::Dynamic(
    unsigned int rows,
    unsigned int cols,
    T value
)
    : _data(0),
      _rows(0),
      _cols(0),
      _ownsData(false) {
    Allocate(rows, cols);
    Fill(value);
}

template <typename T>
Dynamic<T>::Dynamic(
    unsigned int rows,
    unsigned int cols,
    T* externalData
)
    : _data(0),
      _rows(0),
      _cols(0),
      _ownsData(false) {
    Attach(rows, cols, externalData);
}

template <typename T>
Dynamic<T>::Dynamic(const Dynamic<T>& other)
    : _data(0),
      _rows(0),
      _cols(0),
      _ownsData(false) {
    Allocate(other._rows, other._cols);

    for(unsigned int i = 0; i < Size(); i++) {
        _data[i] = other._data[i];
    }
}

template <typename T>
Dynamic<T>::Dynamic(Dynamic<T>&& other) noexcept
    : _data(other._data),
      _rows(other._rows),
      _cols(other._cols),
      _ownsData(other._ownsData) {
    other._data = 0;
    other._rows = 0;
    other._cols = 0;
    other._ownsData = false;
}

template <typename T>
Dynamic<T>::~Dynamic() {
    Release();
}

template <typename T>
Dynamic<T>& Dynamic<T>::operator=(
    const Dynamic<T>& other
) {
    if(this == &other) {
        return *this;
    }

    if(!other.IsValid()) {
        Clear();
        return *this;
    }

    if(_ownsData && IsValid() &&
       _rows == other._rows && _cols == other._cols) {
        for(unsigned int i = 0; i < Size(); i++) {
            _data[i] = other._data[i];
        }

        return *this;
    }

    Dynamic<T> replacement(other);
    if(!replacement.IsValid()) {
        return *this;
    }

    *this = static_cast<Dynamic<T>&&>(replacement);

    return *this;
}

template <typename T>
Dynamic<T>& Dynamic<T>::operator=(
    Dynamic<T>&& other
) noexcept {
    if(this == &other) {
        return *this;
    }

    Release();

    _data = other._data;
    _rows = other._rows;
    _cols = other._cols;
    _ownsData = other._ownsData;

    other._data = 0;
    other._rows = 0;
    other._cols = 0;
    other._ownsData = false;

    return *this;
}

template <typename T>
T* Dynamic<T>::operator[](unsigned int row) {
    return &_data[row * _cols];
}

template <typename T>
const T* Dynamic<T>::operator[](unsigned int row) const {
    return &_data[row * _cols];
}

template <typename T>
void Dynamic<T>::Release() {
    if(_ownsData && _data != 0) {
        delete[] _data;
    }

    _data = 0;
    _rows = 0;
    _cols = 0;
    _ownsData = false;
}

template <typename T>
unsigned int Dynamic<T>::Index(unsigned int row, unsigned int col) const {
    return (row * _cols) + col;
}

template <typename T>
bool Dynamic<T>::Allocate(unsigned int rows, unsigned int cols) {
    unsigned int count = 0;
    if(!Detail::DynamicDimensionsFit<T>(rows, cols, count)) {
        return false;
    }

    if(IsValid() && _rows == rows && _cols == cols) {
        return true;
    }

    T* replacement = Detail::AllocateDynamicStorage<T>(count);
    if(replacement == 0) {
        return false;
    }

    Release();

    _data = replacement;
    _rows = rows;
    _cols = cols;
    _ownsData = true;

    return true;
}

template <typename T>
bool Dynamic<T>::Attach(
    unsigned int rows,
    unsigned int cols,
    T* externalData
) {
    unsigned int count = 0;
    if(externalData == 0 ||
       !Detail::DynamicDimensionsFit<T>(rows, cols, count)) {
        return false;
    }

    if(_ownsData && IsValid()) {
        const uintptr_t ownedAddress = reinterpret_cast<uintptr_t>(_data);
        const uintptr_t externalAddress =
            reinterpret_cast<uintptr_t>(externalData);
        const uintptr_t ownedBytes =
            static_cast<uintptr_t>(Size()) * sizeof(T);

        if(externalAddress >= ownedAddress &&
           externalAddress - ownedAddress < ownedBytes) {
            return false;
        }
    }

    Release();

    _data = externalData;
    _rows = rows;
    _cols = cols;
    _ownsData = false;

    return true;
}

template <typename T>
void Dynamic<T>::Clear() {
    Release();
}

template <typename T>
unsigned int Dynamic<T>::RowCount() const {
    return _rows;
}

template <typename T>
unsigned int Dynamic<T>::ColumnCount() const {
    return _cols;
}

template <typename T>
unsigned int Dynamic<T>::Size() const {
    return _rows * _cols;
}

template <typename T>
bool Dynamic<T>::IsValid() const {
    return _data != 0 && _rows > 0 && _cols > 0;
}

template <typename T>
bool Dynamic<T>::OwnsData() const {
    return _ownsData;
}

template <typename T>
T* Dynamic<T>::Data() {
    return _data;
}

template <typename T>
const T* Dynamic<T>::Data() const {
    return _data;
}

template <typename T>
T& Dynamic<T>::At(unsigned int row, unsigned int col) {
    return _data[Index(row, col)];
}

template <typename T>
const T& Dynamic<T>::At(unsigned int row, unsigned int col) const {
    return _data[Index(row, col)];
}

template <typename T>
Dynamic<T> Dynamic<T>::operator+(
    const Dynamic<T>& other
) const {
    if(!IsValid() || !other.IsValid() ||
       _rows != other._rows || _cols != other._cols) {
        return Dynamic<T>();
    }

    Dynamic<T> result(_rows, _cols);
    if(!result.IsValid()) {
        return result;
    }

    for(unsigned int i = 0; i < Size(); i++) {
        result._data[i] = _data[i] + other._data[i];
    }

    return result;
}

template <typename T>
Dynamic<T> Dynamic<T>::operator-(
    const Dynamic<T>& other
) const {
    if(!IsValid() || !other.IsValid() ||
       _rows != other._rows || _cols != other._cols) {
        return Dynamic<T>();
    }

    Dynamic<T> result(_rows, _cols);
    if(!result.IsValid()) {
        return result;
    }

    for(unsigned int i = 0; i < Size(); i++) {
        result._data[i] = _data[i] - other._data[i];
    }

    return result;
}

template <typename T>
Dynamic<T>& Dynamic<T>::operator+=(
    const Dynamic<T>& other
) {
    if(!IsValid() || !other.IsValid() ||
       _rows != other._rows || _cols != other._cols) {
        return *this;
    }

    for(unsigned int i = 0; i < Size(); i++) {
        _data[i] += other._data[i];
    }

    return *this;
}

template <typename T>
Dynamic<T>& Dynamic<T>::operator-=(
    const Dynamic<T>& other
) {
    if(!IsValid() || !other.IsValid() ||
       _rows != other._rows || _cols != other._cols) {
        return *this;
    }

    for(unsigned int i = 0; i < Size(); i++) {
        _data[i] -= other._data[i];
    }

    return *this;
}

template <typename T>
Dynamic<T> Dynamic<T>::operator*(T scalar) const {
    if(!IsValid()) {
        return Dynamic<T>();
    }

    Dynamic<T> result(_rows, _cols);
    if(!result.IsValid()) {
        return result;
    }

    for(unsigned int i = 0; i < Size(); i++) {
        result._data[i] = _data[i] * scalar;
    }

    return result;
}

template <typename T>
Dynamic<T> Dynamic<T>::operator/(T scalar) const {
    if(!IsValid()) {
        return Dynamic<T>();
    }

    Dynamic<T> result(_rows, _cols);
    if(!result.IsValid()) {
        return result;
    }

    for(unsigned int i = 0; i < Size(); i++) {
        result._data[i] = _data[i] / scalar;
    }

    return result;
}

template <typename T>
Dynamic<T>& Dynamic<T>::operator*=(T scalar) {
    for(unsigned int i = 0; i < Size(); i++) {
        _data[i] *= scalar;
    }

    return *this;
}

template <typename T>
Dynamic<T>& Dynamic<T>::operator/=(T scalar) {
    for(unsigned int i = 0; i < Size(); i++) {
        _data[i] /= scalar;
    }

    return *this;
}

template <typename T>
bool Dynamic<T>::operator==(const Dynamic<T>& other) const {
    if(_rows != other._rows || _cols != other._cols) {
        return false;
    }

    for(unsigned int i = 0; i < Size(); i++) {
        if(_data[i] != other._data[i]) {
            return false;
        }
    }

    return true;
}

template <typename T>
bool Dynamic<T>::operator!=(const Dynamic<T>& other) const {
    return !(*this == other);
}

template <typename T>
void Dynamic<T>::Fill(T value) {
    for(unsigned int i = 0; i < Size(); i++) {
        _data[i] = value;
    }
}

template <typename T>
void Dynamic<T>::Zero() {
    Fill(T());
}

template <typename T>
bool Dynamic<T>::TransposeTo(Dynamic<T>& result) const {
    if(!IsValid() || Detail::DynamicStorageOverlaps(*this, result)) {
        return false;
    }

    if(!result.Allocate(_cols, _rows)) {
        return false;
    }

    for(unsigned int i = 0; i < _rows; i++) {
        for(unsigned int j = 0; j < _cols; j++) {
            result.At(j, i) = At(i, j);
        }
    }

    return true;
}

template <typename T>
Dynamic<T> Dynamic<T>::Transposed() const {
    Dynamic<T> result;
    TransposeTo(result);
    return result;
}

template <typename T>
Dynamic<T> operator*(T scalar, const Dynamic<T>& matrix) {
    return matrix * scalar;
}

} // namespace Foundation::Math::Matrix

#undef FOUNDATION_DETAIL_MATRIX_HAS_NOTHROW_NEW

#endif // FOUNDATION_MATH_MATRIX_DYNAMIC_TPP
