#ifndef FOUNDATION_MATH_CORE_H
#define FOUNDATION_MATH_CORE_H


    #if __has_include(<Foundation/Math/Ratio.h>)
        #ifndef FOUNDATION_RATIO
            #define FOUNDATION_RATIO
        #endif

        #include <Foundation/Math/Ratio.h>
    #endif

    #if __has_include(<Foundation/Math/Complex.h>)
        #ifndef FOUNDATION_COMPLEX
            #define FOUNDATION_COMPLEX
        #endif

        #include <Foundation/Math/Complex.h>
    #endif

    #if __has_include(<Foundation/Math/Matrix.h>)
        #ifndef FOUNDATION_MATRIX
            #define FOUNDATION_MATRIX
        #endif

        #include <Foundation/Math/Matrix.h>
    #endif

#endif//FOUNDATION_MATH_CORE_H