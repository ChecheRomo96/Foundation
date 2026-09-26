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
        -DFOUNDATION_TYPE_TRAITS=OFF
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
    TimeWithoutTypeTraits
    FOUNDATION_TIME
    FOUNDATION_TYPE_TRAITS
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

# Every staged public header must compile alone in an otherwise empty
# translation unit. Including it twice also checks the include guard.
# Template implementation files (.tpp) are reachable only through their .h.
get_property(FOUNDATION_STAGED_HEADERS GLOBAL PROPERTY FOUNDATION_STAGED_HEADERS)
list(FILTER FOUNDATION_STAGED_HEADERS INCLUDE REGEX "\\.h$")
set(FOUNDATION_HEADER_CHECK_SOURCES "")
foreach(header IN LISTS FOUNDATION_STAGED_HEADERS)
    string(REPLACE "/" "-" header_id "${header}")
    set(source "${CMAKE_CURRENT_BINARY_DIR}/HeaderSelfContainment/${header_id}.cc")
    file(CONFIGURE
        OUTPUT "${source}"
        CONTENT "#include <${header}>\n#include <${header}>\n"
    )
    list(APPEND FOUNDATION_HEADER_CHECK_SOURCES "${source}")
endforeach()

add_library(FoundationHeaderSelfContainment OBJECT ${FOUNDATION_HEADER_CHECK_SOURCES})
target_link_libraries(FoundationHeaderSelfContainment PRIVATE Foundation::Foundation)
foundation_enable_warnings(FoundationHeaderSelfContainment)
foundation_enable_warnings(Foundation)

add_test(
    NAME FoundationHeaders.SelfContained
    COMMAND
        ${CMAKE_COMMAND}
        --build "${CMAKE_BINARY_DIR}"
        --config $<CONFIG>
        --target FoundationHeaderSelfContainment
)
set_tests_properties(
    FoundationHeaders.SelfContained
    PROPERTIES LABELS "Foundation;Headers"
)

add_subdirectory(Foundation)
