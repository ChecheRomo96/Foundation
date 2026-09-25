set(FOUNDATION_MATH_TEST_SOURCES
    MathTests.cc
    FoundationUmbrellaTests.cc
    TopLevelMathTests.cc
)

if(FOUNDATION_MATH_MATRIX)
    list(APPEND FOUNDATION_MATH_TEST_SOURCES MatrixTests.cc)
endif()

foundation_add_test(FoundationMathTests ${FOUNDATION_MATH_TEST_SOURCES})
