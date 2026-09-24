#ifndef FOUNDATION_CONTAINERS_CIRCULAR_BUFFER_H
#define FOUNDATION_CONTAINERS_CIRCULAR_BUFFER_H

#include <stddef.h>
#include "../Utils/Move.h"

namespace Foundation::Containers {

    /**
     * @brief Fixed-capacity FIFO with circular storage reuse.
     * @ingroup Foundation_Containers
     * @tparam T Stored value type.
     *
     * CircularBuffer operates on a caller-provided array and never allocates.
     * The array is not owned and must outlive the buffer.
     */
    template <typename T>
    class CircularBuffer {
    private:
        T* _buffer;
        size_t _size;
        size_t _readIndex;
        size_t _writeIndex;
        size_t _available;

    public:
        /**
         * @brief Creates an empty buffer over existing storage.
         * @param buffer Array containing at least `size` constructed elements.
         * @param size Maximum number of values that may be stored.
         * @note A null pointer with zero capacity is a valid empty buffer. A
         * null pointer with non-zero capacity creates an invalid but safe
         * buffer for which Push() and Pop() return false.
         */
        CircularBuffer(T* buffer, size_t size)
            : _buffer(buffer), _size(size), _readIndex(0), _writeIndex(0), _available(0) { }

        /**
         * @brief Copies one value to the back of the buffer.
         * @return `true` on insertion; `false` when the buffer is full.
         */
        bool Push(const T& value) {
            if (!IsValid() || IsFull()) { return false; }
            _buffer[_writeIndex] = value;
            AdvanceWrite();
            return true;
        }

        /**
         * @brief Move-assigns one value to the back of the buffer.
         * @return `true` on insertion; `false` when the buffer is full.
         */
        bool Push(T&& value) {
            if (!IsValid() || IsFull()) { return false; }
            _buffer[_writeIndex] = Foundation::Utils::Move(value);
            AdvanceWrite();
            return true;
        }

        /**
         * @brief Removes the oldest value and move-assigns it to `out`.
         * @param out Destination updated only when a value is available.
         * @return `true` on removal; `false` when the buffer is empty.
         */
        bool Pop(T& out) {
            if (!IsValid() || IsEmpty()) { return false; }
            out = Foundation::Utils::Move(_buffer[_readIndex]);
            AdvanceRead();
            return true;
        }

        /** @brief Returns the number of stored values. */
        size_t GetAvailable() const { return _available; }
        /** @brief Returns available capacity, or zero when invalid. */
        size_t GetFreeSpace() const {
            return IsValid() ? _size - _available : 0;
        }
        /** @brief Returns the fixed capacity supplied at construction. */
        size_t GetSize() const { return _size; }
        /** @brief Validates the storage pointer and capacity combination. */
        bool IsValid() const { return _size == 0 || _buffer != 0; }
        /** @brief Reports whether no values are stored. */
        bool IsEmpty() const { return _available == 0; }
        /** @brief Reports whether invalid or at capacity. */
        bool IsFull() const { return !IsValid() || _available >= _size; }

        /**
         * @brief Clears logical contents and resets both indices.
         * @note Existing array elements are not destroyed or overwritten.
         */
        void Reset() {
            _readIndex = 0;
            _writeIndex = 0;
            _available = 0;
        }

    private:
        void AdvanceWrite() {
            _writeIndex++;
            if (_writeIndex >= _size) { _writeIndex = 0; }
            _available++;
        }

        void AdvanceRead() {
            _readIndex++;
            if (_readIndex >= _size) { _readIndex = 0; }
            _available--;
        }
    };
}

#endif
