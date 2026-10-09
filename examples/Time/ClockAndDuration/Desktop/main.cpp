#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::Time::ClockAndDuration::Result result =
        FoundationExamples::Time::ClockAndDuration::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Time / ClockAndDuration\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Turn raw ticks into related TimePoints and an elapsed\n"
        << "  Duration using a 1 kHz clock.\n\n"
        << "[1] CLOCK CONFIGURATION\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Time::Clock clock(\n"
        << "        ReadTick, Foundation::Time::Frequency(1000, 1));\n"
        << "  Frequency ............ 1000 Hz\n"
        << "  Period ............... 1 millisecond per tick\n\n"
        << "[2] SAMPLE TWO TIME POINTS\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    const auto start = clock.Now();\n"
        << "    const auto end = clock.Now();\n"
        << "  Start ................ " << result.StartingTick << " ticks\n"
        << "  End .................. " << result.EndingTick << " ticks\n"
        << "  Same Clock identity .. "
        << (result.SameClock ? "yes" : "no") << "\n\n"
        << "[3] SUBTRACT THE TIME POINTS\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    const auto elapsed = end - start;\n"
        << "  Elapsed .............. " << result.ElapsedTicks << " ticks = "
        << result.ElapsedMilliseconds << " ms\n\n"
        << "[4] ADD A 50-TICK DURATION\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    const auto future =\n"
        << "        end + Foundation::Time::Duration(50);\n"
        << "  Future point ......... " << result.FutureTick << " ticks\n\n"
        << "[5] UNBIND THE TICK CALLBACK\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    clock.Unbind();\n"
        << "  Clock remains bound .. "
        << (result.BoundAfterUnbind ? "yes" : "no") << "\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  TimePoint carries Clock identity; Duration carries only a\n"
        << "  tick count and needs a Period for conversion.\n"
        << "============================================================\n";

    return result.StartingTick == 100
        && result.EndingTick == 350
        && result.ElapsedTicks == 250
        && result.ElapsedMilliseconds == 250.0f
        && result.FutureTick == 400
        && result.SameClock
        && !result.BoundAfterUnbind
        ? 0
        : 1;
}
