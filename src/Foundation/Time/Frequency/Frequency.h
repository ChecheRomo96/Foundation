// Frequency.h

#ifndef FOUNDATION_TIME_FREQUENCY_H
#define FOUNDATION_TIME_FREQUENCY_H

    #include <Foundation/Math/Ratio.h>

    namespace Foundation {
        namespace Time {

            class Period;

            /**
             * @brief Rational number of ticks per second.
             * @ingroup Foundation_Time_Frequency
             *
             * Frequency is backed by Math::UnsignedRatio and may be converted
             * to its reciprocal Period. Both terms must be non-zero for a
             * Frequency to be valid, ensuring every valid value has a finite,
             * representable reciprocal.
             */
            class Frequency {
            private:
                Foundation::Math::UnsignedRatio _ratio;

            public:

                /**
                 * @brief Creates `num / den` ticks per second.
                 * @param num Frequency numerator; zero creates an invalid value.
                 * @param den Frequency denominator; zero creates an invalid ratio.
                 */
                Frequency(
                    uint32_t num = 0,
                    uint32_t den = 1
                );

                /** @brief Creates a frequency from an existing unsigned ratio. */
                constexpr Frequency(
                    const Foundation::Math::UnsignedRatio& ratio
                );

                /** @brief Returns the frequency numerator. */
                constexpr uint32_t Num() const;

                /** @brief Returns the frequency denominator. */
                constexpr uint32_t Den() const;

                /** @brief Replaces the numerator; zero makes the frequency invalid. */
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

                /** @brief Returns `num / den` ticks per second, or zero when invalid. */
                float Hertz() const;

                /**
                 * @brief Returns reciprocal seconds per tick, or zero when invalid.
                 */
                float PeriodSeconds() const;

                /** @brief Returns reciprocal milliseconds per tick, or zero when invalid. */
                float PeriodMilliseconds() const;

                /** @brief Returns reciprocal microseconds per tick, or zero when invalid. */
                float PeriodMicroseconds() const;

                /**
                 * @brief Returns a const reference to the backing mathematical ratio.
                 * @note Use Frequency::IsValid() for the domain contract. A
                 * mathematical `0 / 1` ratio is valid even though zero is not
                 * a valid Frequency.
                 */
                const Foundation::Math::UnsignedRatio&
                GetRatio() const;

                /** @brief Returns the reciprocal Period, or an invalid Period. */
                Period GetPeriod() const;
            };

            constexpr Frequency::Frequency(
                const Foundation::Math::UnsignedRatio& ratio
            )
                : _ratio(ratio) {}

            constexpr uint32_t
            Frequency::Num() const {
                return _ratio.Num();
            }

            constexpr uint32_t
            Frequency::Den() const {
                return _ratio.Den();
            }

        }
    }

#endif
