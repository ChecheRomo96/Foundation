#include <gtest/gtest.h>

#include <Foundation/Scheduling/OneShotTask.h>
#include <Foundation/Scheduling/PeriodicTask.h>
#include <Foundation/Scheduling/TaskScheduler.h>
#include <Foundation/Time/Clock.h>
#include <Foundation/Time/Duration.h>
#include <Foundation/Time/Frequency.h>
#include <Foundation/Time/TimePoint.h>

namespace {

Foundation::Time::Tick CurrentTick = 0;

Foundation::Time::Tick ReadTick() {
    return CurrentTick;
}

void Increment(void* context) {
    ++(*static_cast<int*>(context));
}

} // namespace

using Foundation::Scheduling::OneShotTask;
using Foundation::Scheduling::PeriodicTask;
using Foundation::Scheduling::Task;
using Foundation::Scheduling::TaskScheduler;
using Foundation::Time::Clock;
using Foundation::Time::Duration;
using Foundation::Time::Frequency;
using Foundation::Time::TimePoint;

TEST(OneShotTaskTest, RunsOnceAtOrAfterItsTriggerTime) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    OneShotTask task(Increment, &runs, TimePoint(10, &clock));

    EXPECT_FALSE(task.ShouldRun(TimePoint(9, &clock)));
    EXPECT_TRUE(task.ShouldRun(TimePoint(10, &clock)));
    task.Run(TimePoint(10, &clock));
    EXPECT_EQ(runs, 1);
    EXPECT_TRUE(task.HasRun());
    EXPECT_FALSE(task.ShouldRun(TimePoint(11, &clock)));
}

TEST(PeriodicTaskTest, RunsImmediatelyAndThenAtItsPeriod) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    PeriodicTask task(Increment, &runs, Duration(5));

    EXPECT_TRUE(task.ShouldRun(TimePoint(2, &clock)));
    task.Run(TimePoint(2, &clock));
    EXPECT_EQ(runs, 1);
    EXPECT_FALSE(task.ShouldRun(TimePoint(6, &clock)));
    EXPECT_TRUE(task.ShouldRun(TimePoint(7, &clock)));
    task.Run(TimePoint(7, &clock));
    EXPECT_EQ(runs, 2);
}

TEST(TaskSchedulerTest, ExecutesDueTasksUsingItsClock) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    OneShotTask scheduled(Increment, &runs, TimePoint(20, &clock));
    Task* storage[1] = {};
    TaskScheduler scheduler(storage, 1, &clock);

    EXPECT_TRUE(scheduler.AddTask(&scheduled));
    EXPECT_FALSE(scheduler.AddTask(nullptr));
    EXPECT_EQ(scheduler.GetTaskCount(), 1u);

    CurrentTick = 19;
    scheduler.Update();
    EXPECT_EQ(runs, 0);
    CurrentTick = 20;
    scheduler.Update();
    EXPECT_EQ(runs, 1);
    scheduler.Update();
    EXPECT_EQ(runs, 1);

    scheduler.Clear();
    EXPECT_EQ(scheduler.GetTaskCount(), 0u);
}
