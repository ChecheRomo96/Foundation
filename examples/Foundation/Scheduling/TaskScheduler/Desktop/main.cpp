#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::Scheduling::TaskScheduler::Result result =
        FoundationExamples::Scheduling::TaskScheduler::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Scheduling / TaskScheduler\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Drive periodic and one-shot work from a simulated 1 kHz\n"
        << "  clock without threads or heap allocation.\n\n"
        << "[1] SCHEDULER CONFIGURATION\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Scheduling::TaskScheduler scheduler(\n"
        << "        tasks, 2, &clock);\n"
        << "    scheduler.AddTask(&periodicTask);\n"
        << "    scheduler.AddTask(&oneShotTask);\n"
        << "  Simulated ticks ...... 0 through 6\n"
        << "  Periodic interval .... 2 ticks\n"
        << "  One-shot trigger ..... tick 5\n"
        << "  Registered/capacity .. " << result.RegisteredTasks << '/'
        << result.SchedulerCapacity << "\n\n"
        << "[2] OBSERVED EXECUTION\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    scheduler.Update();  // called once per simulated tick\n"
        << "  Periodic runs ........ " << result.PeriodicRuns
        << "  (ticks 0, 2, 4, 6)\n"
        << "  One-shot runs ........ " << result.OneShotRuns
        << "  (once at tick 5)\n"
        << "  One-shot completed ... "
        << (result.OneShotHasRun ? "yes" : "no") << "\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Update() samples the clock and runs due tasks\n"
        << "  synchronously in registration order.\n"
        << "============================================================\n";

    return result.PeriodicRuns == 4
        && result.OneShotRuns == 1
        && result.OneShotHasRun
        && result.RegisteredTasks == 2
        && result.SchedulerCapacity == 2
        ? 0
        : 1;
}
