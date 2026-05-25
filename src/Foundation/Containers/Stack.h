#ifndef FOUNDATION_CONTAINERS_STACK_H
#define FOUNDATION_CONTAINERS_STACK_H

#include <stddef.h>
#include "../Utils/Move.h"

namespace Foundation::Containers {

    template <typename T>
    class Stack {
    private:
        T* _buffer;
        size_t _capacity;
        size_t _count;

    public:
        Stack(T* buffer, size_t capacity)
            : _buffer(buffer), _capacity(capacity), _count(0) { }

        bool Push(const T& value) {
            if (IsFull()) { return false; }
            _buffer[_count++] = value;
            return true;
        }

        bool Push(T&& value) {
            if (IsFull()) { return false; }
            _buffer[_count++] = Foundation::Utils::Move(value);
            return true;
        }

        bool Pop(T& out) {
            if (IsEmpty()) { return false; }
            out = Foundation::Utils::Move(_buffer[--_count]);
            return true;
        }

        size_t GetCount() const { return _count; }
        size_t GetCapacity() const { return _capacity; }
        bool IsEmpty() const { return _count == 0; }
        bool IsFull() const { return _count >= _capacity; }
        void Reset() { _count = 0; }
    };
}

#endif
