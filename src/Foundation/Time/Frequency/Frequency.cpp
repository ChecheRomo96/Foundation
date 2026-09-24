// Frequency.cpp

#include <Foundation/Time/Frequency.h>
#include <Foundation/Time/Period.h>

namespace Foundation {
    namespace Time {
        
        Period Frequency::GetPeriod() const noexcept {
            return IsValid()
                ? Period(_ratio.Den(), _ratio.Num())
                : Period();
        }

    }
}
