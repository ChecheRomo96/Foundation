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
             * @tparam TickType Tick specialization returned by this clock.
             *
             * Clock does not own external state used by the tick callback.
             * TimePoints returned by Now() retain this Clock's address as their
             * identity, so the Clock must outlive those TimePoints.
             */
            template <typename TickType = Tick32>
            class BasicClock {
            public:
                using Representation = typename TickType::Representation;
                using TimePointType = BasicTimePoint<TickType>;

                /** @brief Free/static function returning the raw counter value. */
                using Callback = Representation (*)();

            private:
                Functional::Callback<Representation> _callback;
                Frequency _frequency;

            public:

                /** @brief Creates an unbound clock with a zero frequency. */
                BasicClock()
                    : _frequency(0, 1) {}

                /**
                 * @brief Creates a clock bound to a tick source.
                 * @param callback Free/static function returning the current tick.
                 * @param frequency Number of callback ticks per second.
                 */
                BasicClock(Callback callback, const Frequency& frequency)
                    : _frequency(frequency) {
                    _callback.Bind(callback);
                }

                /**
                 * @brief Samples the bound callback and associates the result with this Clock.
                 * @return A TimePoint containing the sampled tick, or tick zero when unbound.
                 * @note The returned point still references this Clock when unbound.
                 */
                TimePointType Now() const {
                    return TimePointType(
                        _callback.IsBound() ? _callback.Invoke() : 0,
                        this
                    );
                }

                /** @brief Reports whether a tick callback is currently bound. */
                bool IsBound() const {
                    return _callback.IsBound();
                }

                /** @brief Binds or replaces the free/static tick callback. */
                void Bind(Callback callback) {
                    _callback.Bind(callback);
                }

                /** @brief Removes the tick callback without changing frequency. */
                void Unbind() {
                    _callback.Unbind();
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

            /** @brief Default 32-bit Clock specialization. */
            using Clock = BasicClock<>;
        }
    }

#endif
