#ifndef FOUNDATION_MATH_MODULE_H
#define FOUNDATION_MATH_MODULE_H

    #include <Foundation_BuildSettings.h>

    #if __has_include(<Foundation/Math/Ratio.h>)
        #ifndef FOUNDATION_MATH_RATIO
            #define FOUNDATION_MATH_RATIO
        #endif

        #include <Foundation/Math/Ratio.h>
    #endif

    #if __has_include(<Foundation/Math/Complex.h>)
        #ifndef FOUNDATION_MATH_COMPLEX
            #define FOUNDATION_MATH_COMPLEX
        #endif

        #include <Foundation/Math/Complex.h>
    #endif

    #if __has_include(<Foundation/Math/Matrix.h>)
        #ifndef FOUNDATION_MATH_MATRIX
            #define FOUNDATION_MATH_MATRIX
        #endif

        #include <Foundation/Math/Matrix.h>
    #endif

    #include <Foundation/Math/Arithmetic.h>
    #include <Foundation/Math/Trigonometry.h>

#endif//FOUNDATION_MATH_MODULE_H
