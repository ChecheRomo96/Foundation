#ifndef FOUNDATION_SCHEDULING_PERIODIC_TASK_H
#define FOUNDATION_SCHEDULING_PERIODIC_TASK_H

#include "Task.h"

namespace Foundation::Scheduling {

    /**
     * @brief Task that runs immediately and then at a fixed tick interval.
     * @ingroup Foundation_Scheduling
     *
     * The first ShouldRun() returns true. After Run(), the next due time is
     * measured from the TimePoint passed to that invocation.
     */
    class PeriodicTask : public Task {
    public:
        /** @brief Callback receiving the stored context pointer. */
        typedef void (*Callback)(void* context);

    private:
        Callback _callback;
        void* _context;
        Foundation::Time::Duration _period;
        Foundation::Time::TimePoint _lastRun;
        bool _firstRun;

    public:
        /**
         * @brief Creates a periodic task.
         * @param callback Function invoked when the task runs; may be null.
         * @param context Non-owning pointer forwarded unchanged to `callback`.
         * @param period Minimum number of ticks between runs after the first.
         * @warning A non-null context must outlive the task and all invocations.
         */
        PeriodicTask(Callback callback, void* context, Foundation::Time::Duration period)
            : _callback(callback), _context(context), _period(period), _lastRun(0), _firstRun(true) { }

        /** @copydoc Task::ShouldRun */
        bool ShouldRun(Foundation::Time::TimePoint now) override {
            return _firstRun || ((now - _lastRun) >= _period);
        }

        /**
         * @copydoc Task::Run
         * @note Records `now` even when the callback is null.
         */
        void Run(Foundation::Time::TimePoint now) override {
            _firstRun = false;
            _lastRun = now;
            if (_callback) { _callback(_context); }
        }
    };
}

#endif
