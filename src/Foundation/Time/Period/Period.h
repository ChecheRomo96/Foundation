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
            constexpr Period(
                uint32_t num = 0,
                uint32_t den = 1
            ) noexcept;

            /** @brief Creates a period from an existing unsigned ratio. */
            constexpr Period(
                const Foundation::Math::UnsignedRatio& ratio
            ) noexcept;

            /** @brief Returns the period numerator. */
            constexpr uint32_t Numerator() const noexcept;

            /** @brief Returns the period denominator. */
            constexpr uint32_t Denominator() const noexcept;

            /** @brief Replaces the numerator; zero makes the period invalid. */
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

            /** @brief Returns `num / den` seconds, or zero when invalid. */
            constexpr float Seconds() const noexcept;

            /** @brief Returns the period in milliseconds. */
            constexpr float Milliseconds() const noexcept;

            /** @brief Returns the period in microseconds. */
            constexpr float Microseconds() const noexcept;

            /**
             * @brief Returns a const reference to the backing mathematical ratio.
             * @note Use Period::IsValid() for the domain contract. A mathematical
             * `0 / 1` ratio is valid even though zero is not a valid Period.
             */
            constexpr const Foundation::Math::UnsignedRatio&
            GetRatio() const noexcept;

            /** @brief Returns the reciprocal Frequency, or an invalid Frequency. */
            Frequency GetFrequency() const noexcept;
        };

        constexpr Period::Period(
            uint32_t num,
            uint32_t den
        ) noexcept
            : _ratio(num, den) {}

        constexpr Period::Period(
            const Foundation::Math::UnsignedRatio& ratio
        ) noexcept
            : _ratio(ratio) {}

        constexpr uint32_t Period::Numerator() const noexcept {
            return _ratio.Numerator();
        }

        constexpr uint32_t Period::Denominator() const noexcept {
            return _ratio.Denominator();
        }

        constexpr void Period::SetNumerator(uint32_t num) noexcept {
            _ratio.SetNumerator(num);
        }

        constexpr void Period::SetDenominator(uint32_t den) noexcept {
            _ratio.SetDenominator(den);
        }

        constexpr void Period::Set(
            uint32_t num,
            uint32_t den
        ) noexcept {
            _ratio.Set(num, den);
        }

        constexpr bool Period::IsValid() const noexcept {
            return _ratio.IsValid() && Numerator() != 0;
        }

        constexpr float Period::Seconds() const noexcept {
            return IsValid() ? _ratio.ToFloat() : 0.0f;
        }

        constexpr float Period::Milliseconds() const noexcept {
            return Seconds() * 1000.0f;
        }

        constexpr float Period::Microseconds() const noexcept {
            return Seconds() * 1000000.0f;
        }

        constexpr const Foundation::Math::UnsignedRatio&
        Period::GetRatio() const noexcept {
            return _ratio;
        }

    }
}

#endif
