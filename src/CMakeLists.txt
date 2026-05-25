######################################################################################################
# Store this directory as a variable called FOUNDATION_SRC_DIRECTORY to reference in deeper directories

    set(FOUNDATION_SRC_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

    add_library(Foundation STATIC)

    if(NOT FOUNDATION_PLATFORM)
        set(FOUNDATION_PLATFORM "desktop")
    endif()

    set(FOUNDATION_OUTPUT_DIRECTORY
        "${FOUNDATION_ROOT_DIRECTORY}/dist/${FOUNDATION_PLATFORM}"
    )

    set_target_properties(Foundation PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${FOUNDATION_OUTPUT_DIRECTORY}/lib"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${FOUNDATION_OUTPUT_DIRECTORY}/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${FOUNDATION_OUTPUT_DIRECTORY}/lib"

        LIBRARY_OUTPUT_DIRECTORY "${FOUNDATION_OUTPUT_DIRECTORY}/lib"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${FOUNDATION_OUTPUT_DIRECTORY}/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${FOUNDATION_OUTPUT_DIRECTORY}/lib"

        RUNTIME_OUTPUT_DIRECTORY "${FOUNDATION_OUTPUT_DIRECTORY}/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${FOUNDATION_OUTPUT_DIRECTORY}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${FOUNDATION_OUTPUT_DIRECTORY}/bin"

        OUTPUT_NAME_DEBUG "Foundation_d"
        OUTPUT_NAME_RELEASE "Foundation"
    )

    set(FOUNDATION_BUILD_INCLUDE_DIR
        "${CMAKE_BINARY_DIR}/include/FOUNDATION-${FOUNDATION_VERSION}"
    )
#
######################################################################################################
# Initialize FOUNDATION_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(FOUNDATION_SOURCES_LOCAL "")
    set(FOUNDATION_PRIVATE_HEADERS_LOCAL "")
    set(FOUNDATION_PUBLIC_HEADERS_LOCAL "")
    set(FOUNDATION_DOX_LOCAL "")
#
######################################################################################################
# Append Headers (local to this subdirectory)

    list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Foundation.h"
        "${CMAKE_CURRENT_LIST_DIR}/Foundation_BuildSettings.h"
    )

    list(APPEND FOUNDATION_SOURCES_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Foundation.cpp"
    )
    
    list(APPEND FOUNDATION_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Foundation.dox"
    )

#
######################################################################################################
# Add sources to the Foundation target

    target_sources(Foundation PRIVATE
        ${FOUNDATION_PUBLIC_HEADERS_LOCAL}
        ${FOUNDATION_PRIVATE_HEADERS_LOCAL}
        ${FOUNDATION_SOURCES_LOCAL}
    )
#
######################################################################################################
# add subdirectories
    
    if(FOUNDATION_MATH)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Foundation_Math.h"
        )
    endif()
    
    if(FOUNDATION_TIME)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Foundation_Time.h"
        )
    endif()
    
    if(FOUNDATION_UTILS)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Foundation_Utils.h"
        )
    endif()

    add_subdirectory(Foundation)
#
######################################################################################################
# Add Compiler Definitions

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeMacros.cmake")
        include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeMacros.cmake")
    else()
        message(WARNING "src/CMakeMacros.cmake not found! Impossible to add compiler macro definitions to targets.")
    endif()
# 
######################################################################################################
# Include directories for build and install

    target_include_directories(Foundation
        PUBLIC
            "$<BUILD_INTERFACE:${FOUNDATION_BUILD_INCLUDE_DIR}>"
            "$<INSTALL_INTERFACE:include/FOUNDATION-${PROJECT_VERSION}>"
    )
#
######################################################################################################
# Add sources for doxygen

    if(FOUNDATION_DOCS)
        foundation_add_dox(
            ${FOUNDATION_PUBLIC_HEADERS_LOCAL}
            ${FOUNDATION_DOX_LOCAL}
        )
    endif()
#
######################################################################################################
# Stage headers

    foundation_stage_headers(${FOUNDATION_PUBLIC_HEADERS_LOCAL})
#
######################################################################################################
# FOUNDATION install process

    install(TARGETS Foundation
    EXPORT FoundationTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/Foundation-${FOUNDATION_VERSION}
)

    install(
        DIRECTORY "${FOUNDATION_BUILD_INCLUDE_DIR}/"
        DESTINATION "include/Foundation-${FOUNDATION_VERSION}"
    )

    install(EXPORT FoundationTargets
        FILE FoundationTargets.cmake
        NAMESPACE Foundation::
        DESTINATION lib/cmake/Foundation
    )
#
######################################################################################################