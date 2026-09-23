#ifndef FOUNDATION_CONTAINERS_QUEUE_H
#define FOUNDATION_CONTAINERS_QUEUE_H

#include "CircularBuffer.h"

namespace Foundation::Containers {

    /**
     * @brief FIFO spelling of CircularBuffer.
     * @ingroup Foundation_Containers
     * @tparam T Stored value type.
     *
     * Queue has exactly the construction, capacity, ownership, and operation
     * semantics of CircularBuffer.
     */
    template <typename T>
    using Queue = CircularBuffer<T>;
}

#endif
