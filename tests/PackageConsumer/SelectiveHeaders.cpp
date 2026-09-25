#include <Foundation/Time/Frequency.h>
#include <Foundation/Time/Period.h>

bool ValidateSelectiveHeaders() {
    const Foundation::Time::Frequency frequency(48000, 1);
    const Foundation::Time::Period period = frequency.GetPeriod();
    const Foundation::Time::Frequency roundTrip = period.GetFrequency();

    return
        period.IsValid() &&
        period.Numerator() == 1 &&
        period.Denominator() == 48000 &&
        roundTrip.Numerator() == 48000 &&
        roundTrip.Denominator() == 1;
}
