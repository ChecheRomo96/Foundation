#ifndef FOUNDATION_TIME_TICK_H
#define FOUNDATION_TIME_TICK_H

#include <stdint.h>

namespace Foundation {
    namespace Time {

        /**
         * @brief Unsigned 32-bit clock tick value.
         * @ingroup Foundation_Time_Tick
         *
         * Tick arithmetic follows normal `uint32_t` wraparound semantics.
         */
        using Tick = uint32_t;

    }
}

#endif
