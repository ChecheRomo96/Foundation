######################################################################################################
# Add Compiler Definitions

    if(FOUNDATION_FUNCTIONAL_CALLBACK)
        message(STATUS "Foundation::Functional::Callback")
        target_compile_definitions(Foundation PUBLIC FOUNDATION_FUNCTIONAL_CALLBACK)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "FOUNDATION_FUNCTIONAL_CALLBACK")

        #include(${CMAKE_CURRENT_LIST_DIR}/Utils/Callback/CMakeMacros.cmake)
    endif()

# Other conditional flags can be added here as needed
######################################################################################################