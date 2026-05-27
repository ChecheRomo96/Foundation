#################################################################################################################################
# CMake options for FOUNDATION
    if(FOUNDATION_FULL_BUILD)
        set(FOUNDATION_MATH_MATRIX_FIXED ON)
        set(FOUNDATION_MATH_MATRIX_DYNAMIC ON)
    endif()
#
#################################################################################################################################
# Fixed Matrix

    option(FOUNDATION_MATH_MATRIX_FIXED "Enables the compilation of Foundation::Math::FixedMatrix" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeOptions.cmake)
#
#################################################################################################################################
# Dynamic Matrix

    option(FOUNDATION_MATH_MATRIX_DYNAMIC "Enables the compilation of Foundation::Math::DynamicMatrix" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeOptions.cmake)
#
#################################################################################################################################

