#ifndef FOUNDATION_UTILS_MODULE_H
#define FOUNDATION_UTILS_MODULE_H

    #include <Foundation_BuildSettings.h>

    #if __has_include(<Foundation/Utils/Move.h>)
        #ifndef FOUNDATION_UTILS_MOVE
            #define FOUNDATION_UTILS_MOVE
        #endif
        #include <Foundation/Utils/Move.h>
    #endif
    
    #if __has_include(<Foundation/Utils/CallbackHandler.h>)
        #ifndef FOUNDATION_UTILS_CALLBACK_HANDLER
            #define FOUNDATION_UTILS_CALLBACK_HANDLER
        #endif
        #include <Foundation/Utils/CallbackHandler.h>
    #endif

#endif//FOUNDATION_UTILS_MODULE_H
