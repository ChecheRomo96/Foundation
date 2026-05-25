#ifndef FOUNDATION_CONTAINERS_CIRCULAR_BUFFER_H
#define FOUNDATION_CONTAINERS_CIRCULAR_BUFFER_H

#include <stddef.h>
#include "../Utils/Move.h"

namespace Foundation::Containers {

    template <typename T>
    class CircularBuffer {
    private:
        T* _buffer;
        size_t _size;
        size_t _readIndex;
        size_t _writeIndex;
        size_t _available;

    public:
        CircularBuffer(T* buffer, size_t size)
            : _buffer(buffer), _size(size), _readIndex(0), _writeIndex(0), _available(0) { }

        bool Push(const T& value) {
            if (IsFull()) { return false; }
            _buffer[_writeIndex] = value;
            AdvanceWrite();
            return true;
        }

        bool Push(T&& value) {
            if (IsFull()) { return false; }
            _buffer[_writeIndex] = Foundation::Utils::Move(value);
            AdvanceWrite();
            return true;
        }

        bool Pop(T& out) {
            if (IsEmpty()) { return false; }
            out = Foundation::Utils::Move(_buffer[_readIndex]);
            AdvanceRead();
            return true;
        }

        size_t GetAvailable() const { return _available; }
        size_t GetFreeSpace() const { return _size - _available; }
        size_t GetSize() const { return _size; }
        bool IsEmpty() const { return _available == 0; }
        bool IsFull() const { return _available >= _size; }

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
