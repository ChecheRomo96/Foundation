######################################################################################################
# Initialize FOUNDATION_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(FOUNDATION_SOURCES_LOCAL "")
    set(FOUNDATION_PRIVATE_HEADERS_LOCAL "")
    set(FOUNDATION_PUBLIC_HEADERS_LOCAL "")
    set(FOUNDATION_DOX_LOCAL "")
#
######################################################################################################
# Append Headers (local to this subdirectory)

    list(APPEND FOUNDATION_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Foundation.dox"
    )

    if(FOUNDATION_MATH)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Math.h"
        )
        add_subdirectory(Math)
    endif()

    if(FOUNDATION_TYPE_TRAITS)
        add_subdirectory(TypeTraits)
    endif()

    if(FOUNDATION_CONTAINERS)
        add_subdirectory(Containers)
    endif()

    if(FOUNDATION_FUNCTIONAL)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Functional.h"
        )
        add_subdirectory(Functional)
    endif()
    
    if(FOUNDATION_TIME)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Time.h"
        )
        add_subdirectory(Time)
    endif()

    if(FOUNDATION_SCHEDULING)
        add_subdirectory(Scheduling)
    endif()
    
    if(FOUNDATION_UTILS)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Utils.h"
        )
        add_subdirectory(Utils)
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
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Foundation"
    )
#
######################################################################################################
