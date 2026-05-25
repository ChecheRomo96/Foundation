#ifndef FOUNDATION_TIME_FREQUENCY_H
#define FOUNDATION_TIME_FREQUENCY_H

#include <Foundation/Math/Ratio.h>

namespace Foundation {
    namespace Time {

        class Period;

        class Frequency {
        private:
            Foundation::Math::Ratio _ratio;

        public:

            constexpr Frequency(
                uint32_t num = 0,
                uint32_t den = 1
            )
                : _ratio(num, den) {}

            constexpr Frequency(
                const Foundation::Math::Ratio& ratio
            )
                : _ratio(ratio) {}

            constexpr uint32_t Num() const {
                return _ratio.Num();
            }

            constexpr uint32_t Den() const {
                return _ratio.Den();
            }

            void SetNum(uint32_t num) {
                _ratio.SetNum(num);
            }

            void SetDen(uint32_t den) {
                _ratio.SetDen(den);
            }

            void Set(
                uint32_t num,
                uint32_t den
            ) {
                _ratio.Set(num, den);
            }

            bool IsValid() const {
                return _ratio.IsValid();
            }

            float Hertz() const {
                return _ratio.ToFloat();
            }

            float PeriodSeconds() const {
                return IsValid()
                    ? static_cast<float>(Den()) /
                    static_cast<float>(Num())
                    : 0.0f;
            }

            float PeriodMilliseconds() const {
                return PeriodSeconds() * 1000.0f;
            }

            float PeriodMicroseconds() const {
                return PeriodSeconds() * 1000000.0f;
            }

            const Foundation::Math::Ratio&
            GetRatio() const {
                return _ratio;
            }

            Period GetPeriod() const;
        };
    }
}

#endif