add_subdirectory(Arithmetic)

if(FOUNDATION_MATH_RATIO)
    add_subdirectory(Ratio)
endif()

if(FOUNDATION_MATH_COMPLEX)
    add_subdirectory(Complex)
endif()

if(FOUNDATION_MATH_MATRIX)
    add_subdirectory(Matrix)
endif()
