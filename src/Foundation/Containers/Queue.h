#ifndef FOUNDATION_CONTAINERS_QUEUE_H
#define FOUNDATION_CONTAINERS_QUEUE_H

#include "CircularBuffer.h"

namespace Foundation::Containers {

    template <typename T>
    using Queue = CircularBuffer<T>;
}

#endif
