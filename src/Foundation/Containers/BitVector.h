#ifndef FOUNDATION_CONTAINERS_BIT_VECTOR_H
#define FOUNDATION_CONTAINERS_BIT_VECTOR_H

#include <stddef.h>
#include <stdint.h>

#include <CPutility.h>
#include <CPvector.h>

#if !defined(CPSTL_VECTOR_ENABLED) && !defined(DOXYGEN)
    #error "Foundation::Containers::BitVector needs cpstd::vector: enable CPSTL_VECTOR"
#endif

namespace Foundation { namespace Containers {

    /**
     * @brief Growable sequence of bits packed eight per byte.
     * @ingroup Foundation_Containers
     *
     * Bits are read with Get() and written with Set(): a single bit has no
     * address, so there is no `bool&`. Bit `i` is bit `i % 8` of byte `i / 8`
     * in Data(). Bits past GetCount() are always zero.
     *
     * The bytes live in one of two places:
     *
     * - **Owned:** a `cpstd::vector<uint8_t>`, the default. It grows on
     *   demand (PushBack() lets the vector grow its capacity). An allocation
     *   failure follows `cpstd::vector`: with CPSTL's own implementation the
     *   operation returns `false` and leaves the bits unchanged; in CPSTL's
     *   STL mode it is `std::vector`'s behavior.
     * - **External:** a caller buffer handed in with a non-null pointer
     *   (constructor or Attach()). It is used as is, never grows, and is never
     *   released by the bit vector; an operation that needs more bytes than it
     *   holds returns `false` and leaves the bits unchanged.
     *
     * Implementers who change the bits in time-critical code reserve the
     * space beforehand (Reserve() or an external buffer). Capacity counts
     * whole bytes, so it is a multiple of eight bits.
     */
    class BitVector {
    private:
        cpstd::vector<uint8_t> _owned;  // holds exactly BytesFor(_count) bytes when owned
        uint8_t* _external;             // caller buffer; null means owned storage
        size_t _externalBytes;
        size_t _count;                  // bits

        static uint8_t Mask(size_t index) {
            return static_cast<uint8_t>(1u << (index % 8u));
        }

        uint8_t* Bytes() { return _external != nullptr ? _external : _owned.data(); }
        const uint8_t* Bytes() const { return _external != nullptr ? _external : _owned.data(); }

        // Number of bytes that hold the current bits.
        size_t UsedBytes() const { return BytesFor(_count); }

        size_t ByteCapacity() const {
            return _external != nullptr ? _externalBytes : _owned.capacity();
        }

        // Makes `bytes` bytes available for the bits, filling new ones with
        // `fill`. `false` and unchanged when the storage cannot hold them.
        bool SetUsedBytes(size_t bytes, uint8_t fill) {
            const size_t used = UsedBytes();
            if (_external != nullptr) {
                if (bytes > _externalBytes) {
                    return false;
                }
                for (size_t i = used; i < bytes; ++i) {
                    _external[i] = fill;
                }
                return true;
            }
            if (bytes == _owned.size()) {
                return true;
            }
            if (bytes == _owned.size() + 1u) {
                _owned.push_back(fill);  // lets the vector grow its capacity geometrically
            } else {
                _owned.resize(bytes, fill);
            }
            return _owned.size() == bytes;
        }

        // Clears the bits from `first` to the end of the byte holding bit `first - 1`.
        void ClearTail(size_t first) {
            if (first % 8u != 0u) {
                uint8_t& byte = Bytes()[first / 8u];
                byte = static_cast<uint8_t>(byte & static_cast<uint8_t>(Mask(first) - 1u));
            }
        }

        void Write(size_t index, bool value) {
            uint8_t& byte = Bytes()[index / 8u];
            byte = value ? static_cast<uint8_t>(byte | Mask(index))
                         : static_cast<uint8_t>(byte & static_cast<uint8_t>(~Mask(index)));
        }

        bool Read(size_t index) const {
            return (static_cast<unsigned int>(Bytes()[index / 8u]) & Mask(index)) != 0u;
        }

        // Copies `other`'s bits into this storage; `false` and unchanged when they do not fit.
        bool CopyBits(const BitVector& other) {
            const size_t bytes = other.UsedBytes();
            if (_external != nullptr) {
                if (bytes > _externalBytes) {
                    return false;
                }
                for (size_t i = bytes; i < UsedBytes(); ++i) {
                    _external[i] = 0u;  // bytes no longer in use
                }
            } else {
                _owned.resize(bytes);  // reuses the capacity when it is enough
                if (_owned.size() != bytes) {
                    return false;
                }
            }
            const uint8_t* source = other.Bytes();
            uint8_t* destination = Bytes();
            for (size_t i = 0; i < bytes; ++i) {
                destination[i] = source[i];
            }
            _count = other._count;
            return true;
        }

    public:
        /** @brief Bytes needed to store `bits` bits; use it to size buffers. */
        static constexpr size_t BytesFor(size_t bits) {
            return bits / 8u + (bits % 8u != 0u ? 1u : 0u);
        }

        /** @brief Creates an empty bit vector with owned (not yet allocated) storage. */
        BitVector() : _owned(), _external(nullptr), _externalBytes(0), _count(0) { }

        /**
         * @brief Creates an empty bit vector over `bytes` bytes at `buffer`
         * when `buffer` is not null, or with owned storage otherwise; see
         * Attach().
         */
        BitVector(uint8_t* buffer, size_t bytes)
            : _owned(),
              _external(buffer),
              _externalBytes(buffer != nullptr ? bytes : 0u),
              _count(0) { }

        /** @brief Copies `other`'s bits into owned storage; empty when it cannot be allocated. */
        BitVector(const BitVector& other)
            : _owned(), _external(nullptr), _externalBytes(0), _count(0) {
            CopyBits(other);
        }

        /** @brief Takes `other`'s storage, owned or external, and leaves it empty and owned. */
        BitVector(BitVector&& other) noexcept
            : _owned(cpstd::move(other._owned)),
              _external(other._external),
              _externalBytes(other._externalBytes),
              _count(other._count) {
            other._external = nullptr;
            other._externalBytes = 0;
            other._count = 0;
        }

        /** @brief Copies `other`'s bits; see Assign(). */
        BitVector& operator=(const BitVector& other) {
            Assign(other);
            return *this;
        }

        /**
         * @brief Copies `other`'s bits into the current storage; owned storage
         * grows when it is too small. `false` and unchanged when they cannot be
         * stored.
         */
        bool Assign(const BitVector& other) {
            if (this == &other) {
                return true;
            }
            return CopyBits(other);
        }

        /**
         * @brief Takes `other`'s storage and leaves it empty. A bit vector on
         * an external buffer instead copies the bits into that buffer and is
         * unchanged when they do not fit.
         */
        BitVector& operator=(BitVector&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            if (_external != nullptr) {
                CopyBits(other);
                return *this;
            }
            _owned = cpstd::move(other._owned);
            _external = other._external;
            _externalBytes = other._externalBytes;
            _count = other._count;
            other._external = nullptr;
            other._externalBytes = 0;
            other._count = 0;
            return *this;
        }

        /**
         * @brief Selects the storage and leaves the bit vector empty.
         *
         * A non-null `buffer` is used as external storage of `bytes` bytes:
         * owned storage is released, the buffer is not zeroed, and it must
         * outlive the attachment. A null `buffer` detaches any external buffer
         * and returns to empty owned storage. Returns `false` and changes
         * nothing when `buffer` is this bit vector's own owned storage.
         */
        bool Attach(uint8_t* buffer, size_t bytes) {
            if (buffer != nullptr && _external == nullptr &&
                _owned.capacity() != 0u && buffer == _owned.data()) {
                return false;
            }
            _owned = cpstd::vector<uint8_t>();
            _external = buffer;
            _externalBytes = buffer != nullptr ? bytes : 0u;
            _count = 0;
            return true;
        }

        /** @brief Removes every bit, keeping the storage. */
        void Clear() {
            if (_external == nullptr) {
                _owned.clear();
            } else {
                for (size_t i = 0; i < UsedBytes(); ++i) {
                    _external[i] = 0u;
                }
            }
            _count = 0;
        }

        /** @brief Releases owned storage or detaches the external buffer, leaving an empty owned bit vector. */
        void Release() {
            _owned = cpstd::vector<uint8_t>();
            _external = nullptr;
            _externalBytes = 0;
            _count = 0;
        }

        /** @brief Ensures room for `bits` bits; `false` and unchanged on failure. */
        bool Reserve(size_t bits) {
            const size_t bytes = BytesFor(bits);
            if (_external != nullptr) {
                return bytes <= _externalBytes;
            }
            if (bytes > _owned.capacity()) {
                _owned.reserve(bytes);
            }
            return _owned.capacity() >= bytes;
        }

        /**
         * @brief Changes the count. New bits are `value`; removed bits are
         * cleared. `false` and unchanged on failure.
         */
        bool Resize(size_t bits, bool value = false) {
            const size_t previous = _count;
            const size_t previousBytes = UsedBytes();
            const size_t bytes = BytesFor(bits);
            if (_external != nullptr && bytes > _externalBytes) {
                return false;
            }
            if (!SetUsedBytes(bytes, value ? uint8_t(0xFFu) : uint8_t(0u))) {
                return false;
            }
            if (_external != nullptr) {
                for (size_t i = bytes; i < previousBytes; ++i) {
                    _external[i] = 0u;
                }
            }
            _count = bits;
            // Bits that were already stored in the previous last byte.
            const size_t storedEnd = previousBytes * 8u;
            for (size_t i = previous; i < bits && i < storedEnd; ++i) {
                Write(i, value);
            }
            ClearTail(bits);
            return true;
        }

        /** @brief Appends one bit; `false` and unchanged on failure. */
        bool PushBack(bool value) {
            if (_count == static_cast<size_t>(-1)) {
                return false;
            }
            if (_count % 8u == 0u && !SetUsedBytes(UsedBytes() + 1u, uint8_t(0u))) {
                return false;
            }
            Write(_count, value);
            ++_count;
            return true;
        }

        /** @brief Removes the last bit; `false` when empty. */
        bool PopBack() {
            bool ignored = false;
            return PopBack(ignored);
        }

        /** @brief Copies the last bit into `out` and removes it; `false` and `out` untouched when empty. */
        bool PopBack(bool& out) {
            if (_count == 0u) {
                return false;
            }
            out = Read(_count - 1u);
            Write(_count - 1u, false);
            --_count;
            if (_count % 8u == 0u && _external == nullptr) {
                _owned.pop_back();
            }
            return true;
        }

        /**
         * @brief Asks owned storage to shrink to the bytes in use, as
         * `cpstd::vector::shrink_to_fit()` does; external buffers are
         * unchanged. Always returns `true`.
         */
        bool ShrinkToFit() {
            if (_external == nullptr) {
                _owned.shrink_to_fit();
            }
            return true;
        }

        /** @brief Returns bit `index`, or `false` when `index` is out of range. */
        bool Get(size_t index) const { return index < _count && Read(index); }

        /** @brief Writes bit `index`; `false` and unchanged when out of range. */
        bool Set(size_t index, bool value) {
            if (index >= _count) {
                return false;
            }
            Write(index, value);
            return true;
        }

        /** @brief Inverts bit `index`; `false` when out of range. */
        bool Flip(size_t index) {
            if (index >= _count) {
                return false;
            }
            Write(index, !Read(index));
            return true;
        }

        /** @brief Inverts every bit. */
        void FlipAll() {
            uint8_t* bytes = Bytes();
            for (size_t i = 0; i < UsedBytes(); ++i) {
                bytes[i] = static_cast<uint8_t>(~bytes[i]);
            }
            ClearTail(_count);
        }

        /** @brief Returns how many bits are set. */
        size_t CountOnes() const {
            const uint8_t* bytes = Bytes();
            size_t ones = 0;
            for (size_t i = 0; i < UsedBytes(); ++i) {
                uint8_t remaining = bytes[i];
                while (remaining != 0u) {
                    remaining = static_cast<uint8_t>(remaining & (remaining - 1u));
                    ++ones;
                }
            }
            return ones;
        }

        /** @brief Returns the number of bits. */
        size_t GetCount() const { return _count; }

        /** @brief Returns how many bits fit without reallocating, or in the external buffer. */
        size_t GetCapacity() const {
            const size_t bytes = ByteCapacity();
            return bytes > static_cast<size_t>(-1) / 8u ? static_cast<size_t>(-1) : bytes * 8u;
        }

        /** @brief Returns the number of bytes holding the bits. */
        size_t GetByteCount() const { return UsedBytes(); }

        /** @brief Reports whether no bits are stored. */
        bool IsEmpty() const { return _count == 0u; }

        /** @brief Reports whether the bits live in an external buffer. */
        bool IsExternal() const { return _external != nullptr; }

        /** @brief Reports whether the bit vector holds allocated owned storage. */
        bool OwnsStorage() const { return _external == nullptr && _owned.capacity() != 0u; }

        /** @brief Returns the packed bytes, or null when there is no storage. */
        const uint8_t* Data() const {
            if (_external != nullptr) {
                return _external;
            }
            return _owned.capacity() != 0u ? _owned.data() : nullptr;
        }

        /** @brief Same count and bits; storage is ignored. */
        friend bool operator==(const BitVector& a, const BitVector& b) {
            if (a._count != b._count) {
                return false;
            }
            const uint8_t* left = a.Bytes();
            const uint8_t* right = b.Bytes();
            for (size_t i = 0; i < a.UsedBytes(); ++i) {
                if (left[i] != right[i]) {
                    return false;
                }
            }
            return true;
        }

        /** @brief Negation of `operator==`. */
        friend bool operator!=(const BitVector& a, const BitVector& b) { return !(a == b); }
    };

}}

#endif
