#ifndef FOUNDATION_SCHEDULING_TASK_H
#define FOUNDATION_SCHEDULING_TASK_H

#include "../Time/Clock.h"

namespace Foundation::Scheduling {

    /**
     * @brief Interface implemented by cooperatively scheduled tasks.
     * @ingroup Foundation_Scheduling
     * @tparam TickType Tick specialization shared by the scheduling family.
     *
     * BasicTaskScheduler calls ShouldRun() and Run() synchronously. Derived
     * objects are not owned by the scheduler.
     */
    template <typename TickType = Foundation::Time::Tick32>
    class BasicTask {
    public:
        using TimePointType = Foundation::Time::BasicTimePoint<TickType>;

        /** @brief Enables safe destruction through a Task pointer. */
        virtual ~BasicTask() { }

        /**
         * @brief Decides whether the task is due at `now`.
         * @param now Time sampled once by the scheduler for the current update.
         * @return `true` when Run() should be called.
         */
        virtual bool ShouldRun(TimePointType now) = 0;

        /**
         * @brief Executes the task at `now`.
         * @param now Same sampled TimePoint supplied to ShouldRun().
         */
        virtual void Run(TimePointType now) = 0;
    };

    /** @brief Default 32-bit scheduling task interface. */
    using Task = BasicTask<>;
}

#endif
