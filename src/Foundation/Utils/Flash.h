#ifndef FOUNDATION_FLASH_H
#define FOUNDATION_FLASH_H

    #include <Foundation_BuildSettings.h>
    #include <stddef.h>
    #include <string.h>

    #if defined(__AVR__)
        #include <avr/pgmspace.h>
    #endif

    /**
     * @brief Places a namespace-scope `const` object in program memory.
     * @ingroup Foundation_Utils
     *
     * On AVR this expands to `PROGMEM`, so the object stays in flash instead
     * of being copied to RAM at startup. Such an object must only be read
     * through Foundation::Utils::Flash. On every other target, where constant
     * data is already addressable in place, it expands to nothing.
     *
     * @code{.cpp}
     * const uint16_t Table[] FOUNDATION_FLASH = {1, 2, 3};
     * uint16_t second = Foundation::Utils::Flash::Read(&Table[1]);
     * @endcode
     */
    #if defined(__AVR__)
        #define FOUNDATION_FLASH PROGMEM
    #else
        #define FOUNDATION_FLASH
    #endif

    namespace Foundation { namespace Utils { namespace Flash {

        /**
         * @brief Copies bytes from an object declared with FOUNDATION_FLASH.
         * @ingroup Foundation_Utils
         * @param destination RAM destination of at least `size` bytes.
         * @param source Program-memory source of at least `size` bytes.
         * @param size Number of bytes to copy.
         */
        inline void Copy(void* destination, const void* source, size_t size) noexcept {
            #if defined(__AVR__)
                memcpy_P(destination, source, size);
            #else
                memcpy(destination, source, size);
            #endif
        }

        /**
         * @brief Reads one object declared with FOUNDATION_FLASH by value.
         * @ingroup Foundation_Utils
         * @tparam T Trivially copyable, default-constructible type.
         * @param source Address of the object in program memory.
         * @return A RAM copy of the object.
         */
        template <typename T>
        T Read(const T* source) noexcept {
            static_assert(__is_trivially_copyable(T),
                "Flash::Read requires a trivially copyable type");
            T value;
            Copy(&value, source, sizeof(T));
            return value;
        }

        /**
         * @brief Returns the length of a string declared with FOUNDATION_FLASH.
         * @ingroup Foundation_Utils
         * @param source Null-terminated program-memory string.
         * @return The number of characters before the terminator.
         */
        inline size_t StringLength(const char* source) noexcept {
            #if defined(__AVR__)
                return strlen_P(source);
            #else
                return strlen(source);
            #endif
        }

        /**
         * @brief Copies a string declared with FOUNDATION_FLASH into a buffer.
         * @ingroup Foundation_Utils
         *
         * Follows `snprintf` semantics: at most `capacity - 1` characters are
         * written followed by a terminator, and the full source length is
         * returned so that truncation can be detected. A zero capacity writes
         * nothing.
         *
         * @param destination RAM buffer of `capacity` bytes; may be null only
         *        when `capacity` is zero.
         * @param capacity Size of `destination` in bytes.
         * @param source Null-terminated program-memory string.
         * @return The length of `source`.
         */
        inline size_t CopyString(char* destination, size_t capacity, const char* source) noexcept {
            const size_t length = StringLength(source);
            if (capacity != 0) {
                const size_t count = length < capacity ? length : capacity - 1;
                Copy(destination, source, count);
                destination[count] = '\0';
            }
            return length;
        }

    }}}

#endif//FOUNDATION_FLASH_H
