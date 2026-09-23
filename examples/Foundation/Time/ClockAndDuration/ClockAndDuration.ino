#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Time::ClockAndDuration::Result result =
        FoundationExamples::Time::ClockAndDuration::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Time / ClockAndDuration");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Turn raw ticks into related TimePoints and an elapsed");
    Serial.println("  Duration using a 1 kHz clock.");
    Serial.println();
    Serial.println("[1] CLOCK CONFIGURATION");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Time::Clock clock(");
    Serial.println("        ReadTick, Foundation::Time::Frequency(1000, 1));");
    Serial.println("  Frequency ............ 1000 Hz");
    Serial.println("  Period ............... 1 millisecond per tick");
    Serial.println();
    Serial.println("[2] SAMPLE TWO TIME POINTS");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    const auto start = clock.Now();");
    Serial.println("    const auto end = clock.Now();");
    Serial.print("  Start ................ ");
    Serial.println(result.StartingTick);
    Serial.print("  End .................. ");
    Serial.println(result.EndingTick);
    Serial.print("  Same Clock identity .. ");
    Serial.println(result.SameClock ? "yes" : "no");
    Serial.println();
    Serial.println("[3] SUBTRACT THE TIME POINTS");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    const auto elapsed = end - start;");
    Serial.print("  Elapsed ticks ........ ");
    Serial.print(result.ElapsedTicks);
    Serial.print(" ticks = ");
    Serial.print(result.ElapsedMilliseconds, 3);
    Serial.println(" ms");
    Serial.println();
    Serial.println("[4] ADD A 50-TICK DURATION");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    const auto future =");
    Serial.println("        end + Foundation::Time::Duration(50);");
    Serial.print("  Future point ......... ");
    Serial.println(result.FutureTick);
    Serial.println();
    Serial.println("[5] UNBIND THE TICK CALLBACK");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    clock.Unbind();");
    Serial.print("  Clock remains bound .. ");
    Serial.println(result.BoundAfterUnbind ? "yes" : "no");
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  TimePoint carries Clock identity; Duration carries only a");
    Serial.println("  tick count and needs a Period for conversion.");
    Serial.println("============================================================");
}

void loop() {
}
