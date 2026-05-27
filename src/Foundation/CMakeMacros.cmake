######################################################################################################
# Add Compiler Definitions

    if(FOUNDATION_TYPE_TRAITS)
        message(STATUS "Foundation::TypeTraits")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_TYPE_TRAITS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_TYPE_TRAITS")

        #include(${CMAKE_CURRENT_LIST_DIR}/TypeTraits/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_MATH)
        message(STATUS "Foundation::Math")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_MATH)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_MATH")

        include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_TIME)
        message(STATUS "Foundation::Time")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_TIME)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_TIME")

        include(${CMAKE_CURRENT_LIST_DIR}/Time/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_FUNCTIONAL)
        message(STATUS "Foundation::Functional")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_FUNCTIONAL)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_FUNCTIONAL")

        include(${CMAKE_CURRENT_LIST_DIR}/Functional/CMakeMacros.cmake)
    endif() 

    if(FOUNDATION_UTILS)
        message(STATUS "Foundation::Utils")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_UTILS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_UTILS")

        include(${CMAKE_CURRENT_LIST_DIR}/Utils/CMakeMacros.cmake)
    endif()

# Other conditional flags can be added here as needed
######################################################################################################