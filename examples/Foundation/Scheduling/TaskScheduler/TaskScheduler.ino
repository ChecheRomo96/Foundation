#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Scheduling::TaskScheduler::Result result =
        FoundationExamples::Scheduling::TaskScheduler::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Scheduling / TaskScheduler");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Drive periodic and one-shot work from a simulated 1 kHz");
    Serial.println("  clock without threads or heap allocation.");
    Serial.println();
    Serial.println("[1] SCHEDULER CONFIGURATION");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Scheduling::TaskScheduler scheduler(");
    Serial.println("        tasks, 2, &clock);");
    Serial.println("    scheduler.AddTask(&periodicTask);");
    Serial.println("    scheduler.AddTask(&oneShotTask);");
    Serial.println("  Simulated ticks ...... 0 through 6");
    Serial.println("  Periodic interval .... 2 ticks");
    Serial.println("  One-shot trigger ..... tick 5");
    Serial.print("  Registered/capacity .. ");
    Serial.print(result.RegisteredTasks);
    Serial.print('/');
    Serial.println(result.SchedulerCapacity);
    Serial.println();
    Serial.println("[2] OBSERVED EXECUTION");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    scheduler.Update();  // once per simulated tick");
    Serial.print("  Periodic runs ........ ");
    Serial.println(result.PeriodicRuns);
    Serial.print("  One-shot runs ........ ");
    Serial.println(result.OneShotRuns);
    Serial.print("  One-shot completed ... ");
    Serial.println(result.OneShotHasRun ? "yes" : "no");
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Update() samples the clock and runs due tasks");
    Serial.println("  synchronously in registration order.");
    Serial.println("============================================================");
}

void loop() {
}
