#ifndef FOUNDATION_BUILD_SETTINGS_H
#define FOUNDATION_BUILD_SETTINGS_H
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Version

        #ifndef FOUNDATION_VERSION
            #define FOUNDATION_VERSION "2.0.2"
        #endif

    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // C++ Language Standard

        #ifndef FOUNDATION_CPLUSPLUS
            #if defined(_MSVC_LANG)
                #define FOUNDATION_CPLUSPLUS _MSVC_LANG
            #elif defined(__cplusplus)
                #define FOUNDATION_CPLUSPLUS __cplusplus
            #else
                #define FOUNDATION_CPLUSPLUS 0L
            #endif
        #endif

        // Arduino source builds accept the stock cores' C++11; every other
        // integration mode requires C++17.
        #if !defined(DOXYGEN) && defined(ARDUINO) && (FOUNDATION_CPLUSPLUS < 201103L)
            #error "Foundation requires C++11 or newer for Arduino source builds"
        #elif !defined(DOXYGEN) && !defined(ARDUINO) && (FOUNDATION_CPLUSPLUS < 201703L)
            #error "Foundation requires C++17 or newer"
        #endif

        // C++14 relaxed constexpr: mutators and multi-statement functions are
        // constexpr from C++14 and ordinary inline functions under C++11.
        #ifndef FOUNDATION_CONSTEXPR14
            #if FOUNDATION_CPLUSPLUS >= 201402L
                #define FOUNDATION_CONSTEXPR14 constexpr
            #else
                #define FOUNDATION_CONSTEXPR14 inline
            #endif
        #endif

    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // CPSTL
    //
    // Foundation builds on CPSTL (cpstd) for the standard vocabulary. Every
    // root header in src/ includes this file, so the Arduino builder also
    // discovers the CPSTL library from any Foundation include.

        #include <CPSTL_BuildSettings.h>

    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Doxygen

        #ifdef DOXYGEN
            #define __has_include(x) 1 // bypass header checks for Doxygen
        #endif
#endif//FOUNDATION_BUILD_SETTINGS_H
