#include <iostream>
#include <Foundation/Math/Ratio.h>

using namespace Foundation::Math;

int main() {

    Ratio Half(1, 2);
    Ratio AudioRate(48000, 1);
    Ratio Ntsc(30000, 1001);

    std::cout << "Half:" << std::endl;
    std::cout << "  Num: " << Half.Num() << std::endl;
    std::cout << "  Den: " << Half.Den() << std::endl;
    std::cout << "  Float: " << Half.ToFloat() << std::endl;
    std::cout << std::endl;

    std::cout << "AudioRate:" << std::endl;
    std::cout << "  Num: " << AudioRate.Num() << std::endl;
    std::cout << "  Den: " << AudioRate.Den() << std::endl;
    std::cout << "  Float: " << AudioRate.ToFloat() << std::endl;
    std::cout << std::endl;

    std::cout << "NTSC:" << std::endl;
    std::cout << "  Num: " << Ntsc.Num() << std::endl;
    std::cout << "  Den: " << Ntsc.Den() << std::endl;
    std::cout << "  Float: " << Ntsc.ToFloat() << std::endl;
    std::cout << std::endl;

    return 0;
}