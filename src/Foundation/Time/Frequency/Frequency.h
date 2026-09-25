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
                constexpr Frequency(
                    uint32_t num = 0,
                    uint32_t den = 1
                ) noexcept;

                /** @brief Creates a frequency from an existing unsigned ratio. */
                constexpr Frequency(
                    const Foundation::Math::UnsignedRatio& ratio
                ) noexcept;

                /** @brief Returns the frequency numerator. */
                constexpr uint32_t Numerator() const noexcept;

                /** @brief Returns the frequency denominator. */
                constexpr uint32_t Denominator() const noexcept;

                /** @brief Replaces the numerator; zero makes the frequency invalid. */
                constexpr void SetNumerator(uint32_t num) noexcept;

                /** @brief Replaces the denominator; zero makes the ratio invalid. */
                constexpr void SetDenominator(uint32_t den) noexcept;

                /** @brief Replaces both ratio terms. */
                constexpr void Set(
                    uint32_t num,
                    uint32_t den
                ) noexcept;

                /** @brief Reports whether both ratio terms are non-zero. */
                constexpr bool IsValid() const noexcept;

                /** @brief Returns `num / den` ticks per second, or zero when invalid. */
                constexpr float Hertz() const noexcept;

                /**
                 * @brief Returns reciprocal seconds per tick, or zero when invalid.
                 */
                constexpr float PeriodSeconds() const noexcept;

                /** @brief Returns reciprocal milliseconds per tick, or zero when invalid. */
                constexpr float PeriodMilliseconds() const noexcept;

                /** @brief Returns reciprocal microseconds per tick, or zero when invalid. */
                constexpr float PeriodMicroseconds() const noexcept;

                /**
                 * @brief Returns a const reference to the backing mathematical ratio.
                 * @note Use Frequency::IsValid() for the domain contract. A
                 * mathematical `0 / 1` ratio is valid even though zero is not
                 * a valid Frequency.
                 */
                constexpr const Foundation::Math::UnsignedRatio&
                GetRatio() const noexcept;

                /** @brief Returns the reciprocal Period, or an invalid Period. */
                Period GetPeriod() const noexcept;
            };

            constexpr Frequency::Frequency(
                uint32_t num,
                uint32_t den
            ) noexcept
                : _ratio(num, den) {}

            constexpr Frequency::Frequency(
                const Foundation::Math::UnsignedRatio& ratio
            ) noexcept
                : _ratio(ratio) {}

            constexpr uint32_t
            Frequency::Numerator() const noexcept {
                return _ratio.Numerator();
            }

            constexpr uint32_t
            Frequency::Denominator() const noexcept {
                return _ratio.Denominator();
            }

            constexpr void Frequency::SetNumerator(uint32_t num) noexcept {
                _ratio.SetNumerator(num);
            }

            constexpr void Frequency::SetDenominator(uint32_t den) noexcept {
                _ratio.SetDenominator(den);
            }

            constexpr void Frequency::Set(
                uint32_t num,
                uint32_t den
            ) noexcept {
                _ratio.Set(num, den);
            }

            constexpr bool Frequency::IsValid() const noexcept {
                return _ratio.IsValid() && Numerator() != 0;
            }

            constexpr float Frequency::Hertz() const noexcept {
                return IsValid() ? _ratio.ToFloat() : 0.0f;
            }

            constexpr float Frequency::PeriodSeconds() const noexcept {
                return IsValid()
                    ? static_cast<float>(Denominator()) /
                      static_cast<float>(Numerator())
                    : 0.0f;
            }

            constexpr float Frequency::PeriodMilliseconds() const noexcept {
                return PeriodSeconds() * 1000.0f;
            }

            constexpr float Frequency::PeriodMicroseconds() const noexcept {
                return PeriodSeconds() * 1000000.0f;
            }

            constexpr const Foundation::Math::UnsignedRatio&
            Frequency::GetRatio() const noexcept {
                return _ratio;
            }

        }
    }

#endif
