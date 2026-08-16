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
        "${CMAKE_CURRENT_LIST_DIR}/Functional.h"
    )
    
    list(APPEND FOUNDATION_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Functional.dox"
    )

#
######################################################################################################
# Add sources to the FOUNDATION target

    if(FOUNDATION_FUNCTIONAL_CALLBACK)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Callback.h"
        )
        add_subdirectory(Callback)
    endif()
#
######################################################################################################
# Add sources to the FOUNDATION target

    target_sources(Foundation PRIVATE
        ${FOUNDATION_PUBLIC_HEADERS_LOCAL}
        ${FOUNDATION_PRIVATE_HEADERS_LOCAL}
        ${FOUNDATION_SOURCES_LOCAL}
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
# FOUNDATION Install Process

    # Install headers for this subdirectory
    install(
        FILES ${FOUNDATION_PUBLIC_HEADERS_LOCAL}
        DESTINATION "${FOUNDATION_DIST_INCLUDE_DIR}/Foundation/Functional"
    )
#
######################################################################################################