######################################################################################################
# Add Compiler Definitions

    if(FOUNDATION_MATH_ARITHMETIC)
        message(STATUS "Foundation::Math::Arithmetic")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_MATH_ARITHMETIC)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_MATH_ARITHMETIC")

        #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeMacros.cmake)
    endif()
    
    if(FOUNDATION_MATH_RATIO)
        message(STATUS "Foundation::Math::Ratio")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_MATH_RATIO)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_MATH_RATIO")

        #include(${CMAKE_CURRENT_LIST_DIR}/Math/CMakeMacros.cmake)
    endif()


# Other conditional flags can be added here as needed
######################################################################################################