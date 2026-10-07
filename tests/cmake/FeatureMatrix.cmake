# Configures, builds, and tests the real project for every supported feature
# selection, and checks that unsupported selections fail at configure time with
# the precise dependency error.
#
# cmake -DPRESET=<configure preset> [-DWORK_DIR=<dir>] [-DCONFIG=Debug]
#       [-DONLY=<case regex>] -P tests/cmake/FeatureMatrix.cmake

if(NOT DEFINED PRESET)
    message(FATAL_ERROR "PRESET must be provided.")
endif()

get_filename_component(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
if(NOT DEFINED WORK_DIR)
    set(WORK_DIR "${SOURCE_DIR}/build/feature-matrix/${PRESET}")
endif()
if(NOT DEFINED CONFIG)
    set(CONFIG Debug)
endif()

set(MODULES
    FOUNDATION_MATH
    FOUNDATION_CONTAINERS
    FOUNDATION_SCHEDULING
    FOUNDATION_TIME
    FOUNDATION_UTILS
    FOUNDATION_FUNCTIONAL
)
list(TRANSFORM MODULES APPEND "=OFF" OUTPUT_VARIABLE CORE_ONLY)
list(TRANSFORM CORE_ONLY PREPEND "-D")

set(PASSED 0)

# Forward how CPSTL is resolved (-DFETCHCONTENT_SOURCE_DIR_CPSTL=<dir> or
# -DFOUNDATION_CPSTL_PREFIX=<dir>) to every case.
set(CPSTL_ARGS "")
foreach(var FETCHCONTENT_SOURCE_DIR_CPSTL FOUNDATION_CPSTL_PREFIX
        FETCHCONTENT_SOURCE_DIR_GOOGLETEST)
    if(DEFINED ${var})
        list(APPEND CPSTL_ARGS "-D${var}=${${var}}")
    endif()
endforeach()

function(configure_case name)
    set(build_dir "${WORK_DIR}/${name}")
    file(REMOVE_RECURSE "${build_dir}")
    execute_process(
        COMMAND "${CMAKE_COMMAND}" --preset "${PRESET}" -S "${SOURCE_DIR}"
            -B "${build_dir}"
            -DFOUNDATION_TESTING=ON
            # One GoogleTest and CPSTL download shared by every case.
            "-DFETCHCONTENT_BASE_DIR=${WORK_DIR}/_deps"
            ${CPSTL_ARGS}
            ${ARGN}
        WORKING_DIRECTORY "${SOURCE_DIR}"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE output
    )
    set(CONFIGURE_RESULT "${result}" PARENT_SCOPE)
    set(CONFIGURE_OUTPUT "${output}" PARENT_SCOPE)
endfunction()

function(run_or_fail name)
    execute_process(COMMAND ${ARGN} RESULT_VARIABLE result)
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "[${name}] failed: ${ARGN}")
    endif()
endfunction()

# A selection that must configure, build, and pass its tests.
function(expect_pass name)
    if(DEFINED ONLY AND NOT name MATCHES "${ONLY}")
        return()
    endif()
    message(STATUS "[${name}] ${ARGN}")
    configure_case(${name} ${ARGN})
    if(NOT CONFIGURE_RESULT EQUAL 0)
        message(FATAL_ERROR "[${name}] configure failed:\n${CONFIGURE_OUTPUT}")
    endif()
    set(build_dir "${WORK_DIR}/${name}")
    run_or_fail(${name} "${CMAKE_COMMAND}" --build "${build_dir}"
        --config ${CONFIG} --parallel)
    file(MAKE_DIRECTORY "${build_dir}/test-results")
    run_or_fail(${name} "${CMAKE_CTEST_COMMAND}" --test-dir "${build_dir}"
        -C ${CONFIG} --output-on-failure --no-tests=error
        --output-junit "${build_dir}/test-results/${name}.xml")
    file(REMOVE_RECURSE "${build_dir}")
    math(EXPR count "${PASSED} + 1")
    set(PASSED ${count} PARENT_SCOPE)
endfunction()

# A selection that must be rejected at configure time with this message.
function(expect_reject name feature dependency)
    if(DEFINED ONLY AND NOT name MATCHES "${ONLY}")
        return()
    endif()
    message(STATUS "[${name}] -D${dependency}=OFF")
    configure_case(${name} "-D${dependency}=OFF")
    set(expected "${feature}=ON requires ${dependency}=ON")
    if(CONFIGURE_RESULT EQUAL 0)
        message(FATAL_ERROR "[${name}] configured, expected: ${expected}")
    endif()
    string(FIND "${CONFIGURE_OUTPUT}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR
            "[${name}] rejected without '${expected}':\n${CONFIGURE_OUTPUT}")
    endif()
    file(REMOVE_RECURSE "${WORK_DIR}/${name}")
    math(EXPR count "${PASSED} + 1")
    set(PASSED ${count} PARENT_SCOPE)
endfunction()

expect_pass(Default)
expect_pass(Full -DFOUNDATION_FULL_BUILD=ON)
expect_pass(CoreOnly ${CORE_ONLY})

# Each top-level module disabled together with the modules that need it.
expect_pass(WithoutMath
    -DFOUNDATION_MATH=OFF -DFOUNDATION_TIME=OFF -DFOUNDATION_SCHEDULING=OFF)
expect_pass(WithoutContainers -DFOUNDATION_CONTAINERS=OFF)
expect_pass(WithoutScheduling -DFOUNDATION_SCHEDULING=OFF)
expect_pass(WithoutTime -DFOUNDATION_TIME=OFF -DFOUNDATION_SCHEDULING=OFF)
expect_pass(WithoutUtils -DFOUNDATION_UTILS=OFF)
expect_pass(WithoutFunctional
    -DFOUNDATION_FUNCTIONAL=OFF -DFOUNDATION_TIME=OFF -DFOUNDATION_SCHEDULING=OFF)

# Optional submodules.
expect_pass(WithoutComplex -DFOUNDATION_MATH_COMPLEX=OFF)
expect_pass(WithoutMatrix -DFOUNDATION_MATH_MATRIX=OFF)
expect_pass(WithoutRatio
    -DFOUNDATION_MATH_RATIO=OFF -DFOUNDATION_TIME=OFF -DFOUNDATION_SCHEDULING=OFF)
expect_pass(WithoutCallback
    -DFOUNDATION_FUNCTIONAL_CALLBACK=OFF -DFOUNDATION_TIME=OFF
    -DFOUNDATION_SCHEDULING=OFF)

# A module disabled alone while another enabled module still needs it.
expect_reject(RejectMath FOUNDATION_TIME FOUNDATION_MATH)
expect_reject(RejectTime FOUNDATION_SCHEDULING FOUNDATION_TIME)
expect_reject(RejectFunctional FOUNDATION_TIME FOUNDATION_FUNCTIONAL)
expect_reject(RejectRatio FOUNDATION_TIME FOUNDATION_MATH_RATIO)
expect_reject(RejectCallback FOUNDATION_TIME FOUNDATION_FUNCTIONAL_CALLBACK)

message(STATUS "Feature matrix: ${PASSED} case(s) passed for ${PRESET}.")
