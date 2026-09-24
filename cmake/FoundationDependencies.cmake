function(foundation_require_feature feature dependency)
    if(${feature} AND NOT ${dependency})
        message(FATAL_ERROR
            "Invalid Foundation configuration:\n"
            "  ${feature}=ON requires ${dependency}=ON.\n"
            "Enable it with -D${dependency}=ON, or disable ${feature} "
            "with -D${feature}=OFF."
        )
    endif()
endfunction()

function(foundation_validate_dependencies)
    foundation_require_feature(
        FOUNDATION_CONTAINERS
        FOUNDATION_UTILS
    )

    foundation_require_feature(
        FOUNDATION_TIME
        FOUNDATION_MATH
    )
    foundation_require_feature(
        FOUNDATION_TIME
        FOUNDATION_TYPE_TRAITS
    )
    foundation_require_feature(
        FOUNDATION_TIME
        FOUNDATION_MATH_RATIO
    )
    foundation_require_feature(
        FOUNDATION_TIME
        FOUNDATION_FUNCTIONAL
    )
    foundation_require_feature(
        FOUNDATION_TIME
        FOUNDATION_FUNCTIONAL_CALLBACK
    )

    foundation_require_feature(
        FOUNDATION_SCHEDULING
        FOUNDATION_TIME
    )
endfunction()
