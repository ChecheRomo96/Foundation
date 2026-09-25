#ifndef FOUNDATION_EXAMPLES_CONTAINERS_DATA_STRUCTURES_SHARED_H
#define FOUNDATION_EXAMPLES_CONTAINERS_DATA_STRUCTURES_SHARED_H

#include <Foundation/Containers.h>

namespace FoundationExamples {
namespace Containers {
namespace DataStructures {

    struct Result {
        int CircularBufferFirst;
        int CircularBufferLast;
        bool CircularBufferEmpty;
        int QueueFront;
        int StackTop;
        unsigned int StackRemaining;
    };

    Result Run();

} // namespace DataStructures
} // namespace Containers
} // namespace FoundationExamples

#endif
