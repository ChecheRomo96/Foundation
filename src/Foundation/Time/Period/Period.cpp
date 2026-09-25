// Period.cpp

#include <Foundation/Time/Period.h>
#include <Foundation/Time/Frequency.h>

namespace Foundation {
    namespace Time {

        Frequency Period::GetFrequency() const noexcept {
            return IsValid()
                ? Frequency(_ratio.Denominator(), _ratio.Numerator())
                : Frequency();
        }

    }
}
