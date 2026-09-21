if(FOUNDATION_MATH)
    add_subdirectory(Math)
endif()

if(FOUNDATION_TIME AND FOUNDATION_MATH AND FOUNDATION_MATH_RATIO)
    add_subdirectory(Time)
endif()

if(FOUNDATION_TYPE_TRAITS)
    add_subdirectory(TypeTraits)
endif()
