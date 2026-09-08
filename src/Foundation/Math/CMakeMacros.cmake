######################################################################################################
# Add Compiler Definitions
    
    if(FOUNDATION_MATH_RATIO)
        message(STATUS "Foundation::Math::Ratio")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_MATH_RATIO)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_MATH_RATIO")

        #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeMacros.cmake)
    endif()
    
    if(FOUNDATION_MATH_COMPLEX)
        message(STATUS "Foundation::Math::Complex")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_MATH_COMPLEX)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_MATH_COMPLEX")

        #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeMacros.cmake)
    endif()
    
    if(FOUNDATION_MATH_MATRIX)
        message(STATUS "Foundation::Math::Matrix")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_MATH_MATRIX)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_MATH_MATRIX")

        include(${CMAKE_CURRENT_LIST_DIR}/Matrix/CMakeMacros.cmake)
    endif()


# Other conditional flags can be added here as needed
######################################################################################################