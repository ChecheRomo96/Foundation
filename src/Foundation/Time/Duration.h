#ifndef FOUNDATION_TIME_DURATION_H
#define FOUNDATION_TIME_DURATION_H

#include <Foundation/Time/Tick.h>
#include <Foundation/Time/Period.h>

namespace Foundation {
    namespace Time {

        /**
         * @brief Count of elapsed ticks without an associated Clock.
         * @ingroup Foundation_Time_Duration
         *
         * Supply the relevant Period when converting ticks to physical units.
         */
        class Duration {
        private:
            Tick _ticks;

        public:

            /** @brief Creates a duration containing `ticks` ticks. */
            constexpr Duration(Tick ticks = 0)
                : _ticks(ticks) {}

            /** @brief Returns the stored tick count. */
            constexpr Tick Ticks() const {
                return _ticks;
            }

            /** @brief Replaces the stored tick count. */
            void SetTicks(Tick ticks) {
                _ticks = ticks;
            }

            /** @brief Reports whether the duration contains zero ticks. */
            bool IsZero() const {
                return _ticks == 0;
            }

            /** @brief Converts this tick count to seconds using `period`. */
            float Seconds(const Period& period) const {
                return static_cast<float>(_ticks) * period.Seconds();
            }

            /** @brief Converts this tick count to milliseconds using `period`. */
            float Milliseconds(const Period& period) const {
                return static_cast<float>(_ticks) * period.Milliseconds();
            }

            /** @brief Converts this tick count to microseconds using `period`. */
            float Microseconds(const Period& period) const {
                return static_cast<float>(_ticks) * period.Microseconds();
            }

            /** @brief Adds two unsigned tick counts. */
            Duration operator+(const Duration& rhs) const {
                return Duration(_ticks + rhs._ticks);
            }

            /**
             * @brief Subtracts one unsigned tick count from another.
             * @warning The result wraps when `rhs` is greater than this value.
             */
            Duration operator-(const Duration& rhs) const {
                return Duration(_ticks - rhs._ticks);
            }

            /** @brief Compares tick counts for equality. */
            bool operator==(const Duration& rhs) const {
                return _ticks == rhs._ticks;
            }

            /** @brief Compares tick counts for inequality. */
            bool operator!=(const Duration& rhs) const {
                return _ticks != rhs._ticks;
            }

            /** @brief Orders durations by their stored tick counts. */
            bool operator<(const Duration& rhs) const {
                return _ticks < rhs._ticks;
            }

            /** @brief Orders durations by their stored tick counts. */
            bool operator>(const Duration& rhs) const {
                return _ticks > rhs._ticks;
            }

            /** @brief Orders durations by their stored tick counts. */
            bool operator<=(const Duration& rhs) const {
                return _ticks <= rhs._ticks;
            }

            /** @brief Orders durations by their stored tick counts. */
            bool operator>=(const Duration& rhs) const {
                return _ticks >= rhs._ticks;
            }
        };
    }
}

#endif
