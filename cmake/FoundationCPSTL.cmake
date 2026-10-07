# Resolves the CPSTL dependency and defines CPSTL::CPSTL.
#
# Foundation builds on CPSTL for the standard vocabulary (cpstd::vector,
# cpstd::move, type traits): cpstd aliases std in CPSTL's STL mode and is
# CPSTL's own implementation elsewhere, such as AVR.
#
# Resolution order:
#   0. A CPSTL::CPSTL target already defined by a parent project.
#   1. FOUNDATION_CPSTL_PREFIX (cache or environment): an explicit package
#      prefix. Failing to use it is a fatal error.
#   2. ../CPSTL/dist/<FOUNDATION_PLATFORM>: a sibling export
#      (`./scripts/install.sh <preset>` in CPSTL).
#   3. Normal find_package() search: CPSTL_DIR, CMAKE_PREFIX_PATH and system
#      locations.
#   4. FOUNDATION_FETCH_CPSTL (default ON): the CPSTL sources at tag
#      v<FOUNDATION_CPSTL_VERSION>, built as part of Foundation and installed
#      with it. FETCHCONTENT_SOURCE_DIR_CPSTL points this at a local working
#      copy instead, for example -DFETCHCONTENT_SOURCE_DIR_CPSTL=../CPSTL.
#
# Outputs: CPSTL_VERSION and FOUNDATION_CPSTL_SOURCE.

set(FOUNDATION_CPSTL_VERSION "1.1.3")
set(FOUNDATION_CPSTL_REPOSITORY "ChecheRomo96/CPSTL" CACHE STRING
    "GitHub repository (owner/name) used to fetch CPSTL")
option(FOUNDATION_FETCH_CPSTL
    "Fetch CPSTL from GitHub when no local package is found" ON)

set(FOUNDATION_CPSTL_SOURCE "")
set(FOUNDATION_USER_CPSTL_DIR "${CPSTL_DIR}")

macro(foundation_restore_cpstl_dir)
    if(FOUNDATION_USER_CPSTL_DIR AND
       NOT FOUNDATION_USER_CPSTL_DIR MATCHES "-NOTFOUND$")
        set(CPSTL_DIR "${FOUNDATION_USER_CPSTL_DIR}" CACHE PATH
            "Directory containing CPSTLConfig.cmake" FORCE)
    else()
        unset(CPSTL_DIR CACHE)
    endif()
endmacro()

function(foundation_find_cpstl_package prefix)
    unset(CPSTL_DIR CACHE)
    find_package(CPSTL ${FOUNDATION_CPSTL_VERSION} CONFIG QUIET
        PATHS "${prefix}"
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH)
    foundation_restore_cpstl_dir()
    if(TARGET CPSTL::CPSTL)
        set(CPSTL_VERSION "${CPSTL_VERSION}" PARENT_SCOPE)
    endif()
endfunction()

# 0. Target provided by a parent project.
if(TARGET CPSTL::CPSTL)
    set(FOUNDATION_CPSTL_SOURCE "parent project")
endif()

# 1. Explicit prefix.
if(NOT TARGET CPSTL::CPSTL AND NOT DEFINED FOUNDATION_CPSTL_PREFIX AND
   NOT "$ENV{FOUNDATION_CPSTL_PREFIX}" STREQUAL "")
    set(FOUNDATION_CPSTL_PREFIX "$ENV{FOUNDATION_CPSTL_PREFIX}")
endif()
set(FOUNDATION_CPSTL_PREFIX "${FOUNDATION_CPSTL_PREFIX}" CACHE PATH
    "CPSTL package prefix for the same platform and ABI")

if(NOT TARGET CPSTL::CPSTL AND NOT FOUNDATION_CPSTL_PREFIX STREQUAL "")
    foundation_find_cpstl_package("${FOUNDATION_CPSTL_PREFIX}")
    if(NOT TARGET CPSTL::CPSTL)
        message(FATAL_ERROR
            "FOUNDATION_CPSTL_PREFIX='${FOUNDATION_CPSTL_PREFIX}' does not "
            "contain a CPSTL ${FOUNDATION_CPSTL_VERSION}+ (1.x) package.")
    endif()
    set(FOUNDATION_CPSTL_SOURCE "prefix ${FOUNDATION_CPSTL_PREFIX}")
endif()

# 2. Sibling export.
if(NOT TARGET CPSTL::CPSTL AND FOUNDATION_PLATFORM)
    get_filename_component(foundation_cpstl_sibling
        "${FOUNDATION_ROOT_DIRECTORY}/../CPSTL/dist/${FOUNDATION_PLATFORM}" ABSOLUTE)
    if(EXISTS "${foundation_cpstl_sibling}/lib/cmake/CPSTL/CPSTLConfig.cmake")
        foundation_find_cpstl_package("${foundation_cpstl_sibling}")
        if(TARGET CPSTL::CPSTL)
            set(FOUNDATION_CPSTL_SOURCE "sibling export")
        else()
            message(WARNING
                "Ignoring ${foundation_cpstl_sibling}: it is not a compatible "
                "CPSTL ${FOUNDATION_CPSTL_VERSION}+ (1.x) package.")
        endif()
    endif()
endif()

# 3. Normal package search.
if(NOT TARGET CPSTL::CPSTL AND NOT FETCHCONTENT_SOURCE_DIR_CPSTL)
    foundation_restore_cpstl_dir()
    find_package(CPSTL ${FOUNDATION_CPSTL_VERSION} CONFIG QUIET)
    if(TARGET CPSTL::CPSTL)
        set(FOUNDATION_CPSTL_SOURCE "${CPSTL_DIR}")
    else()
        foundation_restore_cpstl_dir()
    endif()
endif()

# 4. Sources, from GitHub or a local working copy.
if(NOT TARGET CPSTL::CPSTL AND
   (FOUNDATION_FETCH_CPSTL OR FETCHCONTENT_SOURCE_DIR_CPSTL))
    include(FetchContent)
    if(FETCHCONTENT_SOURCE_DIR_CPSTL)
        get_filename_component(FETCHCONTENT_SOURCE_DIR_CPSTL
            "${FETCHCONTENT_SOURCE_DIR_CPSTL}" ABSOLUTE
            BASE_DIR "${FOUNDATION_ROOT_DIRECTORY}")
        message(STATUS "Using CPSTL sources at ${FETCHCONTENT_SOURCE_DIR_CPSTL}")
    else()
        message(STATUS "Fetching CPSTL sources at v${FOUNDATION_CPSTL_VERSION}")
    endif()
    # CPSTL keeps its defaults (own implementation, vector enabled). Its own
    # tests and examples stay off inside Foundation.
    set(CPSTL_TESTING OFF CACHE BOOL "" FORCE)
    set(CPSTL_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(CPSTL_AVR_SMOKE OFF CACHE BOOL "" FORCE)
    # Freestanding Arm and AVR consumers do not link a C++ runtime. Keep
    # CPSTL's allocator on malloc/free unless an embedding project explicitly
    # selects another backend.
    if((CMAKE_SYSTEM_NAME STREQUAL "Generic" OR
        CMAKE_SYSTEM_PROCESSOR MATCHES "^avr") AND
       NOT DEFINED CPSTL_ALLOCATION)
        set(CPSTL_ALLOCATION "C" CACHE STRING
            "cpstd::allocator backend: C, CPP or STD")
    endif()
    FetchContent_Declare(CPSTL
        GIT_REPOSITORY "https://github.com/${FOUNDATION_CPSTL_REPOSITORY}.git"
        GIT_TAG "v${FOUNDATION_CPSTL_VERSION}"
        GIT_SHALLOW TRUE
        GIT_SUBMODULES "")
    FetchContent_MakeAvailable(CPSTL)
    if(NOT TARGET CPSTL::CPSTL)
        message(FATAL_ERROR "CPSTL sources do not define CPSTL::CPSTL.")
    endif()
    file(STRINGS "${cpstl_SOURCE_DIR}/library.properties"
        cpstl_version_line REGEX "^version=" LIMIT_COUNT 1)
    string(REGEX REPLACE "^version=" "" CPSTL_VERSION "${cpstl_version_line}")
    string(REGEX MATCH "^[0-9]+" cpstl_major "${CPSTL_VERSION}")
    string(REGEX MATCH "^[0-9]+" cpstl_required_major "${FOUNDATION_CPSTL_VERSION}")
    if(CPSTL_VERSION VERSION_LESS FOUNDATION_CPSTL_VERSION OR
       NOT cpstl_major STREQUAL cpstl_required_major)
        message(FATAL_ERROR
            "CPSTL sources at ${cpstl_SOURCE_DIR} are version '${CPSTL_VERSION}', "
            "but Foundation requires ${FOUNDATION_CPSTL_VERSION}+ "
            "(${cpstl_required_major}.x).")
    endif()
    if(FETCHCONTENT_SOURCE_DIR_CPSTL)
        set(FOUNDATION_CPSTL_SOURCE "sources at ${FETCHCONTENT_SOURCE_DIR_CPSTL}")
    else()
        set(FOUNDATION_CPSTL_SOURCE "sources at v${FOUNDATION_CPSTL_VERSION}")
    endif()
endif()

if(NOT TARGET CPSTL::CPSTL)
    message(FATAL_ERROR
        "Foundation requires CPSTL ${FOUNDATION_CPSTL_VERSION}+ (1.x). Set "
        "FOUNDATION_CPSTL_PREFIX to a CPSTL package, export "
        "../CPSTL/dist/${FOUNDATION_PLATFORM}, set "
        "FETCHCONTENT_SOURCE_DIR_CPSTL to a CPSTL working copy, or enable "
        "FOUNDATION_FETCH_CPSTL.")
endif()

message(STATUS "CPSTL       = ${CPSTL_VERSION} from ${FOUNDATION_CPSTL_SOURCE}")
