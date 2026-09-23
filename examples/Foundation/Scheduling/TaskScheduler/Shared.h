#ifndef FOUNDATION_EXAMPLES_SCHEDULING_TASK_SCHEDULER_SHARED_H
#define FOUNDATION_EXAMPLES_SCHEDULING_TASK_SCHEDULER_SHARED_H

#include <Foundation/Scheduling.h>
#include <Foundation/Time.h>

namespace FoundationExamples {
namespace Scheduling {
namespace TaskScheduler {

    struct Result {
        unsigned int PeriodicRuns;
        unsigned int OneShotRuns;
        bool OneShotHasRun;
        unsigned int RegisteredTasks;
        unsigned int SchedulerCapacity;
    };

    Result Run();

} // namespace TaskScheduler
} // namespace Scheduling
} // namespace FoundationExamples

#endif
