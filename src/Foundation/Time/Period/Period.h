// Period.h

#ifndef FOUNDATION_TIME_PERIOD_H
#define FOUNDATION_TIME_PERIOD_H

#include <Foundation/Math/Ratio.h>

namespace Foundation {
    namespace Time {

        class Frequency;

        /**
         * @brief Rational number of seconds represented by one tick.
         * @ingroup Foundation_Time_Period
         *
         * Period is backed by Math::UnsignedRatio and may be converted to its
         * reciprocal Frequency. Both terms must be non-zero for a Period to be
         * valid, ensuring every valid value has a finite, representable
         * reciprocal.
         */
        class Period {
        private:
            Foundation::Math::UnsignedRatio _ratio;

        public:

            /**
             * @brief Creates a period of `num / den` seconds per tick.
             * @param num Period numerator; zero creates an invalid value.
             * @param den Period denominator; zero creates an invalid ratio.
             */
            Period(
                uint32_t num = 0,
                uint32_t den = 1
            );

            /** @brief Creates a period from an existing unsigned ratio. */
            Period(
                const Foundation::Math::UnsignedRatio& ratio
            );

            /** @brief Returns the period numerator. */
            uint32_t Num() const;

            /** @brief Returns the period denominator. */
            uint32_t Den() const;

            /** @brief Replaces the numerator; zero makes the period invalid. */
            void SetNum(uint32_t num);

            /** @brief Replaces the denominator; zero makes the ratio invalid. */
            void SetDen(uint32_t den);

            /** @brief Replaces both ratio terms. */
            void Set(
                uint32_t num,
                uint32_t den
            );

            /** @brief Reports whether both ratio terms are non-zero. */
            bool IsValid() const;

            /** @brief Returns `num / den` seconds, or zero when invalid. */
            float Seconds() const;

            /** @brief Returns the period in milliseconds. */
            float Milliseconds() const;

            /** @brief Returns the period in microseconds. */
            float Microseconds() const;

            /**
             * @brief Returns a const reference to the backing mathematical ratio.
             * @note Use Period::IsValid() for the domain contract. A mathematical
             * `0 / 1` ratio is valid even though zero is not a valid Period.
             */
            const Foundation::Math::UnsignedRatio&
            GetRatio() const;

            /** @brief Returns the reciprocal Frequency, or an invalid Frequency. */
            Frequency GetFrequency() const;
        };

    }
}

#endif
