#################################################################################################################################
# CMake options for FOUNDATION
    if(FOUNDATION_FULL_BUILD)
        set(FOUNDATION_MATH_ARITHMETIC ON)
        set(FOUNDATION_MATH_RATIO ON)
    endif()
#
#################################################################################################################################
# Ratio

    option(FOUNDATION_MATH_RATIO "Enables the compilation of Foundation::Math::Ratio" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeOptions.cmake)
#
#################################################################################################################################
# Arithmetic

    option(FOUNDATION_MATH_ARITHMETIC "Enables the compilation of Foundation::Math::Arithmetic" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeOptions.cmake)
#
#################################################################################################################################
