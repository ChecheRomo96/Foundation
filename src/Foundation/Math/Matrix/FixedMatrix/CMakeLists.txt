######################################################################################################
# Initialize local lists

    set(FOUNDATION_SOURCES_LOCAL "")
    set(FOUNDATION_PRIVATE_HEADERS_LOCAL "")
    set(FOUNDATION_PUBLIC_HEADERS_LOCAL "")
    set(FOUNDATION_DOX_LOCAL "")

#
######################################################################################################
# Append headers

    list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
        "${CMAKE_CURRENT_SOURCE_DIR}/FixedMatrix.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/FixedMatrix.tpp"
    )

    list(APPEND FOUNDATION_DOX_LOCAL
        "${CMAKE_CURRENT_SOURCE_DIR}/FixedMatrix.dox"
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
# Add sources for Doxygen

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
# Foundation install process

    install(
        FILES ${FOUNDATION_PUBLIC_HEADERS_LOCAL}
        DESTINATION "${FOUNDATION_DIST_INCLUDE_DIR}/Foundation/Math/Matrix/FixedMatrix"
    )

#
######################################################################################################