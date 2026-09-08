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
        Foundation::Time::Duration _period;
        Foundation::Time::TimePoint _lastRun;
        bool _firstRun;

    public:
        PeriodicTask(Callback callback, void* context, Foundation::Time::Duration period)
            : _callback(callback), _context(context), _period(period), _lastRun(0), _firstRun(true) { }

        bool ShouldRun(Foundation::Time::TimePoint now) override {
            return _firstRun || ((now - _lastRun) >= _period);
        }

        void Run(Foundation::Time::TimePoint now) override {
            _firstRun = false;
            _lastRun = now;
            if (_callback) { _callback(_context); }
        }
    };
}

#endif
