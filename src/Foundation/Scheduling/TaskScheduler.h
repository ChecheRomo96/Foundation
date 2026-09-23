#ifndef FOUNDATION_SCHEDULING_TASK_SCHEDULER_H
#define FOUNDATION_SCHEDULING_TASK_SCHEDULER_H

#include <stddef.h>
#include "Task.h"
#include "../Time/Clock.h"

namespace Foundation::Scheduling {

    /**
     * @brief Fixed-capacity cooperative scheduler driven by a Clock.
     * @ingroup Foundation_Scheduling
     *
     * Stores non-owning Task pointers in caller-provided storage. Update()
     * executes due tasks synchronously in registration order.
     */
    class TaskScheduler {
    private:
        Task** _tasks;
        size_t _capacity;
        size_t _count;
        Foundation::Time::Clock* _clock;

    public:
        /**
         * @brief Creates an empty scheduler.
         * @param tasks Array capable of storing `capacity` Task pointers.
         * @param capacity Maximum number of registered tasks.
         * @param clock Non-owning clock used by Update(); may be null.
         * @warning For non-zero capacity, `tasks` must be non-null. The storage,
         * clock, and registered tasks must outlive their use by the scheduler.
         */
        TaskScheduler(Task** tasks, size_t capacity, Foundation::Time::Clock* clock)
            : _tasks(tasks), _capacity(capacity), _count(0), _clock(clock) { }

        /**
         * @brief Appends a task in execution order.
         * @param task Non-owning task pointer.
         * @return `true` when registered; `false` for null or full capacity.
         */
        bool AddTask(Task* task) {
            if (_count >= _capacity || task == 0) { return false; }
            _tasks[_count++] = task;
            return true;
        }

        /**
         * @brief Samples the clock and synchronously runs every due task.
         *
         * Does nothing when the Clock pointer is null. One TimePoint is shared
         * by all task decisions and executions in this update.
         */
        void Update() {
            if (_clock == 0) { return; }
            Foundation::Time::TimePoint now = _clock->Now();
            for (size_t i = 0; i < _count; ++i) {
                if (_tasks[i] && _tasks[i]->ShouldRun(now)) {
                    _tasks[i]->Run(now);
                }
            }
        }

        /** @brief Returns the number of registered task pointers. */
        size_t GetTaskCount() const { return _count; }
        /** @brief Returns the fixed registration capacity. */
        size_t GetCapacity() const { return _capacity; }
        /** @brief Forgets all tasks without destroying them. */
        void Clear() { _count = 0; }
    };
}

#endif
