# Foundation 1.x compatibility wrapper for RoModularBuild.
# Target profiles continue to set the established FOUNDATION_* variables.

include("${CMAKE_CURRENT_LIST_DIR}/FoundationRoModularCompatibility.cmake")

foundation_forward_toolchain_cache(
    FOUNDATION_ARM_TOOLCHAIN_ROOT
    ROMODULAR_ARM_TOOLCHAIN_ROOT
    PATH
    ""
    "Optional GNU Arm Embedded installation root"
)
foundation_forward_toolchain_cache(
    FOUNDATION_ARM_TOOLCHAIN_PREFIX
    ROMODULAR_ARM_TOOLCHAIN_PREFIX
    STRING
    "arm-none-eabi"
    "GNU Arm Embedded compiler prefix"
)
foundation_forward_toolchain_cache(
    FOUNDATION_ARM_CPU
    ROMODULAR_ARM_CPU
    STRING
    "cortex-m3"
    "Target Arm CPU"
)
foundation_forward_toolchain_cache(
    FOUNDATION_FLOAT_ABI
    ROMODULAR_FLOAT_ABI
    STRING
    "soft"
    "Target floating-point ABI"
)
foundation_forward_toolchain_cache(
    FOUNDATION_FPU
    ROMODULAR_FPU
    STRING
    ""
    "Target FPU name"
)
foundation_forward_toolchain_cache(
    FOUNDATION_ARM_ADDITIONAL_FLAGS
    ROMODULAR_ARM_ADDITIONAL_FLAGS
    STRING
    ""
    "Additional flags shared by C and C++"
)
foundation_forward_toolchain_cache(
    FOUNDATION_ARM_SYSROOT
    ROMODULAR_ARM_SYSROOT
    PATH
    ""
    "Optional target sysroot containing the C runtime headers and libraries"
)

set(FOUNDATION_ROMODULAR_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../../tools/RoModularBuild"
)
set(FOUNDATION_ROMODULAR_ARM_TOOLCHAIN
    "${FOUNDATION_ROMODULAR_ROOT}/cmake/toolchains/arm-none-eabi.cmake"
)
if(NOT EXISTS "${FOUNDATION_ROMODULAR_ARM_TOOLCHAIN}")
    message(FATAL_ERROR
        "RoModularBuild is not initialized; run "
        "'git submodule update --init --recursive'"
    )
endif()

include("${FOUNDATION_ROMODULAR_ARM_TOOLCHAIN}")
