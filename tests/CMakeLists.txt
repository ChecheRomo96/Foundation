set(FOUNDATION_DEPENDENCY_TEST_SCRIPT
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ValidateDependencies.cmake"
)
set(FOUNDATION_INVALID_DEPENDENCY_TEST_SCRIPT
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ExpectInvalidDependency.cmake"
)

function(foundation_add_invalid_dependency_test name feature dependency)
    add_test(
        NAME "FoundationConfiguration.${name}"
        COMMAND
            ${CMAKE_COMMAND}
            "-DFEATURE=${feature}"
            "-DDEPENDENCY=${dependency}"
            -P "${FOUNDATION_INVALID_DEPENDENCY_TEST_SCRIPT}"
    )
    set_tests_properties(
        "FoundationConfiguration.${name}"
        PROPERTIES
            LABELS "Foundation;Configuration"
    )
endfunction()

add_test(
    NAME FoundationConfiguration.ValidDefault
    COMMAND ${CMAKE_COMMAND} -P "${FOUNDATION_DEPENDENCY_TEST_SCRIPT}"
)
set_tests_properties(
    FoundationConfiguration.ValidDefault
    PROPERTIES LABELS "Foundation;Configuration"
)

add_test(
    NAME FoundationConfiguration.ValidWithoutContainersAndUtils
    COMMAND
        ${CMAKE_COMMAND}
        -DFOUNDATION_CONTAINERS=OFF
        -DFOUNDATION_UTILS=OFF
        -P "${FOUNDATION_DEPENDENCY_TEST_SCRIPT}"
)
set_tests_properties(
    FoundationConfiguration.ValidWithoutContainersAndUtils
    PROPERTIES LABELS "Foundation;Configuration"
)

add_test(
    NAME FoundationConfiguration.ValidWithoutTimeChain
    COMMAND
        ${CMAKE_COMMAND}
        -DFOUNDATION_TIME=OFF
        -DFOUNDATION_SCHEDULING=OFF
        -DFOUNDATION_MATH_RATIO=OFF
        -DFOUNDATION_FUNCTIONAL_CALLBACK=OFF
        -P "${FOUNDATION_DEPENDENCY_TEST_SCRIPT}"
)
set_tests_properties(
    FoundationConfiguration.ValidWithoutTimeChain
    PROPERTIES LABELS "Foundation;Configuration"
)

foundation_add_invalid_dependency_test(
    ContainersWithoutUtils
    FOUNDATION_CONTAINERS
    FOUNDATION_UTILS
)
foundation_add_invalid_dependency_test(
    TimeWithoutMath
    FOUNDATION_TIME
    FOUNDATION_MATH
)
foundation_add_invalid_dependency_test(
    TimeWithoutRatio
    FOUNDATION_TIME
    FOUNDATION_MATH_RATIO
)
foundation_add_invalid_dependency_test(
    TimeWithoutFunctional
    FOUNDATION_TIME
    FOUNDATION_FUNCTIONAL
)
foundation_add_invalid_dependency_test(
    TimeWithoutCallback
    FOUNDATION_TIME
    FOUNDATION_FUNCTIONAL_CALLBACK
)
foundation_add_invalid_dependency_test(
    SchedulingWithoutTime
    FOUNDATION_SCHEDULING
    FOUNDATION_TIME
)

add_subdirectory(Foundation)
