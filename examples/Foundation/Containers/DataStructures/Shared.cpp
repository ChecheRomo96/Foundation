#include "Shared.h"

namespace FoundationExamples {
namespace Containers {
namespace DataStructures {

    Result Run() {
        int circularStorage[3] = {};
        Foundation::Containers::CircularBuffer<int> circularBuffer(
            circularStorage,
            3
        );

        circularBuffer.Push(1);
        circularBuffer.Push(2);
        circularBuffer.Push(3);

        int circularFirst = 0;
        circularBuffer.Pop(circularFirst);
        circularBuffer.Push(4);

        int discarded = 0;
        int circularLast = 0;
        circularBuffer.Pop(discarded);
        circularBuffer.Pop(discarded);
        circularBuffer.Pop(circularLast);

        int queueStorage[2] = {};
        Foundation::Containers::Queue<int> queue(queueStorage, 2);
        queue.Push(10);
        queue.Push(20);
        int queueFront = 0;
        queue.Pop(queueFront);

        int stackStorage[2] = {};
        Foundation::Containers::Stack<int> stack(stackStorage, 2);
        stack.Push(5);
        stack.Push(6);
        int stackTop = 0;
        stack.Pop(stackTop);

        Result result;
        result.CircularBufferFirst = circularFirst;
        result.CircularBufferLast = circularLast;
        result.CircularBufferEmpty = circularBuffer.IsEmpty();
        result.QueueFront = queueFront;
        result.StackTop = stackTop;
        result.StackRemaining = static_cast<unsigned int>(stack.GetCount());
        return result;
    }

} // namespace DataStructures
} // namespace Containers
} // namespace FoundationExamples
