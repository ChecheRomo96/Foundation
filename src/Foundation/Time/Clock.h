#ifndef FOUNDATION_TIME_CLOCK_H
#define FOUNDATION_TIME_CLOCK_H

    #include <Foundation/Time/Tick.h>
    #include <Foundation/Time/Frequency.h>
    #include <Foundation/Time/Period.h>
    #include <Foundation/Time/TimePoint.h>
    #include <Foundation/Functional/Callback.h>

    namespace Foundation {
        namespace Time {

            /**
             * @brief Callback-backed tick source with an associated Frequency.
             * @ingroup Foundation_Time_Clock
             *
             * Clock does not own external state used by the tick callback.
             * TimePoints returned by Now() retain this Clock's address as their
             * identity, so the Clock must outlive those TimePoints.
             */
            class Clock {
            public:
                /** @brief Free/static function returning the current Tick. */
                using Callback = Tick (*)();

            private:
                Functional::Callback<Tick> _callback;
                Frequency _frequency;

            public:

                /** @brief Creates an unbound clock with a zero frequency. */
                Clock()
                    : _frequency(0, 1) {}

                /**
                 * @brief Creates a clock bound to a tick source.
                 * @param callback Free/static function returning the current tick.
                 * @param frequency Number of callback ticks per second.
                 */
                Clock( Callback callback, const Frequency& frequency )
                    : _frequency(frequency) {
                    _callback.bind(callback);
                }

                /**
                 * @brief Samples the bound callback and associates the result with this Clock.
                 * @return A TimePoint containing the sampled tick, or tick zero when unbound.
                 * @note The returned point still references this Clock when unbound.
                 */
                TimePoint Now() const {
                    return TimePoint(
                        _callback.status() ? _callback.invoke() : 0,
                        this
                    );
                }

                /** @brief Reports whether a tick callback is currently bound. */
                bool IsBound() const {
                    return _callback.status();
                }

                /** @brief Binds or replaces the free/static tick callback. */
                void Bind(Callback callback) {
                    _callback.bind(callback);
                }

                /** @brief Removes the tick callback without changing frequency. */
                void Unbind() {
                    _callback.unbind();
                }

                /** @brief Returns the configured ticks-per-second value. */
                Frequency GetFrequency() const {
                    return _frequency;
                }

                /** @brief Returns the reciprocal seconds-per-tick value. */
                Period GetPeriod() const {
                    return _frequency.GetPeriod();
                }

                /** @brief Replaces the ticks-per-second value. */
                void SetFrequency(const Frequency& frequency) {
                    _frequency = frequency;
                }
            };
        }
    }

#endif
