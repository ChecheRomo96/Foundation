#include "Shared.h"

namespace FoundationExamples {
namespace Scheduling {
namespace TaskScheduler {

    namespace {

        Foundation::Time::Tick32::Representation CurrentTick = 0;

        Foundation::Time::Tick32::Representation ReadTick() {
            return CurrentTick;
        }

        void Increment(void* context) {
            ++(*static_cast<unsigned int*>(context));
        }

    } // namespace

    Result Run() {
        Foundation::Time::Clock clock(
            ReadTick,
            Foundation::Time::Frequency(1000, 1)
        );

        unsigned int periodicRuns = 0;
        unsigned int oneShotRuns = 0;

        Foundation::Scheduling::PeriodicTask periodicTask(
            Increment,
            &periodicRuns,
            Foundation::Time::Duration(2)
        );
        Foundation::Scheduling::OneShotTask oneShotTask(
            Increment,
            &oneShotRuns,
            clock.Now() + Foundation::Time::Duration(5)
        );

        Foundation::Scheduling::Task* tasks[2] = {};
        Foundation::Scheduling::TaskScheduler scheduler(tasks, 2, &clock);
        scheduler.AddTask(&periodicTask);
        scheduler.AddTask(&oneShotTask);

        for (CurrentTick = 0; CurrentTick <= 6; ++CurrentTick) {
            scheduler.Update();
        }

        Result result;
        result.PeriodicRuns = periodicRuns;
        result.OneShotRuns = oneShotRuns;
        result.OneShotHasRun = oneShotTask.HasRun();
        result.RegisteredTasks = static_cast<unsigned int>(
            scheduler.GetTaskCount()
        );
        result.SchedulerCapacity = static_cast<unsigned int>(
            scheduler.GetCapacity()
        );
        return result;
    }

} // namespace TaskScheduler
} // namespace Scheduling
} // namespace FoundationExamples
