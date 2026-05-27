// Period.cpp

#include <Foundation/Time/Period.h>
#include <Foundation/Time/Frequency.h>

namespace Foundation {
    namespace Time {

        Period::Period(
            uint32_t num,
            uint32_t den
        )
            : _ratio(num, den) {}

        Period::Period(
            const Foundation::Math::Ratio& ratio
        )
            : _ratio(ratio) {}

        uint32_t
        Period::Num() const {
            return _ratio.Num();
        }

        uint32_t
        Period::Den() const {
            return _ratio.Den();
        }

        void Period::SetNum(uint32_t num) {
            _ratio.SetNum(num);
        }

        void Period::SetDen(uint32_t den) {
            _ratio.SetDen(den);
        }

        void Period::Set(
            uint32_t num,
            uint32_t den
        ) {
            _ratio.Set(num, den);
        }

        bool Period::IsValid() const {
            return _ratio.IsValid();
        }

        float Period::Seconds() const {
            return _ratio.ToFloat();
        }

        float Period::Milliseconds() const {
            return Seconds() * 1000.0f;
        }

        float Period::Microseconds() const {
            return Seconds() * 1000000.0f;
        }

        const Foundation::Math::Ratio&
        Period::GetRatio() const {
            return _ratio;
        }

        Frequency Period::GetFrequency() const {
            return Frequency(
                _ratio.Den(),
                _ratio.Num()
            );
        }

    }
}