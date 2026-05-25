#ifndef FOUNDATION_SCHEDULING_PERIODIC_TASK_H
#define FOUNDATION_SCHEDULING_PERIODIC_TASK_H

#include "Task.h"

namespace Foundation::Scheduling {

    class PeriodicTask : public Task {
    public:
        typedef void (*Callback)(void* context);

    private:
        Callback _callback;
        void* _context;
        Foundation::Time::Clock::TimePoint _period;
        Foundation::Time::Clock::TimePoint _lastRun;
        bool _firstRun;

    public:
        PeriodicTask(Callback callback, void* context, Foundation::Time::Clock::TimePoint period)
            : _callback(callback), _context(context), _period(period), _lastRun(0), _firstRun(true) { }

        bool ShouldRun(Foundation::Time::Clock::TimePoint now) override {
            return _firstRun || ((now - _lastRun) >= _period);
        }

        void Run(Foundation::Time::Clock::TimePoint now) override {
            _firstRun = false;
            _lastRun = now;
            if (_callback) { _callback(_context); }
        }
    };
}

#endif
