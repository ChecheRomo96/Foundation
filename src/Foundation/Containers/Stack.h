#ifndef FOUNDATION_CONTAINERS_STACK_H
#define FOUNDATION_CONTAINERS_STACK_H

#include <stddef.h>
#include "../Utils/Move.h"

namespace Foundation::Containers {

    /**
     * @brief Fixed-capacity LIFO stack backed by caller-provided storage.
     * @ingroup Foundation_Containers
     * @tparam T Stored value type.
     *
     * Stack never allocates and does not own its storage array.
     */
    template <typename T>
    class Stack {
    private:
        T* _buffer;
        size_t _capacity;
        size_t _count;

    public:
        /**
         * @brief Creates an empty stack over existing storage.
         * @param buffer Array containing at least `capacity` constructed elements.
         * @param capacity Maximum number of values that may be stored.
         * @note A null pointer with zero capacity is a valid empty stack. A
         * null pointer with non-zero capacity creates an invalid but safe
         * stack for which Push() and Pop() return false.
         */
        Stack(T* buffer, size_t capacity)
            : _buffer(buffer), _capacity(capacity), _count(0) { }

        /**
         * @brief Copies one value onto the top of the stack.
         * @return `true` on insertion; `false` when the stack is full.
         */
        bool Push(const T& value) {
            if (!IsValid() || IsFull()) { return false; }
            _buffer[_count++] = value;
            return true;
        }

        /**
         * @brief Move-assigns one value onto the top of the stack.
         * @return `true` on insertion; `false` when the stack is full.
         */
        bool Push(T&& value) {
            if (!IsValid() || IsFull()) { return false; }
            _buffer[_count++] = Foundation::Utils::Move(value);
            return true;
        }

        /**
         * @brief Removes the newest value and move-assigns it to `out`.
         * @param out Destination updated only when a value is available.
         * @return `true` on removal; `false` when the stack is empty.
         */
        bool Pop(T& out) {
            if (!IsValid() || IsEmpty()) { return false; }
            out = Foundation::Utils::Move(_buffer[--_count]);
            return true;
        }

        /** @brief Returns the number of stored values. */
        size_t GetCount() const { return _count; }
        /** @brief Returns the fixed capacity supplied at construction. */
        size_t GetCapacity() const { return _capacity; }
        /** @brief Validates the storage pointer and capacity combination. */
        bool IsValid() const { return _capacity == 0 || _buffer != 0; }
        /** @brief Reports whether no values are stored. */
        bool IsEmpty() const { return _count == 0; }
        /** @brief Reports whether invalid or at capacity. */
        bool IsFull() const { return !IsValid() || _count >= _capacity; }
        /** @brief Clears logical contents without destroying array elements. */
        void Reset() { _count = 0; }
    };
}

#endif
