#ifndef FOUNDATION_SCHEDULING_ONE_SHOT_TASK_H
#define FOUNDATION_SCHEDULING_ONE_SHOT_TASK_H

#include "Task.h"

namespace Foundation::Scheduling {

    /**
     * @brief Task that becomes due once at or after a trigger TimePoint.
     * @ingroup Foundation_Scheduling
     * @tparam TickType Tick specialization shared with its scheduler.
     */
    template <typename TickType = Foundation::Time::Tick32>
    class BasicOneShotTask : public BasicTask<TickType> {
    public:
        using TimePointType = Foundation::Time::BasicTimePoint<TickType>;

        /** @brief Callback receiving the stored context pointer. */
        typedef void (*Callback)(void* context);

    private:
        Callback _callback;
        void* _context;
        TimePointType _triggerTime;
        bool _hasRun;

    public:
        /**
         * @brief Creates a one-shot task.
         * @param callback Function invoked once when due; may be null.
         * @param context Non-owning pointer forwarded unchanged to `callback`.
         * @param triggerTime Earliest TimePoint at which the task is due.
         * @note A null context is allowed and is forwarded to a non-null
         * callback. A null callback still allows Run() to complete the task.
         * @warning The trigger and scheduler TimePoints must use the same Clock.
         * A non-null context must outlive the task and its invocation.
         */
        BasicOneShotTask(
            Callback callback,
            void* context,
            TimePointType triggerTime
        )
            : _callback(callback),
              _context(context),
              _triggerTime(triggerTime),
              _hasRun(false) { }

        /** @brief Reports whether this task is ready at `now`. */
        bool ShouldRun(TimePointType now) override {
            return !_hasRun && now >= _triggerTime;
        }

        /**
         * @brief Marks the task completed and invokes its callback.
         *
         * The supplied scheduler time is intentionally ignored because the
         * trigger comparison already occurred in ShouldRun().
         * @note Marks the task as completed even when the callback is null.
         */
        void Run(TimePointType) override {
            _hasRun = true;
            if (_callback) { _callback(_context); }
        }

        /** @brief Reports whether Run() has already been called. */
        bool HasRun() const { return _hasRun; }
    };

    /** @brief Default 32-bit one-shot task specialization. */
    using OneShotTask = BasicOneShotTask<>;
}

#endif
