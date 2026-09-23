#ifndef FOUNDATION_SCHEDULING_TASK_H
#define FOUNDATION_SCHEDULING_TASK_H

#include "../Time/Clock.h"

namespace Foundation::Scheduling {

    /**
     * @brief Interface implemented by cooperatively scheduled tasks.
     * @ingroup Foundation_Scheduling
     *
     * TaskScheduler calls ShouldRun() and Run() synchronously. Derived objects
     * are not owned by the scheduler.
     */
    class Task {
    public:
        /** @brief Enables safe destruction through a Task pointer. */
        virtual ~Task() { }

        /**
         * @brief Decides whether the task is due at `now`.
         * @param now Time sampled once by the scheduler for the current update.
         * @return `true` when Run() should be called.
         */
        virtual bool ShouldRun(Foundation::Time::TimePoint now) = 0;

        /**
         * @brief Executes the task at `now`.
         * @param now Same sampled TimePoint supplied to ShouldRun().
         */
        virtual void Run(Foundation::Time::TimePoint now) = 0;
    };
}

#endif
