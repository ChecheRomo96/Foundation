cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED PRESET OR PRESET STREQUAL "")
    message(FATAL_ERROR "PRESET is required")
endif()

get_filename_component(FOUNDATION_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../.."
    ABSOLUTE
)

set(BUILD_DIR "${FOUNDATION_ROOT}/build/${PRESET}")
set(SOURCE_PACKAGE "${FOUNDATION_ROOT}/dist/${PRESET}")
set(NEGATIVE_ROOT
    "${FOUNDATION_ROOT}/build/package-identity-negative/${PRESET}"
)

if(NOT IS_DIRECTORY "${SOURCE_PACKAGE}")
    message(FATAL_ERROR "Package prefix not found: ${SOURCE_PACKAGE}")
endif()

function(foundation_expect_identity_failure scenario)
    set(TEST_PACKAGE "${NEGATIVE_ROOT}/${scenario}")
    file(REMOVE_RECURSE "${TEST_PACKAGE}")
    file(MAKE_DIRECTORY "${TEST_PACKAGE}")
    file(COPY "${SOURCE_PACKAGE}/" DESTINATION "${TEST_PACKAGE}")

    if(scenario STREQUAL "debug-archive")
        file(WRITE "${TEST_PACKAGE}/lib/Foundation_d.lib" "not an archive")
    elseif(scenario STREQUAL "missing-header")
        file(REMOVE "${TEST_PACKAGE}/include/Foundation.h")
    elseif(scenario STREQUAL "identity-mismatch")
        file(APPEND
            "${TEST_PACKAGE}/lib/cmake/Foundation/FoundationBuildInfo.cmake"
            "\n# deliberately changed identity\n"
        )
    else()
        message(FATAL_ERROR "Unknown negative identity scenario: ${scenario}")
    endif()

    execute_process(
        COMMAND "${CMAKE_COMMAND}"
            "-DPRESET=${PRESET}"
            "-DBUILD_DIR=${BUILD_DIR}"
            "-DPACKAGE_PREFIX=${TEST_PACKAGE}"
            -P "${CMAKE_CURRENT_LIST_DIR}/PackageIdentity.cmake"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error
    )
    if(result EQUAL 0)
        message(FATAL_ERROR
            "Package identity validator accepted the '${scenario}' mutation"
        )
    endif()
    message(STATUS
        "Package identity validator rejected '${scenario}' as expected"
    )
endfunction()

foundation_expect_identity_failure(debug-archive)
foundation_expect_identity_failure(missing-header)
foundation_expect_identity_failure(identity-mismatch)

file(REMOVE_RECURSE "${NEGATIVE_ROOT}")
