# Seeds or verifies files that must not survive a fresh configure and export.
#
# cmake -DPRESET=<configure preset> -D<mode> -P tests/cmake/FreshState.cmake
#
# Supported modes:
#   SEED_BUILD, SEED_DIST, VERIFY_CLEAN_BUILD, VERIFY_CLEAN_ALL, VERIFY

if(NOT DEFINED PRESET OR PRESET STREQUAL "")
    message(FATAL_ERROR "PRESET must be provided.")
endif()
if(NOT PRESET MATCHES "^[A-Za-z0-9][A-Za-z0-9_.-]*$"
        OR PRESET MATCHES "\\.\\.")
    message(FATAL_ERROR "Invalid PRESET '${PRESET}'.")
endif()
if(NOT DEFINED MODE)
    message(FATAL_ERROR
        "MODE must be SEED_BUILD, SEED_DIST, VERIFY_CLEAN_BUILD, "
        "VERIFY_CLEAN_ALL, or VERIFY."
    )
endif()

get_filename_component(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(BUILD_DIR "${SOURCE_DIR}/build/${PRESET}")
set(DIST_DIR "${SOURCE_DIR}/dist/${PRESET}")

set(STALE_BUILD_PATHS
    "${BUILD_DIR}/stale-state/OldTest"
    "${BUILD_DIR}/bin/Release/Foundation_StaleExample"
    "${BUILD_DIR}/FoundationBuildInfo.cmake.stale"
)
set(STALE_DIST_PATHS
    "${DIST_DIR}/include/Foundation/Stale.h"
    "${DIST_DIR}/bin/examples/Foundation_StaleExample"
    "${DIST_DIR}/lib/libFoundation_d.a"
    "${DIST_DIR}/lib/cmake/Foundation/FoundationTargets-debug.cmake"
)
set(STALE_PATHS ${STALE_BUILD_PATHS} ${STALE_DIST_PATHS})

if(MODE STREQUAL "SEED_BUILD")
    foreach(path IN LISTS STALE_BUILD_PATHS)
        get_filename_component(parent "${path}" DIRECTORY)
        file(MAKE_DIRECTORY "${parent}")
        file(WRITE "${path}" "stale state that a fresh workflow must remove\n")
    endforeach()
    message(STATUS "Seeded stale build state for ${PRESET}.")
elseif(MODE STREQUAL "SEED_DIST")
    foreach(path IN LISTS STALE_DIST_PATHS)
        get_filename_component(parent "${path}" DIRECTORY)
        file(MAKE_DIRECTORY "${parent}")
        file(WRITE "${path}" "stale state that a fresh workflow must remove\n")
    endforeach()
    message(STATUS "Seeded stale distribution state for ${PRESET}.")
elseif(MODE STREQUAL "VERIFY_CLEAN_BUILD")
    foreach(path IN LISTS STALE_BUILD_PATHS)
        if(EXISTS "${path}")
            message(FATAL_ERROR "Build-only clean retained: ${path}")
        endif()
    endforeach()
    foreach(path IN LISTS STALE_DIST_PATHS)
        if(NOT EXISTS "${path}")
            message(FATAL_ERROR "Build-only clean removed distribution path: ${path}")
        endif()
    endforeach()
    message(STATUS "Build-only clean scope verified for ${PRESET}.")
elseif(MODE STREQUAL "VERIFY_CLEAN_ALL")
    foreach(path IN LISTS STALE_PATHS)
        if(EXISTS "${path}")
            message(FATAL_ERROR "Build-and-dist clean retained: ${path}")
        endif()
    endforeach()
    message(STATUS "Build-and-dist clean scope verified for ${PRESET}.")
elseif(MODE STREQUAL "VERIFY")
    foreach(path IN LISTS STALE_PATHS)
        if(EXISTS "${path}")
            message(FATAL_ERROR "Fresh workflow retained stale path: ${path}")
        endif()
    endforeach()

    set(REQUIRED_PATHS
        "${BUILD_DIR}/CMakeCache.txt"
        "${DIST_DIR}/include/Foundation.h"
        "${DIST_DIR}/lib/cmake/Foundation/FoundationConfig.cmake"
        "${DIST_DIR}/lib/cmake/Foundation/FoundationBuildInfo.cmake"
    )
    foreach(path IN LISTS REQUIRED_PATHS)
        if(NOT EXISTS "${path}")
            message(FATAL_ERROR "Fresh workflow did not produce: ${path}")
        endif()
    endforeach()
    message(STATUS "Fresh build and distribution state verified for ${PRESET}.")
else()
    message(FATAL_ERROR
        "Unsupported MODE '${MODE}'."
    )
endif()
