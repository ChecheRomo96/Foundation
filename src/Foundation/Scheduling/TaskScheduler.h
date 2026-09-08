#ifndef FOUNDATION_SCHEDULING_TASK_SCHEDULER_H
#define FOUNDATION_SCHEDULING_TASK_SCHEDULER_H

#include <stddef.h>
#include "Task.h"
#include "../Time/Clock.h"

namespace Foundation::Scheduling {

    class TaskScheduler {
    private:
        Task** _tasks;
        size_t _capacity;
        size_t _count;
        Foundation::Time::Clock* _clock;

    public:
        TaskScheduler(Task** tasks, size_t capacity, Foundation::Time::Clock* clock)
            : _tasks(tasks), _capacity(capacity), _count(0), _clock(clock) { }

        bool AddTask(Task* task) {
            if (_count >= _capacity || task == 0) { return false; }
            _tasks[_count++] = task;
            return true;
        }

        void Update() {
            if (_clock == 0) { return; }
            Foundation::Time::TimePoint now = _clock->Now();
            for (size_t i = 0; i < _count; ++i) {
                if (_tasks[i] && _tasks[i]->ShouldRun(now)) {
                    _tasks[i]->Run(now);
                }
            }
        }

        size_t GetTaskCount() const { return _count; }
        size_t GetCapacity() const { return _capacity; }
        void Clear() { _count = 0; }
    };
}

#endif
