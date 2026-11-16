cmake_minimum_required(VERSION 3.25)

set(FOUNDATION_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(FOUNDATION_VERSION_FILE "${FOUNDATION_ROOT}/VERSION")
set(FOUNDATION_ARDUINO_METADATA "${FOUNDATION_ROOT}/library.properties")
set(FOUNDATION_BUILD_SETTINGS "${FOUNDATION_ROOT}/src/Foundation_BuildSettings.h")

function(foundation_require_semantic_version value description)
    if(NOT "${value}" MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+$")
        message(FATAL_ERROR "${description} must be a semantic version, got '${value}'")
    endif()
endfunction()

function(foundation_read_source_version output)
    file(STRINGS "${FOUNDATION_VERSION_FILE}" source_version LIMIT_COUNT 1)
    string(STRIP "${source_version}" source_version)
    foundation_require_semantic_version("${source_version}" "VERSION")
    set("${output}" "${source_version}" PARENT_SCOPE)
endfunction()

function(foundation_read_arduino_version output)
    file(STRINGS "${FOUNDATION_ARDUINO_METADATA}" arduino_version_line
        REGEX "^version=" LIMIT_COUNT 1)
    string(REGEX REPLACE "^version=" "" arduino_version "${arduino_version_line}")
    foundation_require_semantic_version("${arduino_version}" "library.properties version")
    set("${output}" "${arduino_version}" PARENT_SCOPE)
endfunction()

function(foundation_read_header_version output)
    file(STRINGS "${FOUNDATION_BUILD_SETTINGS}" header_version_line
        REGEX "^[ \\t]*#define[ \\t]+FOUNDATION_VERSION[ \\t]+\"[^\"]+\""
        LIMIT_COUNT 1)
    if(NOT header_version_line MATCHES "\"([^\"]+)\"")
        message(FATAL_ERROR "Foundation_BuildSettings.h does not declare FOUNDATION_VERSION")
    endif()
    set("${output}" "${CMAKE_MATCH_1}" PARENT_SCOPE)
endfunction()

function(foundation_verify_version_metadata expected_version)
    foundation_read_arduino_version(arduino_version)
    foundation_read_header_version(header_version)

    foreach(metadata IN ITEMS
            "library.properties=${arduino_version}"
            "Foundation_BuildSettings.h=${header_version}")
        string(REPLACE "=" ";" metadata_parts "${metadata}")
        list(GET metadata_parts 0 metadata_name)
        list(GET metadata_parts 1 metadata_version)
        if(NOT metadata_version STREQUAL expected_version)
            message(FATAL_ERROR
                "Version metadata is out of sync: VERSION is ${expected_version}, "
                "but ${metadata_name} is ${metadata_version}. Run:\n"
                "  cmake -DFOUNDATION_VERSION=${expected_version} -P cmake/SyncVersion.cmake"
            )
        endif()
    endforeach()

    message(STATUS "Foundation version metadata is synchronized at ${expected_version}")
endfunction()

if(DEFINED FOUNDATION_VERSION AND NOT FOUNDATION_VERSION STREQUAL "")
    set(version_to_sync "${FOUNDATION_VERSION}")
    foundation_require_semantic_version("${version_to_sync}" "FOUNDATION_VERSION")

    foundation_read_arduino_version(current_arduino_version)
    file(READ "${FOUNDATION_ARDUINO_METADATA}" arduino_metadata)
    string(REPLACE "version=${current_arduino_version}" "version=${version_to_sync}"
        arduino_metadata "${arduino_metadata}")
    file(WRITE "${FOUNDATION_ARDUINO_METADATA}" "${arduino_metadata}")

    foundation_read_header_version(current_header_version)
    file(READ "${FOUNDATION_BUILD_SETTINGS}" build_settings)
    string(REPLACE "FOUNDATION_VERSION \"${current_header_version}\""
        "FOUNDATION_VERSION \"${version_to_sync}\"" build_settings "${build_settings}")
    file(WRITE "${FOUNDATION_BUILD_SETTINGS}" "${build_settings}")
    file(WRITE "${FOUNDATION_VERSION_FILE}" "${version_to_sync}\n")

    foundation_verify_version_metadata("${version_to_sync}")
elseif(FOUNDATION_VERIFY_VERSION_SYNC)
    foundation_read_source_version(source_version)
    foundation_verify_version_metadata("${source_version}")
else()
    message(FATAL_ERROR
        "Set FOUNDATION_VERSION to synchronize metadata or "
        "FOUNDATION_VERIFY_VERSION_SYNC=ON to verify it"
    )
endif()
