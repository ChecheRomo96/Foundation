#ifndef FOUNDATION_SCHEDULING_PERIODIC_TASK_H
#define FOUNDATION_SCHEDULING_PERIODIC_TASK_H

#include "Task.h"

namespace Foundation::Scheduling {

    /**
     * @brief Task that runs immediately and then at a fixed tick interval.
     * @ingroup Foundation_Scheduling
     * @tparam TickType Tick specialization shared with its scheduler.
     *
     * The first ShouldRun() returns true. After Run(), the next due time is
     * measured from the TimePoint passed to that invocation.
     */
    template <typename TickType = Foundation::Time::Tick32>
    class BasicPeriodicTask : public BasicTask<TickType> {
    public:
        using DurationType = Foundation::Time::BasicDuration<TickType>;
        using TimePointType = Foundation::Time::BasicTimePoint<TickType>;

        /** @brief Callback receiving the stored context pointer. */
        typedef void (*Callback)(void* context);

    private:
        Callback _callback;
        void* _context;
        DurationType _period;
        TimePointType _lastRun;
        bool _firstRun;

    public:
        /**
         * @brief Creates a periodic task.
         * @param callback Function invoked when the task runs; may be null.
         * @param context Non-owning pointer forwarded unchanged to `callback`.
         * @param period Minimum number of ticks between runs after the first.
         * @note A null context is allowed and is forwarded to a non-null
         * callback. A null callback makes Run() update timing without invoking
         * user code.
         * @warning A non-null context must outlive the task and all invocations.
         */
        BasicPeriodicTask(
            Callback callback,
            void* context,
            DurationType period
        )
            : _callback(callback),
              _context(context),
              _period(period),
              _lastRun(),
              _firstRun(true) { }

        /** @brief Reports whether this task is ready at `now`. */
        bool ShouldRun(TimePointType now) override {
            return _firstRun || ((now - _lastRun) >= _period);
        }

        /**
         * @brief Records `now` and invokes the callback when one is bound.
         * @note Records `now` even when the callback is null.
         */
        void Run(TimePointType now) override {
            _firstRun = false;
            _lastRun = now;
            if (_callback) { _callback(_context); }
        }
    };

    /** @brief Default 32-bit periodic task specialization. */
    using PeriodicTask = BasicPeriodicTask<>;
}

#endif
