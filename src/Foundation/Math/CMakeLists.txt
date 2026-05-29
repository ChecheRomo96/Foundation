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
        "${CMAKE_CURRENT_SOURCE_DIR}/Math.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/Arithmetic.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/Trigonometry.h"
    )
    
    list(APPEND FOUNDATION_SOURCES_LOCAL
        "${CMAKE_CURRENT_SOURCE_DIR}/Arithmetic.cpp"
    )

    list(APPEND FOUNDATION_DOX_LOCAL
        "${CMAKE_CURRENT_SOURCE_DIR}/Math.dox"
    )

#
######################################################################################################
# Append headers
    
    if(FOUNDATION_MATH_RATIO)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Ratio.h"
        )
    endif()
    
    if(FOUNDATION_MATH_COMPLEX)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Complex.h"
        )
        add_subdirectory(Complex)
    endif()
    
    if(FOUNDATION_MATH_MATRIX)
        list(APPEND FOUNDATION_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_SOURCE_DIR}/Matrix.h"
        )
        add_subdirectory(Matrix)
    endif()

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
        DESTINATION "include/Foundation-${FOUNDATION_VERSION}/Foundation/Math"
    )

#
######################################################################################################