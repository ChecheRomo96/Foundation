#ifndef FOUNDATION_TIME_TICK_H
#define FOUNDATION_TIME_TICK_H

#include <stdint.h>
#include <Foundation/TypeTraits/TypeTraits.h>

namespace Foundation {
    namespace Time {

        /**
         * @brief Strongly typed wrapping value for an unsigned hardware counter.
         * @ingroup Foundation_Time_Tick
         * @tparam T Unsigned integer representation; defaults to `uint32_t`
         * and rejects `bool`.
         *
         * Every bit pattern of the representation is a valid raw counter
         * value. Elapsed-time validity and modular ordering belong to
         * BasicDuration and BasicTimePoint rather than to the raw Tick.
         */
        template <typename T = uint32_t>
        class Tick {
        public:
            using Representation =
                Foundation::TypeTraits::remove_cv_t<T>;

        private:
            static_assert(
                Foundation::TypeTraits::is_unsigned_integer<
                    Representation
                >::value,
                "Tick requires an unsigned integer representation"
            );

            Representation _value;

        public:
            /** @brief Creates a tick from a raw counter value. */
            constexpr Tick(Representation value = 0)
                : _value(value) {}

            /** @brief Returns the raw counter value. */
            constexpr Representation Value() const {
                return _value;
            }

            /** @brief Replaces the raw counter value. */
            void SetValue(Representation value) {
                _value = value;
            }

            /** @brief Returns the largest value supported by Representation. */
            static constexpr Representation MaximumValue() {
                return static_cast<Representation>(
                    ~static_cast<Representation>(0)
                );
            }

            /**
             * @brief Returns half of the counter's modular range.
             *
             * Two counter values can be ordered unambiguously only when their
             * distance is strictly less than this value.
             */
            static constexpr Representation HalfRange() {
                return static_cast<Representation>(
                    (MaximumValue() / 2) + 1
                );
            }

            /** @brief Compares raw counter values for equality. */
            constexpr bool operator==(const Tick& rhs) const {
                return _value == rhs._value;
            }

            /** @brief Compares raw counter values for inequality. */
            constexpr bool operator!=(const Tick& rhs) const {
                return !(*this == rhs);
            }
        };

        /** @brief Unsigned 8-bit Tick specialization. */
        using Tick8 = Tick<uint8_t>;

        /** @brief Unsigned 16-bit Tick specialization. */
        using Tick16 = Tick<uint16_t>;

        /** @brief Unsigned 32-bit Tick specialization, equivalent to `Tick<>`. */
        using Tick32 = Tick<uint32_t>;

        /** @brief Unsigned 64-bit Tick specialization. */
        using Tick64 = Tick<uint64_t>;

    }
}

#endif
