#ifndef FOUNDATION_TIME_PERIOD_H
#define FOUNDATION_TIME_PERIOD_H

#include <Foundation/Math/Ratio.h>

namespace Foundation {
    namespace Time {

        class Frequency;

        class Period {
        private:
            Foundation::Math::Ratio _ratio;

        public:

            Period(
                uint32_t num = 0,
                uint32_t den = 1
            )
                : _ratio(num, den) {}

            constexpr Period(
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

            float Seconds() const {
                return _ratio.ToFloat();
            }

            float Milliseconds() const {
                return Seconds() * 1000.0f;
            }

            float Microseconds() const {
                return Seconds() * 1000000.0f;
            }

            const Foundation::Math::Ratio&
            GetRatio() const {
                return _ratio;
            }

            Frequency GetFrequency() const;
        };
    }
}

#endif