#ifndef FOUNDATION_BUILD_SETTINGS_H
#define FOUNDATION_BUILD_SETTINGS_H
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Version

        #ifndef FOUNDATION_VERSION
            #define FOUNDATION_VERSION "1.0.0"
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

        #if !defined(DOXYGEN) && (FOUNDATION_CPLUSPLUS < 201703L)
            #error "Foundation 1.0.0 requires C++17 or newer"
        #endif

    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Doxygen

        #ifdef DOXYGEN
            #define __has_include(x) 1 // bypass header checks for Doxygen
        #endif
#endif//FOUNDATION_BUILD_SETTINGS_H
