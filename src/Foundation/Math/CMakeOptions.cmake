#################################################################################################################################
# CMake options for FOUNDATION
    if(FOUNDATION_FULL_BUILD)
        set(FOUNDATION_MATH_RATIO ON)
        set(FOUNDATION_MATH_COMPLEX ON)
        set(FOUNDATION_MATH_MATRIX ON)
    endif()
#
#################################################################################################################################
# Ratio

    option(FOUNDATION_MATH_RATIO "Enables the compilation of Foundation::Math::Ratio" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeOptions.cmake)
#
#################################################################################################################################
# Complex

    option(FOUNDATION_MATH_COMPLEX "Enables the compilation of Foundation::Math::Complex" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeOptions.cmake)
#
#################################################################################################################################
# Matrix

    option(FOUNDATION_MATH_MATRIX "Enables the compilation of Foundation::Math::Matrix" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Matrix/CMakeOptions.cmake)
#
#################################################################################################################################
