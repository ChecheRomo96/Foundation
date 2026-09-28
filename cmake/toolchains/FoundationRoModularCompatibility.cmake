# Temporary compatibility adapter for the RoModularBuild migration.
# Foundation keeps its public cache variables during the 1.x line while the
# shared implementation uses project-independent ROMODULAR_* names.

function(foundation_forward_toolchain_cache
    FOUNDATION_NAME
    ROMODULAR_NAME
    CACHE_TYPE
    DEFAULT_VALUE
    DESCRIPTION
)
    if(NOT DEFINED ${FOUNDATION_NAME})
        if(DEFINED ${ROMODULAR_NAME})
            set(${FOUNDATION_NAME} "${${ROMODULAR_NAME}}" CACHE ${CACHE_TYPE}
                "${DESCRIPTION}")
        else()
            set(${FOUNDATION_NAME} "${DEFAULT_VALUE}" CACHE ${CACHE_TYPE}
                "${DESCRIPTION}")
        endif()
    endif()

    if(DEFINED ${ROMODULAR_NAME} AND
       NOT "${${ROMODULAR_NAME}}" STREQUAL "${${FOUNDATION_NAME}}")
        message(WARNING
            "Both ${FOUNDATION_NAME} and ${ROMODULAR_NAME} are set; "
            "Foundation 1.x compatibility gives ${FOUNDATION_NAME} precedence"
        )
    endif()

    set(${ROMODULAR_NAME} "${${FOUNDATION_NAME}}" CACHE ${CACHE_TYPE}
        "${DESCRIPTION}" FORCE)
endfunction()
