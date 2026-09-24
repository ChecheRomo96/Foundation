// Frequency.cpp

#include <Foundation/Time/Frequency.h>
#include <Foundation/Time/Period.h>

namespace Foundation {
    namespace Time {
        
        Frequency::Frequency(
            uint32_t num,
            uint32_t den
        )
            : _ratio(num, den) {}

        void Frequency::SetNum(uint32_t num) {
            _ratio.SetNum(num);
        }

        void Frequency::SetDen(uint32_t den) {
            _ratio.SetDen(den);
        }

        void Frequency::Set(
            uint32_t num,
            uint32_t den
        ) {
            _ratio.Set(num, den);
        }

        bool Frequency::IsValid() const {
            return _ratio.IsValid() && Num() != 0;
        }

        float Frequency::Hertz() const {
            return IsValid() ? _ratio.ToFloat() : 0.0f;
        }

        float Frequency::PeriodSeconds() const {
            return IsValid()
                ? static_cast<float>(Den()) /
                  static_cast<float>(Num())
                : 0.0f;
        }

        float Frequency::PeriodMilliseconds() const {
            return PeriodSeconds() * 1000.0f;
        }

        float Frequency::PeriodMicroseconds() const {
            return PeriodSeconds() * 1000000.0f;
        }

        const Foundation::Math::UnsignedRatio&
        Frequency::GetRatio() const {
            return _ratio;
        }

        Period Frequency::GetPeriod() const {
            return IsValid()
                ? Period(_ratio.Den(), _ratio.Num())
                : Period();
        }

    }
}
