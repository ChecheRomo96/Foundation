# Resolves the CPSTL source dependency and defines CPSTL::CPSTL.
#
# Desktop and embedded source builds use one CPSTL target owned by the top-level
# project. A parent may provide that target; otherwise Foundation fetches the
# pinned source revision. Exported Foundation packages retain their separate
# find_dependency(CPSTL) contract in FoundationConfig.cmake.in.

set(FOUNDATION_CPSTL_VERSION "0.0.1")
set(FOUNDATION_CPSTL_REPOSITORY "ChecheRomo96/CPSTL" CACHE STRING
    "GitHub repository (owner/name) used to fetch CPSTL")
option(FOUNDATION_FETCH_CPSTL
    "Fetch CPSTL source when a parent project does not provide it" ON)

set(FOUNDATION_CPSTL_SOURCE "")

if(TARGET CPSTL::CPSTL)
    set(FOUNDATION_CPSTL_SOURCE "parent project")
elseif(NOT FOUNDATION_FETCH_CPSTL)
    message(FATAL_ERROR
        "Foundation requires a parent-provided CPSTL::CPSTL target, or "
        "FOUNDATION_FETCH_CPSTL=ON.")
else()
    include(FetchContent)
    message(STATUS "Fetching CPSTL sources at v${FOUNDATION_CPSTL_VERSION}")
    FetchContent_Declare(CPSTL
        GIT_REPOSITORY "https://github.com/${FOUNDATION_CPSTL_REPOSITORY}.git"
        GIT_TAG "v${FOUNDATION_CPSTL_VERSION}"
        GIT_SHALLOW TRUE
        GIT_SUBMODULES "")
    FetchContent_MakeAvailable(CPSTL)

    if(NOT TARGET CPSTL::CPSTL)
        message(FATAL_ERROR "CPSTL sources do not define CPSTL::CPSTL.")
    endif()

    if(FETCHCONTENT_SOURCE_DIR_CPSTL)
        set(FOUNDATION_CPSTL_SOURCE
            "sources at ${FETCHCONTENT_SOURCE_DIR_CPSTL}")
    else()
        set(FOUNDATION_CPSTL_SOURCE
            "sources at v${FOUNDATION_CPSTL_VERSION}")
    endif()
endif()

if((NOT DEFINED CPSTL_VERSION OR CPSTL_VERSION STREQUAL "") AND
   DEFINED cpstl_SOURCE_DIR AND
   EXISTS "${cpstl_SOURCE_DIR}/library.properties")
    file(STRINGS "${cpstl_SOURCE_DIR}/library.properties"
        cpstl_version_line REGEX "^version=" LIMIT_COUNT 1)
    string(REGEX REPLACE "^version=" "" CPSTL_VERSION "${cpstl_version_line}")
endif()

if(DEFINED CPSTL_VERSION AND NOT CPSTL_VERSION STREQUAL "" AND
   CPSTL_VERSION VERSION_LESS FOUNDATION_CPSTL_VERSION)
    message(FATAL_ERROR
        "Foundation requires CPSTL ${FOUNDATION_CPSTL_VERSION}+; resolved "
        "${CPSTL_VERSION}.")
endif()

message(STATUS "CPSTL       = ${CPSTL_VERSION} from ${FOUNDATION_CPSTL_SOURCE}")
