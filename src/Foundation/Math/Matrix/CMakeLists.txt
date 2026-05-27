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
        "${CMAKE_CURRENT_SOURCE_DIR}/Matrix.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/MatrixOperations.h"
    )

    list(APPEND FOUNDATION_DOX_LOCAL
        "${CMAKE_CURRENT_SOURCE_DIR}/Matrix.dox"
    )

#
######################################################################################################
# Add sources to the Foundation target

    add_subdirectory(FixedMatrix)
    add_subdirectory(DynamicMatrix)

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
        DESTINATION "include/Foundation-${FOUNDATION_VERSION}/Foundation/Math/Matrix"
    )

#
######################################################################################################