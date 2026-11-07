cmake_minimum_required(VERSION 3.25)

foreach(required_variable
        FOUNDATION_SOURCE_DIR
        FOUNDATION_SOURCE_CONSUMER_SOURCE_DIR
        FOUNDATION_SOURCE_CONSUMER_BINARY_DIR)
    if(NOT DEFINED ${required_variable} OR "${${required_variable}}" STREQUAL "")
        message(FATAL_ERROR "${required_variable} is required")
    endif()
endforeach()

execute_process(
    COMMAND "${CMAKE_COMMAND}" -E remove_directory
        "${FOUNDATION_SOURCE_CONSUMER_BINARY_DIR}"
    RESULT_VARIABLE remove_result
)
if(NOT remove_result EQUAL 0)
    message(FATAL_ERROR "Could not clear the source-consumer build directory")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}"
        -S "${FOUNDATION_SOURCE_CONSUMER_SOURCE_DIR}"
        -B "${FOUNDATION_SOURCE_CONSUMER_BINARY_DIR}"
        "-DFOUNDATION_SOURCE_DIR=${FOUNDATION_SOURCE_DIR}"
    RESULT_VARIABLE configure_result
)
if(NOT configure_result EQUAL 0)
    message(FATAL_ERROR "Foundation source consumer configuration failed")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${FOUNDATION_SOURCE_CONSUMER_BINARY_DIR}"
    RESULT_VARIABLE build_result
)
if(NOT build_result EQUAL 0)
    message(FATAL_ERROR "Foundation source consumer build failed")
endif()

execute_process(
    COMMAND "${CMAKE_CTEST_COMMAND}"
        --test-dir "${FOUNDATION_SOURCE_CONSUMER_BINARY_DIR}"
        --output-on-failure
        --no-tests=error
    RESULT_VARIABLE test_result
)
if(NOT test_result EQUAL 0)
    message(FATAL_ERROR "Foundation source consumer tests failed")
endif()
