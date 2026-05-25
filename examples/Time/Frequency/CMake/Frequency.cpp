// FrequencyExample.cpp

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

void PrintFrequency(const char* name, const Foundation::Time::Frequency& f) {
    std::cout << name << "\n";
    std::cout << "  Num:                 " << f.Num() << "\n";
    std::cout << "  Den:                 " << f.Den() << "\n";
    std::cout << "  IsValid:             " << f.IsValid() << "\n";
    std::cout << "  Hertz:               " << f.Hertz() << " Hz\n";
    std::cout << "  PeriodSeconds:       " << f.PeriodSeconds() << " s\n";
    std::cout << "  PeriodMilliseconds:  " << f.PeriodMilliseconds() << " ms\n";
    std::cout << "  PeriodMicroseconds:  " << f.PeriodMicroseconds() << " us\n\n";
}

int main() {
    using Foundation::Math::Ratio;
    using Foundation::Time::Frequency;
    using Foundation::Time::Period;

    Ratio audioRatio(48000, 1);
    PrintRatio("Ratio audioRatio(48000, 1)", audioRatio);

    Frequency Fs(48000, 1);
    PrintFrequency("Frequency Fs(48000, 1)", Fs);

    Period Ts = Fs.GetPeriod();

    Frequency FsBack = Ts.GetFrequency();
    PrintFrequency("Frequency FsBack = Ts.GetFrequency()", FsBack);

    Frequency videoRate(30000, 1001);
    PrintFrequency("Frequency videoRate(30000, 1001)", videoRate);

    Frequency mutableFreq;

    mutableFreq.Set(1000, 1);
    PrintFrequency("Frequency mutableFreq after Set(1000, 1)", mutableFreq);

    mutableFreq.SetNum(44100);
    mutableFreq.SetDen(1);
    PrintFrequency(
        "Frequency mutableFreq after SetNum(44100), SetDen(1)",
        mutableFreq
    );

    Ratio rawFrequencyRatio(96000, 1);

    Frequency freqFromRatio(rawFrequencyRatio);
    PrintFrequency(
        "Frequency freqFromRatio(Ratio(96000, 1))",
        freqFromRatio
    );

    PrintRatio(
        "freqFromRatio.GetRatio()",
        freqFromRatio.GetRatio()
    );

    Frequency invalidFreq(0, 1);
    PrintFrequency("Frequency invalidFreq(0, 1)", invalidFreq);

    return 0;
}