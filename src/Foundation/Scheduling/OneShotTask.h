#ifndef FOUNDATION_SCHEDULING_ONE_SHOT_TASK_H
#define FOUNDATION_SCHEDULING_ONE_SHOT_TASK_H

#include "Task.h"

namespace Foundation::Scheduling {

    class OneShotTask : public Task {
    public:
        typedef void (*Callback)(void* context);

    private:
        Callback _callback;
        void* _context;
        Foundation::Time::TimePoint _triggerTime;
        bool _hasRun;

    public:
        OneShotTask(Callback callback, void* context, Foundation::Time::TimePoint triggerTime)
            : _callback(callback), _context(context), _triggerTime(triggerTime), _hasRun(false) { }

        bool ShouldRun(Foundation::Time::TimePoint now) override {
            return !_hasRun && now >= _triggerTime;
        }

        void Run(Foundation::Time::TimePoint) override {
            _hasRun = true;
            if (_callback) { _callback(_context); }
        }

        bool HasRun() const { return _hasRun; }
    };
}

#endif
