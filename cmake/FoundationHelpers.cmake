function(foundation_add_macro visibility)
    target_compile_definitions(Foundation ${visibility} ${ARGN})
    set_property(TARGET Foundation APPEND PROPERTY FOUNDATION_DOXYGEN_PREDEFS ${ARGN})
endfunction()

function(foundation_add_dox)
    set_property(TARGET Foundation APPEND PROPERTY FOUNDATION_DOXYGEN_INPUTS ${ARGN})
endfunction()

function(foundation_add_test test_target)
    add_executable(${test_target}
        ${ARGN}
    )

    target_link_libraries(${test_target}
        PRIVATE
            Foundation
            gtest
            gtest_main
    )

    target_include_directories(${test_target}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
    )

    gtest_discover_tests(${test_target})

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
    endforeach()
endfunction()