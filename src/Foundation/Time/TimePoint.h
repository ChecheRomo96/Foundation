#ifndef FOUNDATION_TIME_TIMEPOINT_H
#define FOUNDATION_TIME_TIMEPOINT_H

    #include <Foundation/Time/Tick.h>
    #include <Foundation/Time/Duration.h>

    namespace Foundation {
        namespace Time {

            class Clock;

            /**
             * @brief Tick value associated with the identity of one Clock.
             * @ingroup Foundation_Time_TimePoint
             *
             * The Clock pointer is non-owning and is used for compatibility
             * checks; it must outlive every operation that inspects the point.
             */
            class TimePoint {
            private:
                Tick _ticks;
                const Clock* _clock;

            public:

                /**
                 * @brief Creates a point from a tick value and optional Clock identity.
                 * @param ticks Stored tick value.
                 * @param clock Non-owning Clock pointer; null creates an invalid point.
                 */
                constexpr TimePoint(
                    Tick ticks = 0,
                    const Clock* clock = nullptr
                )
                    : _ticks(ticks),
                    _clock(clock) {}

                /** @brief Returns the stored tick value. */
                constexpr Tick Ticks() const {
                    return _ticks;
                }

                /** @brief Returns the non-owning Clock identity, which may be null. */
                const Clock* GetClock() const {
                    return _clock;
                }

                /** @brief Reports whether this point has a non-null Clock identity. */
                bool IsValid() const {
                    return _clock != nullptr;
                }

                /**
                 * @brief Returns the unsigned tick difference between compatible points.
                 * @return Zero Duration when the Clock identities differ.
                 * @warning Compatible subtraction wraps if `rhs` is later than this point.
                 */
                Duration operator-(const TimePoint& rhs) const {
                    if (_clock != rhs._clock) {
                        return Duration(0);
                    }

                    return Duration(_ticks - rhs._ticks);
                }

                /** @brief Adds an unsigned tick Duration while preserving Clock identity. */
                TimePoint operator+(const Duration& duration) const {
                    return TimePoint(_ticks + duration.Ticks(), _clock);
                }

                /**
                 * @brief Subtracts an unsigned tick Duration while preserving Clock identity.
                 * @warning The tick value wraps when the duration is larger.
                 */
                TimePoint operator-(const Duration& duration) const {
                    return TimePoint(_ticks - duration.Ticks(), _clock);
                }

                /** @brief Reports whether two points store the same Clock pointer. */
                bool SameClock(const TimePoint& rhs) const {
                    return _clock == rhs._clock;
                }

                /** @brief Compares Clock identity and tick value for equality. */
                bool operator==(const TimePoint& rhs) const {
                    return (_clock == rhs._clock) &&
                        (_ticks == rhs._ticks);
                }

                /** @brief Negates equality. */
                bool operator!=(const TimePoint& rhs) const {
                    return !(*this == rhs);
                }

                /** @brief Orders tick values only when Clock identities match. */
                bool operator<(const TimePoint& rhs) const {
                    return SameClock(rhs) && (_ticks < rhs._ticks);
                }

                /** @brief Orders tick values only when Clock identities match. */
                bool operator>(const TimePoint& rhs) const {
                    return rhs < *this;
                }

                /** @brief Orders tick values only when Clock identities match. */
                bool operator<=(const TimePoint& rhs) const {
                    return SameClock(rhs) && (_ticks <= rhs._ticks);
                }

                /** @brief Orders tick values only when Clock identities match. */
                bool operator>=(const TimePoint& rhs) const {
                    return SameClock(rhs) && (_ticks >= rhs._ticks);
                }
            };
        }
    }

#endif
