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
             * Frequency is backed by Math::Ratio and may be converted to its
             * reciprocal Period.
             *
             * @warning The public unsigned interface and signed Ratio storage,
             * plus zero-frequency reciprocal behavior, remain under API-011.
             */
            class Frequency {
            private:
                Foundation::Math::Ratio _ratio;

            public:

                /**
                 * @brief Creates `num / den` ticks per second.
                 * @param num Frequency numerator.
                 * @param den Frequency denominator; zero creates an invalid ratio.
                 */
                Frequency(
                    uint32_t num = 0,
                    uint32_t den = 1
                );

                /** @brief Creates a frequency from an existing ratio. */
                constexpr Frequency(
                    const Foundation::Math::Ratio& ratio
                );

                /** @brief Returns the frequency numerator. */
                constexpr uint32_t Num() const;

                /** @brief Returns the frequency denominator. */
                constexpr uint32_t Den() const;

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

                /** @brief Returns `num / den` ticks per second, or zero when invalid. */
                float Hertz() const;

                /**
                 * @brief Returns reciprocal seconds per tick.
                 * @warning A zero numerator currently follows floating-point
                 * division behavior; its final v1 contract is under API-011.
                 */
                float PeriodSeconds() const;

                /** @brief Returns reciprocal milliseconds per tick. */
                float PeriodMilliseconds() const;

                /** @brief Returns reciprocal microseconds per tick. */
                float PeriodMicroseconds() const;

                /** @brief Returns a const reference to the backing ratio. */
                const Foundation::Math::Ratio&
                GetRatio() const;

                /** @brief Returns a Period with numerator and denominator exchanged. */
                Period GetPeriod() const;
            };

            constexpr Frequency::Frequency(
                const Foundation::Math::Ratio& ratio
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
