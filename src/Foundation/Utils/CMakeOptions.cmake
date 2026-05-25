#################################################################################################################################
# CMake options for FOUNDATION
    if(FOUNDATION_FULL_BUILD)
        set(FOUNDATION_UTILS_MOVE ON)
        set(FOUNDATION_UTILS_DELEGATE ON)
    endif()
#
#################################################################################################################################
# Move

    option(FOUNDATION_UTILS_MOVE "Enables the compilation of Foundation::Utils::Move" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Utils/Move/CMakeOptions.cmake)
#
#################################################################################################################################
# Delegate

    option(FOUNDATION_UTILS_DELEGATE "Enables the compilation of Foundation::Utils::Delegate" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Utils/Delegate/CMakeOptions.cmake)
#
#################################################################################################################################
