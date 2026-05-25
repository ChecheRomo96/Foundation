######################################################################################################
# Add Compiler Definitions

    if(FOUNDATION_UTILS_MOVE)
        message(STATUS "Foundation::Utils::Move")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_UTILS_MOVE)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_UTILS_MOVE")

        #include(${CMAKE_CURRENT_LIST_DIR}/Utils/Move/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_UTILS_DELEGATE)
        message(STATUS "Foundation::Utils::Delegate")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_UTILS_DELEGATE)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_UTILS_DELEGATE")

        #include(${CMAKE_CURRENT_LIST_DIR}/Utils/Delegate/CMakeMacros.cmake)
    endif()

# Other conditional flags can be added here as needed
######################################################################################################