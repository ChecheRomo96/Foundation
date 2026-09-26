function(foundation_add_macro visibility)
    target_compile_definitions(Foundation ${visibility} ${ARGN})
    set_property(TARGET Foundation APPEND PROPERTY FOUNDATION_DOXYGEN_PREDEFS ${ARGN})
endfunction()

function(foundation_add_dox)
    set_property(TARGET Foundation APPEND PROPERTY FOUNDATION_DOXYGEN_INPUTS ${ARGN})
endfunction()

# Agreed warning set for Foundation's own targets when testing. Warnings are
# errors; pass --compile-no-warning-as-error to cmake to relax locally.
function(foundation_enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion
            -Wold-style-cast -Wnon-virtual-dtor -Woverloaded-virtual
        )
    endif()
    set_target_properties(${target} PROPERTIES COMPILE_WARNING_AS_ERROR ON)
endfunction()

function(foundation_add_test test_target)
    add_executable(${test_target}
        ${ARGN}
    )

    target_link_libraries(${test_target}
        PRIVATE
            Foundation::Foundation
            GTest::gtest_main
    )
    foundation_enable_warnings(${test_target})

    gtest_discover_tests(${test_target}
        TEST_PREFIX "${test_target}."
        DISCOVERY_MODE PRE_TEST
        PROPERTIES LABELS "Foundation"
    )

    set_property(TARGET Foundation APPEND PROPERTY FOUNDATION_TEST_TARGETS ${test_target})
endfunction()

function(foundation_stage_headers)
    foreach(HEADER ${ARGV})
        file(RELATIVE_PATH REL_HEADER "${FOUNDATION_SRC_DIRECTORY}" "${HEADER}")
        get_filename_component(REL_DIR "${REL_HEADER}" DIRECTORY)
        file(MAKE_DIRECTORY "${FOUNDATION_BUILD_INCLUDE_DIR}/${REL_DIR}")

        configure_file(
            "${HEADER}"
            "${FOUNDATION_BUILD_INCLUDE_DIR}/${REL_HEADER}"
            COPYONLY
        )
        set_property(GLOBAL APPEND PROPERTY FOUNDATION_STAGED_HEADERS "${REL_HEADER}")
    endforeach()
endfunction()
