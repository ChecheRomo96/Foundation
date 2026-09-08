#ifndef FOUNDATION_H
#define FOUNDATION_H

    #include "Foundation_BuildSettings.h"

    #if __has_include("Foundation_TypeTraits.h")
        #include "Foundation_TypeTraits.h"
    #endif

    #if __has_include("Foundation_Math.h")
        #include "Foundation_Math.h"
    #endif

    #if __has_include("Foundation_Time.h")
        #include "Foundation_Time.h"
    #endif

    #if __has_include("Foundation_Containers.h")
        #include "Foundation_Containers.h"
    #endif

    #if __has_include(<Foundation/Functional.h>)
        #include <Foundation/Functional.h>
    #endif

    #if __has_include("Foundation_Scheduling.h")
        #include "Foundation_Scheduling.h"
    #endif

    #if __has_include("Foundation_Utils.h")
        #include "Foundation_Utils.h"
    #endif

#endif
