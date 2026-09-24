#include <gtest/gtest.h>

#include <Foundation/Scheduling.h>
#include <Foundation/Time.h>

namespace {

Foundation::Time::Tick32::Representation CurrentTick = 0;
Foundation::Time::Tick8::Representation CurrentTick8 = 0;
bool NullContextWasForwarded = false;

Foundation::Time::Tick32::Representation ReadTick() {
    return CurrentTick;
}

Foundation::Time::Tick8::Representation ReadTick8() {
    return CurrentTick8;
}

void Increment(void* context) {
    ++(*static_cast<int*>(context));
}

void ObserveNullContext(void* context) {
    NullContextWasForwarded = context == nullptr;
}

} // namespace

using Foundation::Scheduling::OneShotTask;
using Foundation::Scheduling::PeriodicTask;
using Foundation::Scheduling::Task;
using Foundation::Scheduling::TaskScheduler;
using Foundation::Time::Clock;
using Foundation::Time::Duration;
using Foundation::Time::Frequency;

TEST(SchedulingTypeTest, DefaultAliasesUseTick32) {
    static_assert(
        Foundation::TypeTraits::is_same<
            Task,
            Foundation::Scheduling::BasicTask<Foundation::Time::Tick32>
        >::value,
        "Task must use Tick32 by default"
    );
    static_assert(
        Foundation::TypeTraits::is_same<
            TaskScheduler,
            Foundation::Scheduling::BasicTaskScheduler<
                Foundation::Time::Tick32
            >
        >::value,
        "TaskScheduler must use Tick32 by default"
    );
    static_assert(
        Foundation::TypeTraits::is_same<
            decltype(&Task::ShouldRun),
            bool (Task::*)(Task::TimePointType) const
        >::value,
        "Task::ShouldRun must be a const query"
    );

    SUCCEED();
}

TEST(SchedulingTypeTest, RunsAnEightBitSchedulerAcrossCounterWrap) {
    using TickType = Foundation::Time::Tick8;
    using Clock8 = Foundation::Time::BasicClock<TickType>;
    using Duration8 = Foundation::Time::BasicDuration<TickType>;
    using Task8 = Foundation::Scheduling::BasicTask<TickType>;
    using PeriodicTask8 =
        Foundation::Scheduling::BasicPeriodicTask<TickType>;
    using OneShotTask8 =
        Foundation::Scheduling::BasicOneShotTask<TickType>;
    using TaskScheduler8 =
        Foundation::Scheduling::BasicTaskScheduler<TickType>;

    Clock8 clock(ReadTick8, Frequency(1000, 1));
    int periodicRuns = 0;
    int oneShotRuns = 0;
    PeriodicTask8 periodic(Increment, &periodicRuns, Duration8(4));
    OneShotTask8 oneShot(
        Increment,
        &oneShotRuns,
        clock.At(1)
    );
    Task8* storage[2] = {};
    TaskScheduler8 scheduler(storage, 2, &clock);

    ASSERT_TRUE(scheduler.AddTask(&periodic));
    ASSERT_TRUE(scheduler.AddTask(&oneShot));

    CurrentTick8 = 254;
    scheduler.Update();
    EXPECT_EQ(periodicRuns, 1);
    EXPECT_EQ(oneShotRuns, 0);

    CurrentTick8 = 1;
    scheduler.Update();
    EXPECT_EQ(periodicRuns, 1);
    EXPECT_EQ(oneShotRuns, 1);

    CurrentTick8 = 2;
    scheduler.Update();
    EXPECT_EQ(periodicRuns, 2);
    EXPECT_EQ(oneShotRuns, 1);
}

TEST(OneShotTaskTest, RunsOnceAtOrAfterItsTriggerTime) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    OneShotTask task(Increment, &runs, clock.At(10));
    const OneShotTask& query = task;

    EXPECT_FALSE(query.ShouldRun(clock.At(9)));
    EXPECT_TRUE(query.ShouldRun(clock.At(10)));
    task.Run(clock.At(10));
    EXPECT_EQ(runs, 1);
    EXPECT_TRUE(task.HasRun());
    EXPECT_FALSE(task.ShouldRun(clock.At(11)));
}

TEST(OneShotTaskTest, BecomesDueAcrossCounterWrap) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    OneShotTask task(Increment, &runs, clock.At(1));

    EXPECT_FALSE(task.ShouldRun(clock.At(0xFFFFFFFEu)));
    EXPECT_TRUE(task.ShouldRun(clock.At(1)));
}

TEST(PeriodicTaskTest, RunsImmediatelyAndThenAtItsPeriod) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    PeriodicTask task(Increment, &runs, Duration(5));

    EXPECT_TRUE(task.ShouldRun(clock.At(2)));
    task.Run(clock.At(2));
    EXPECT_EQ(runs, 1);
    EXPECT_FALSE(task.ShouldRun(clock.At(6)));
    EXPECT_TRUE(task.ShouldRun(clock.At(7)));
    task.Run(clock.At(7));
    EXPECT_EQ(runs, 2);
}

TEST(PeriodicTaskTest, MeasuresItsPeriodAcrossCounterWrap) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    PeriodicTask task(Increment, &runs, Duration(5));

    task.Run(clock.At(0xFFFFFFFDu));

    EXPECT_FALSE(task.ShouldRun(clock.At(1)));
    EXPECT_TRUE(task.ShouldRun(clock.At(2)));
}

TEST(PeriodicTaskTest, RejectsElapsedTimeFromAnotherClock) {
    Clock taskClock(ReadTick, Frequency(1000, 1));
    Clock otherClock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    PeriodicTask task(Increment, &runs, Duration(1));

    task.Run(taskClock.At(10));

    EXPECT_FALSE(task.ShouldRun(otherClock.At(20)));
}

TEST(TaskSchedulerTest, ExecutesDueTasksUsingItsClock) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    OneShotTask scheduled(Increment, &runs, clock.At(20));
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

TEST(TaskSchedulerTest, RejectsNullStorageWithNonzeroCapacity) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    OneShotTask task(Increment, &runs, clock.At(0));
    TaskScheduler scheduler(nullptr, 1, &clock);

    EXPECT_FALSE(scheduler.IsValid());
    EXPECT_EQ(scheduler.GetCapacity(), 1u);
    EXPECT_FALSE(scheduler.AddTask(&task));
    EXPECT_EQ(scheduler.GetTaskCount(), 0u);

    scheduler.Update();
    EXPECT_EQ(runs, 0);
}

TEST(TaskSchedulerTest, AcceptsNullStorageForZeroCapacity) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    OneShotTask task(Increment, &runs, clock.At(0));
    TaskScheduler scheduler(nullptr, 0, &clock);

    EXPECT_TRUE(scheduler.IsValid());
    EXPECT_FALSE(scheduler.AddTask(&task));
    scheduler.Update();
    EXPECT_EQ(runs, 0);
}

TEST(TaskSchedulerTest, NullClockDisablesUpdateWithoutInvalidatingStorage) {
    Clock taskClock(ReadTick, Frequency(1000, 1));
    int runs = 0;
    OneShotTask task(Increment, &runs, taskClock.At(0));
    Task* storage[1] = {};
    TaskScheduler scheduler(storage, 1, nullptr);

    EXPECT_TRUE(scheduler.IsValid());
    ASSERT_TRUE(scheduler.AddTask(&task));
    scheduler.Update();
    EXPECT_EQ(runs, 0);
}

TEST(SchedulingContextTest, ForwardsIntentionalNullContexts) {
    Clock clock(ReadTick, Frequency(1000, 1));

    NullContextWasForwarded = false;
    PeriodicTask periodic(ObserveNullContext, nullptr, Duration(1));
    periodic.Run(clock.At(0));
    EXPECT_TRUE(NullContextWasForwarded);

    NullContextWasForwarded = false;
    OneShotTask oneShot(
        ObserveNullContext,
        nullptr,
        clock.At(0)
    );
    oneShot.Run(clock.At(0));
    EXPECT_TRUE(NullContextWasForwarded);
}

TEST(SchedulingContextTest, NullCallbacksRemainSafe) {
    Clock clock(ReadTick, Frequency(1000, 1));
    int context = 42;
    PeriodicTask periodic(nullptr, &context, Duration(1));
    OneShotTask oneShot(nullptr, &context, clock.At(0));

    periodic.Run(clock.At(0));
    oneShot.Run(clock.At(0));

    EXPECT_FALSE(periodic.ShouldRun(clock.At(0)));
    EXPECT_TRUE(oneShot.HasRun());
    EXPECT_EQ(context, 42);
}
