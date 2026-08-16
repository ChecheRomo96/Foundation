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
        "${CMAKE_CURRENT_SOURCE_DIR}/Time.h"
    )
    
    #list(APPEND FOUNDATION_SOURCES_LOCAL
    #    "${CMAKE_CURRENT_SOURCE_DIR}/Time.cpp"
    #)
    
    list(APPEND FOUNDATION_DOX_LOCAL
        "${CMAKE_CURRENT_SOURCE_DIR}/Time.dox"
    )
#
######################################################################################################
# Append headers for enabled submodules

    if(FOUNDATION_TIME_CLOCK)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Clock.h"
        )
    endif()

    if(FOUNDATION_TIME_DURATION)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Duration.h"
        )
    endif()

    if(FOUNDATION_TIME_FREQUENCY)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Frequency.h"
        )
        add_subdirectory(Frequency)
    endif()

    if(FOUNDATION_TIME_PERIOD)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Period.h"
        )
        add_subdirectory(Period)
    endif()

    if(FOUNDATION_TIME_TICK)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Tick.h"
        )
    endif()

    if(FOUNDATION_TIME_TIME_POINT)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/TimePoint.h"
        )
    endif()

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
        DESTINATION "${FOUNDATION_DIST_INCLUDE_DIR}/Foundation/Time"
    )
#
######################################################################################################