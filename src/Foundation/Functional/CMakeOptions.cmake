#################################################################################################################################
# CMake options for FOUNDATION
    if(FOUNDATION_FULL_BUILD)
        set(FOUNDATION_FUNCTIONAL_CALLBACK ON)
    endif()
#
#################################################################################################################################
# Callback

    option(FOUNDATION_FUNCTIONAL_CALLBACK "Enables the compilation of Foundation::Functional::Callback" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Functional/Callback/CMakeOptions.cmake)
#
#################################################################################################################################
