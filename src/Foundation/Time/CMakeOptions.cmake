#################################################################################################################################
# CMake options for FOUNDATION
    if(FOUNDATION_FULL_BUILD)
        set(FOUNDATION_TIME_CLOCK ON)
        set(FOUNDATION_TIME_DURATION ON)
        set(FOUNDATION_TIME_FREQUENCY ON)
        set(FOUNDATION_TIME_PERIOD ON)
        set(FOUNDATION_TIME_TICK ON)
        set(FOUNDATION_TIME_TIME_POINT ON)
    endif()
#
#################################################################################################################################
# Time

    option(FOUNDATION_TIME_CLOCK "Enables the compilation of Foundation::Time::Clock" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Time/Clock/CMakeOptions.cmake)
#
#################################################################################################################################
# Duration

    option(FOUNDATION_TIME_DURATION "Enables the compilation of Foundation::Time::Duration" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Time/Duration/CMakeOptions.cmake)
#
#################################################################################################################################
# Frequency

    option(FOUNDATION_TIME_FREQUENCY "Enables the compilation of Foundation::Time::Frequency" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Time/Frequency/CMakeOptions.cmake)
#
#################################################################################################################################
# Period

    option(FOUNDATION_TIME_PERIOD "Enables the compilation of Foundation::Time::Period" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Time/Period/CMakeOptions.cmake)
#
#################################################################################################################################
# Tick

    option(FOUNDATION_TIME_TICK "Enables the compilation of Foundation::Time::Tick" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Time/Tick/CMakeOptions.cmake)
#
#################################################################################################################################
# TimePoint

    option(FOUNDATION_TIME_TIME_POINT "Enables the compilation of Foundation::Time::TimePoint" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Time/TimePoint/CMakeOptions.cmake)
#
#################################################################################################################################
