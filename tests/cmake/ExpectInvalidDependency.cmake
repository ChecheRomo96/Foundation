if(NOT DEFINED FEATURE OR NOT DEFINED DEPENDENCY)
    message(FATAL_ERROR "FEATURE and DEPENDENCY must be provided.")
endif()

execute_process(
    COMMAND
        "${CMAKE_COMMAND}"
        "-D${DEPENDENCY}=OFF"
        -P "${CMAKE_CURRENT_LIST_DIR}/ValidateDependencies.cmake"
    RESULT_VARIABLE VALIDATION_RESULT
    OUTPUT_VARIABLE VALIDATION_STDOUT
    ERROR_VARIABLE VALIDATION_STDERR
)

set(VALIDATION_OUTPUT "${VALIDATION_STDOUT}\n${VALIDATION_STDERR}")
set(EXPECTED_MESSAGE "${FEATURE}=ON requires ${DEPENDENCY}=ON")

if(VALIDATION_RESULT EQUAL 0)
    message(FATAL_ERROR
        "Expected the invalid configuration to be rejected, but validation "
        "succeeded: ${FEATURE}=ON, ${DEPENDENCY}=OFF."
    )
endif()

string(FIND "${VALIDATION_OUTPUT}" "${EXPECTED_MESSAGE}" MESSAGE_POSITION)
if(MESSAGE_POSITION EQUAL -1)
    message(FATAL_ERROR
        "The invalid configuration was rejected without the expected message "
        "'${EXPECTED_MESSAGE}'.\n${VALIDATION_OUTPUT}"
    )
endif()

message(STATUS
    "Correctly rejected ${FEATURE}=ON with ${DEPENDENCY}=OFF."
)
