// PeriodExample.cpp

#include <iostream>

#include <Foundation/Math/Ratio.h>
#include <Foundation/Time/Frequency.h>
#include <Foundation/Time/Period.h>

void PrintRatio(const char* name, const Foundation::Math::Ratio& r) {
    std::cout << name << "\n";
    std::cout << "  Num:      " << r.Num() << "\n";
    std::cout << "  Den:      " << r.Den() << "\n";
    std::cout << "  IsValid:  " << r.IsValid() << "\n";
    std::cout << "  ToFloat:  " << r.ToFloat() << "\n\n";
}

void PrintPeriod(const char* name, const Foundation::Time::Period& p) {
    std::cout << name << "\n";
    std::cout << "  Num:           " << p.Num() << "\n";
    std::cout << "  Den:           " << p.Den() << "\n";
    std::cout << "  IsValid:       " << p.IsValid() << "\n";
    std::cout << "  Seconds:       " << p.Seconds() << " s\n";
    std::cout << "  Milliseconds:  " << p.Milliseconds() << " ms\n";
    std::cout << "  Microseconds:  " << p.Microseconds() << " us\n\n";
}

int main() {
    using Foundation::Math::Ratio;
    using Foundation::Time::Frequency;
    using Foundation::Time::Period;

    Period Ts(1, 48000);
    PrintPeriod("Period Ts(1, 48000)", Ts);

    Frequency Fs = Ts.GetFrequency();

    Period videoPeriod(1001, 30000);
    PrintPeriod("Period videoPeriod(1001, 30000)", videoPeriod);

    Period mutablePeriod;

    mutablePeriod.Set(1, 1000);
    PrintPeriod(
        "Period mutablePeriod after Set(1, 1000)",
        mutablePeriod
    );

    mutablePeriod.SetNum(1);
    mutablePeriod.SetDen(48000);
    PrintPeriod(
        "Period mutablePeriod after SetNum(1), SetDen(48000)",
        mutablePeriod
    );

    Ratio rawPeriodRatio(1, 96000);

    Period periodFromRatio(rawPeriodRatio);
    PrintPeriod(
        "Period periodFromRatio(Ratio(1, 96000))",
        periodFromRatio
    );

    PrintRatio(
        "periodFromRatio.GetRatio()",
        periodFromRatio.GetRatio()
    );

    Period invalidPeriod(0, 1);
    PrintPeriod("Period invalidPeriod(0, 1)", invalidPeriod);

    return 0;
}