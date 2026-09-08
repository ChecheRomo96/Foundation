#ifndef FOUNDATION_SCHEDULING_TASK_H
#define FOUNDATION_SCHEDULING_TASK_H

#include "../Time/Clock.h"

namespace Foundation::Scheduling {

    class Task {
    public:
        virtual ~Task() { }
        virtual bool ShouldRun(Foundation::Time::TimePoint now) = 0;
        virtual void Run(Foundation::Time::TimePoint now) = 0;
    };
}

#endif
