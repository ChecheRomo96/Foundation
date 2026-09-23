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
         * Period is backed by Math::Ratio and may be converted to its reciprocal
         * Frequency.
         *
         * @warning The public unsigned interface and signed Ratio storage,
         * plus zero-period reciprocal behavior, remain under API-011.
         */
        class Period {
        private:
            Foundation::Math::Ratio _ratio;

        public:

            /**
             * @brief Creates a period of `num / den` seconds per tick.
             * @param num Period numerator.
             * @param den Period denominator; zero creates an invalid ratio.
             */
            Period(
                uint32_t num = 0,
                uint32_t den = 1
            );

            /** @brief Creates a period from an existing ratio. */
            Period(
                const Foundation::Math::Ratio& ratio
            );

            /** @brief Returns the period numerator. */
            uint32_t Num() const;

            /** @brief Returns the period denominator. */
            uint32_t Den() const;

            /** @brief Replaces the numerator. */
            void SetNum(uint32_t num);

            /** @brief Replaces the denominator; zero makes the ratio invalid. */
            void SetDen(uint32_t den);

            /** @brief Replaces both ratio terms. */
            void Set(
                uint32_t num,
                uint32_t den
            );

            /** @brief Reports whether the backing ratio denominator is non-zero. */
            bool IsValid() const;

            /** @brief Returns `num / den` seconds, or zero when invalid. */
            float Seconds() const;

            /** @brief Returns the period in milliseconds. */
            float Milliseconds() const;

            /** @brief Returns the period in microseconds. */
            float Microseconds() const;

            /** @brief Returns a const reference to the backing ratio. */
            const Foundation::Math::Ratio&
            GetRatio() const;

            /** @brief Returns a Frequency with numerator and denominator exchanged. */
            Frequency GetFrequency() const;
        };

    }
}

#endif
