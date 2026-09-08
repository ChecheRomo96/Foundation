#################################################################################################################################
# CMake options for FOUNDATION
    if(FOUNDATION_FULL_BUILD)
        set(FOUNDATION_MATH ON)
        set(FOUNDATION_CONTAINERS ON)
        set(FOUNDATION_SCHEDULING ON)
        set(FOUNDATION_TIME ON)
        set(FOUNDATION_TYPE_TRAITS ON)
        set(FOUNDATION_UTILS ON)
        set(FOUNDATION_FUNCTIONAL ON)
    endif()
#
#################################################################################################################################
# Type Traits

    option(FOUNDATION_TYPE_TRAITS "Enables the compilation of Foundation::TypeTraits" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/TypeTraits/CMakeOptions.cmake)
#
#################################################################################################################################
# Math

    option(FOUNDATION_MATH "Enables the compilation of Foundation::Math" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeOptions.cmake)
#
#################################################################################################################################
# Containers

    option(FOUNDATION_CONTAINERS "Enables the compilation of Foundation::Containers" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Containers/CMakeOptions.cmake)
#
#################################################################################################################################
# Scheduling

    option(FOUNDATION_SCHEDULING "Enables the compilation of Foundation::Scheduling" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Scheduling/CMakeOptions.cmake)
#
#################################################################################################################################
# Time

    option(FOUNDATION_TIME "Enables the compilation of Foundation::Time" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Time/CMakeOptions.cmake)
#
#################################################################################################################################
# Utils

    option(FOUNDATION_UTILS "Enables the compilation of Foundation::Utils" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Utils/CMakeOptions.cmake)
#
#################################################################################################################################
# Functional

    option(FOUNDATION_FUNCTIONAL "Enables the compilation of Foundation::Functional" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Functional/CMakeOptions.cmake)
#
#################################################################################################################################
