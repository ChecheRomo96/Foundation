######################################################################################################
# Add Compiler Definitions

    if(FOUNDATION_TIME_CLOCK)
        message(STATUS "Foundation::Time::Clock")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_TIME_CLOCK)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_TIME_CLOCK")

        #include(${CMAKE_CURRENT_LIST_DIR}/Time/Clock/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_TIME_DURATION)
        message(STATUS "Foundation::Time::Duration")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_TIME_DURATION)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_TIME_DURATION")

        #include(${CMAKE_CURRENT_LIST_DIR}/Time/Duration/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_TIME_FREQUENCY)
        message(STATUS "Foundation::Time::Frequency")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_TIME_FREQUENCY)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_TIME_FREQUENCY")

        #include(${CMAKE_CURRENT_LIST_DIR}/Time/Frequency/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_TIME_PERIOD)
        message(STATUS "Foundation::Time::Period")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_TIME_PERIOD)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_TIME_PERIOD")

        #include(${CMAKE_CURRENT_LIST_DIR}/Time/Period/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_TIME_TICK)
        message(STATUS "Foundation::Time::Tick")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_TIME_TICK)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_TIME_TICK")

        #include(${CMAKE_CURRENT_LIST_DIR}/Time/Tick/CMakeMacros.cmake)
    endif()

    if(FOUNDATION_TIME_TIME_POINT)
        message(STATUS "Foundation::Time::TimePoint")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_TIME_TIME_POINT)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_TIME_TIME_POINT")

        #include(${CMAKE_CURRENT_LIST_DIR}/Time/TimePoint/CMakeMacros.cmake)
    endif()

# Other conditional flags can be added here as needed
######################################################################################################