#ifndef FOUNDATION_TIME_DURATION_H
#define FOUNDATION_TIME_DURATION_H

#include <Foundation/Time/Tick.h>
#include <Foundation/Time/Period.h>

namespace Foundation {
    namespace Time {

        /**
         * @brief Validated count of elapsed ticks without an associated Clock.
         * @ingroup Foundation_Time_Duration
         * @tparam TickType Tick specialization that defines the representation.
         *
         * A valid duration is shorter than half of the TickType modular range.
         * This is the interval in which wrapped counter values can be ordered
         * unambiguously. Supply the relevant Period when converting ticks to
         * physical units.
         */
        template <typename TickType = Tick32>
        class BasicDuration {
        public:
            using Representation = typename TickType::Representation;

        private:
            Representation _ticks;

            static constexpr Representation Normalize(
                Representation ticks
            ) noexcept {
                return ticks < TickType::HalfRange()
                    ? ticks
                    : TickType::MaximumValue();
            }

        public:

            /** @brief Creates a duration containing `ticks` ticks. */
            constexpr BasicDuration(Representation ticks = 0) noexcept
                : _ticks(Normalize(ticks)) {}

            /** @brief Creates the invalid Duration value. */
            static constexpr BasicDuration Invalid() noexcept {
                return BasicDuration(
                    TickType::MaximumValue(),
                    InvalidTag()
                );
            }

            /** @brief Returns the largest unambiguously comparable duration. */
            static constexpr Representation MaximumTicks() noexcept {
                return static_cast<Representation>(
                    TickType::HalfRange() - 1
                );
            }

            /** @brief Reports whether this duration can participate in arithmetic. */
            constexpr bool IsValid() const noexcept {
                return _ticks < TickType::HalfRange();
            }

            /** @brief Returns the stored tick count. */
            constexpr Representation Ticks() const noexcept {
                return _ticks;
            }

            /** @brief Replaces the stored tick count. */
            constexpr void SetTicks(Representation ticks) noexcept {
                _ticks = Normalize(ticks);
            }

            /** @brief Reports whether the duration contains zero ticks. */
            constexpr bool IsZero() const noexcept {
                return IsValid() && (_ticks == 0);
            }

            /** @brief Converts this tick count to seconds using `period`. */
            constexpr float Seconds(const Period& period) const noexcept {
                return IsValid()
                    ? static_cast<float>(_ticks) * period.Seconds()
                    : 0.0f;
            }

            /** @brief Converts this tick count to milliseconds using `period`. */
            constexpr float Milliseconds(const Period& period) const noexcept {
                return IsValid()
                    ? static_cast<float>(_ticks) * period.Milliseconds()
                    : 0.0f;
            }

            /** @brief Converts this tick count to microseconds using `period`. */
            constexpr float Microseconds(const Period& period) const noexcept {
                return IsValid()
                    ? static_cast<float>(_ticks) * period.Microseconds()
                    : 0.0f;
            }

            /** @brief Adds durations, returning invalid on range overflow. */
            constexpr BasicDuration operator+(
                const BasicDuration& rhs
            ) const noexcept {
                if (!IsValid() || !rhs.IsValid()) {
                    return Invalid();
                }

                if (rhs._ticks > MaximumTicks() - _ticks) {
                    return Invalid();
                }

                return BasicDuration(
                    static_cast<Representation>(_ticks + rhs._ticks)
                );
            }

            /**
             * @brief Subtracts durations, returning invalid on underflow.
             */
            constexpr BasicDuration operator-(
                const BasicDuration& rhs
            ) const noexcept {
                if (!IsValid() || !rhs.IsValid() || rhs._ticks > _ticks) {
                    return Invalid();
                }

                return BasicDuration(
                    static_cast<Representation>(_ticks - rhs._ticks)
                );
            }

            /** @brief Compares states and tick counts for equality. */
            constexpr bool operator==(
                const BasicDuration& rhs
            ) const noexcept {
                return _ticks == rhs._ticks;
            }

            /** @brief Compares states and tick counts for inequality. */
            constexpr bool operator!=(
                const BasicDuration& rhs
            ) const noexcept {
                return _ticks != rhs._ticks;
            }

            /** @brief Orders valid durations by their stored tick counts. */
            constexpr bool operator<(
                const BasicDuration& rhs
            ) const noexcept {
                return IsValid() && rhs.IsValid() && (_ticks < rhs._ticks);
            }

            /** @brief Orders valid durations by their stored tick counts. */
            constexpr bool operator>(
                const BasicDuration& rhs
            ) const noexcept {
                return IsValid() && rhs.IsValid() && (_ticks > rhs._ticks);
            }

            /** @brief Orders valid durations by their stored tick counts. */
            constexpr bool operator<=(
                const BasicDuration& rhs
            ) const noexcept {
                return IsValid() && rhs.IsValid() && (_ticks <= rhs._ticks);
            }

            /** @brief Orders valid durations by their stored tick counts. */
            constexpr bool operator>=(
                const BasicDuration& rhs
            ) const noexcept {
                return IsValid() && rhs.IsValid() && (_ticks >= rhs._ticks);
            }

        private:
            struct InvalidTag {};

            constexpr BasicDuration(
                Representation ticks,
                InvalidTag
            ) noexcept : _ticks(ticks) {}
        };

        /** @brief Default 32-bit Duration specialization. */
        using Duration = BasicDuration<>;
    }
}

#endif
