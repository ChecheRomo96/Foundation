cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED PRESET OR PRESET STREQUAL "")
    message(FATAL_ERROR "PRESET is required")
endif()

get_filename_component(FOUNDATION_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../.."
    ABSOLUTE
)

if(NOT DEFINED BUILD_DIR OR BUILD_DIR STREQUAL "")
    set(BUILD_DIR "${FOUNDATION_ROOT}/build/${PRESET}")
endif()
if(NOT DEFINED PACKAGE_PREFIX OR PACKAGE_PREFIX STREQUAL "")
    set(PACKAGE_PREFIX "${FOUNDATION_ROOT}/dist/${PRESET}")
endif()

get_filename_component(BUILD_DIR "${BUILD_DIR}" ABSOLUTE)
get_filename_component(PACKAGE_PREFIX "${PACKAGE_PREFIX}" ABSOLUTE)

set(BUILD_INFO "${BUILD_DIR}/FoundationBuildInfo.cmake")
set(PACKAGE_CMAKE_DIR "${PACKAGE_PREFIX}/lib/cmake/Foundation")
set(PACKAGE_BUILD_INFO "${PACKAGE_CMAKE_DIR}/FoundationBuildInfo.cmake")
set(PACKAGE_TARGETS "${PACKAGE_CMAKE_DIR}/FoundationTargets.cmake")
set(PACKAGE_RELEASE_TARGETS
    "${PACKAGE_CMAKE_DIR}/FoundationTargets-release.cmake"
)

function(foundation_require_file path description)
    if(NOT EXISTS "${path}" OR IS_DIRECTORY "${path}")
        message(FATAL_ERROR "Missing ${description}: ${path}")
    endif()
endfunction()

function(foundation_report_list_difference description expected actual)
    set(missing ${expected})
    if(actual)
        list(REMOVE_ITEM missing ${actual})
    endif()

    set(extra ${actual})
    if(expected)
        list(REMOVE_ITEM extra ${expected})
    endif()

    message(FATAL_ERROR
        "${description} mismatch\n"
        "Missing: ${missing}\n"
        "Extra: ${extra}"
    )
endfunction()

foundation_require_file("${BUILD_INFO}" "configured build identity")
foundation_require_file("${PACKAGE_BUILD_INFO}" "installed build identity")
foundation_require_file("${PACKAGE_CMAKE_DIR}/FoundationConfig.cmake"
    "package configuration"
)
foundation_require_file("${PACKAGE_CMAKE_DIR}/FoundationConfigVersion.cmake"
    "package version configuration"
)
foundation_require_file("${PACKAGE_TARGETS}" "imported target definition")
foundation_require_file("${PACKAGE_RELEASE_TARGETS}"
    "Release imported target definition"
)

foreach(document LICENSE README.md CHANGELOG.md)
    foundation_require_file("${PACKAGE_PREFIX}/${document}"
        "packaged ${document}"
    )
endforeach()

file(SHA256 "${BUILD_INFO}" BUILD_INFO_SHA256)
file(SHA256 "${PACKAGE_BUILD_INFO}" PACKAGE_BUILD_INFO_SHA256)
if(NOT BUILD_INFO_SHA256 STREQUAL PACKAGE_BUILD_INFO_SHA256)
    message(FATAL_ERROR
        "Installed FoundationBuildInfo.cmake does not match the configured "
        "build identity"
    )
endif()

include("${PACKAGE_BUILD_INFO}")

if(NOT Foundation_PLATFORM STREQUAL PRESET)
    message(FATAL_ERROR
        "Package platform '${Foundation_PLATFORM}' does not match preset "
        "'${PRESET}'"
    )
endif()
if(NOT Foundation_PACKAGE_CONFIGURATION STREQUAL "Release")
    message(FATAL_ERROR
        "Package configuration must be Release, got "
        "'${Foundation_PACKAGE_CONFIGURATION}'"
    )
endif()

foreach(required_identity
        Foundation_VERSION
        Foundation_PLATFORM
        Foundation_SYSTEM_NAME
        Foundation_SYSTEM_PROCESSOR
        Foundation_CXX_COMPILER_ID
        Foundation_CXX_COMPILER_VERSION
        Foundation_CXX_COMPILER_FRONTEND_VARIANT
        Foundation_CXX_SIZEOF_DATA_PTR
        Foundation_CXX_BYTE_ORDER
        Foundation_CXX_STANDARD
)
    if(NOT DEFINED ${required_identity} OR "${${required_identity}}" STREQUAL "")
        message(FATAL_ERROR
            "Package identity field ${required_identity} is empty"
        )
    endif()
endforeach()

file(GLOB PACKAGE_METADATA
    LIST_DIRECTORIES false
    RELATIVE "${PACKAGE_CMAKE_DIR}"
    "${PACKAGE_CMAKE_DIR}/*.cmake"
)
list(SORT PACKAGE_METADATA)
set(EXPECTED_METADATA
    FoundationBuildInfo.cmake
    FoundationConfig.cmake
    FoundationConfigVersion.cmake
    FoundationTargets-release.cmake
    FoundationTargets.cmake
)
list(SORT EXPECTED_METADATA)
if(NOT "${PACKAGE_METADATA}" STREQUAL "${EXPECTED_METADATA}")
    foundation_report_list_difference(
        "CMake package metadata"
        "${EXPECTED_METADATA}"
        "${PACKAGE_METADATA}"
    )
endif()

file(GLOB_RECURSE PACKAGE_ARCHIVES
    LIST_DIRECTORIES false
    "${PACKAGE_PREFIX}/lib/*.a"
    "${PACKAGE_PREFIX}/lib/*.lib"
)
# A Foundation build that compiled CPSTL from source installs it alongside,
# as its own package.
set(CPSTL_ARCHIVES ${PACKAGE_ARCHIVES})
list(FILTER CPSTL_ARCHIVES INCLUDE REGEX "/(libCPSTL\\.a|CPSTL\\.lib)$")
list(FILTER PACKAGE_ARCHIVES EXCLUDE REGEX "/(libCPSTL\\.a|CPSTL\\.lib)$")
if(CPSTL_ARCHIVES AND
   NOT EXISTS "${PACKAGE_PREFIX}/lib/cmake/CPSTL/CPSTLConfig.cmake")
    message(FATAL_ERROR
        "The package bundles ${CPSTL_ARCHIVES} without a CPSTL CMake package")
endif()
list(LENGTH PACKAGE_ARCHIVES PACKAGE_ARCHIVE_COUNT)
if(NOT PACKAGE_ARCHIVE_COUNT EQUAL 1)
    message(FATAL_ERROR
        "Expected exactly one static Release archive; found "
        "${PACKAGE_ARCHIVE_COUNT}: ${PACKAGE_ARCHIVES}"
    )
endif()
list(GET PACKAGE_ARCHIVES 0 PACKAGE_ARCHIVE)
get_filename_component(PACKAGE_ARCHIVE_NAME "${PACKAGE_ARCHIVE}" NAME)

if(Foundation_SYSTEM_NAME STREQUAL "Windows")
    set(EXPECTED_ARCHIVE_NAME "Foundation.lib")
else()
    set(EXPECTED_ARCHIVE_NAME "libFoundation.a")
endif()
if(NOT PACKAGE_ARCHIVE_NAME STREQUAL EXPECTED_ARCHIVE_NAME)
    message(FATAL_ERROR
        "Expected Release archive ${EXPECTED_ARCHIVE_NAME}; found "
        "${PACKAGE_ARCHIVE_NAME}"
    )
endif()

file(GLOB_RECURSE PACKAGE_FILES
    LIST_DIRECTORIES false
    RELATIVE "${PACKAGE_PREFIX}"
    "${PACKAGE_PREFIX}/*"
)
foreach(package_file IN LISTS PACKAGE_FILES)
    get_filename_component(package_name "${package_file}" NAME)
    if(package_name MATCHES "_d\\.(a|lib|dll|dylib|so|exe)$" OR
       package_name STREQUAL "FoundationTargets-debug.cmake")
        message(FATAL_ERROR
            "Debug artifact found in Release package: ${package_file}"
        )
    endif()
endforeach()

file(READ "${PACKAGE_RELEASE_TARGETS}" RELEASE_TARGETS_CONTENT)
if(RELEASE_TARGETS_CONTENT MATCHES "IMPORTED_LOCATION_DEBUG")
    message(FATAL_ERROR "Release target metadata contains a Debug import")
endif()
string(FIND "${RELEASE_TARGETS_CONTENT}"
    "IMPORTED_LOCATION_RELEASE" RELEASE_LOCATION_INDEX
)
string(FIND "${RELEASE_TARGETS_CONTENT}"
    "${EXPECTED_ARCHIVE_NAME}" RELEASE_ARCHIVE_INDEX
)
if(RELEASE_LOCATION_INDEX EQUAL -1 OR RELEASE_ARCHIVE_INDEX EQUAL -1)
    message(FATAL_ERROR
        "Release target metadata does not import ${EXPECTED_ARCHIVE_NAME}"
    )
endif()

set(STAGED_INCLUDE_DIR
    "${BUILD_DIR}/include/Foundation-${Foundation_VERSION}"
)
set(PACKAGE_INCLUDE_DIR "${PACKAGE_PREFIX}/include")
if(NOT IS_DIRECTORY "${STAGED_INCLUDE_DIR}")
    message(FATAL_ERROR
        "Configured public-header tree not found: ${STAGED_INCLUDE_DIR}"
    )
endif()
if(NOT IS_DIRECTORY "${PACKAGE_INCLUDE_DIR}")
    message(FATAL_ERROR
        "Package public-header tree not found: ${PACKAGE_INCLUDE_DIR}"
    )
endif()

file(GLOB_RECURSE STAGED_HEADERS
    LIST_DIRECTORIES false
    RELATIVE "${STAGED_INCLUDE_DIR}"
    "${STAGED_INCLUDE_DIR}/*"
)
file(GLOB_RECURSE PACKAGE_HEADERS
    LIST_DIRECTORIES false
    RELATIVE "${PACKAGE_INCLUDE_DIR}"
    "${PACKAGE_INCLUDE_DIR}/*"
)
# CPSTL headers installed with a bundled CPSTL belong to that package.
if(CPSTL_ARCHIVES)
    list(FILTER PACKAGE_HEADERS EXCLUDE REGEX "^CPSTL/")
endif()
list(SORT STAGED_HEADERS)
list(SORT PACKAGE_HEADERS)
if(NOT "${STAGED_HEADERS}" STREQUAL "${PACKAGE_HEADERS}")
    foundation_report_list_difference(
        "Installed public headers"
        "${STAGED_HEADERS}"
        "${PACKAGE_HEADERS}"
    )
endif()

list(LENGTH PACKAGE_HEADERS PACKAGE_HEADER_COUNT)
message(STATUS
    "Validated package contents: ${PACKAGE_ARCHIVE_NAME}, "
    "${PACKAGE_HEADER_COUNT} public headers, Release-only metadata"
)
message(STATUS
    "Validated package identity: ${Foundation_PLATFORM}; "
    "${Foundation_SYSTEM_NAME}/${Foundation_SYSTEM_PROCESSOR}; "
    "${Foundation_CXX_COMPILER_ID} ${Foundation_CXX_COMPILER_VERSION}; "
    "${Foundation_CXX_COMPILER_ABI}; ${Foundation_CXX_SIZEOF_DATA_PTR}-byte "
    "pointers; ${Foundation_CXX_BYTE_ORDER}"
)
