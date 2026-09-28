# Foundation 1.x compatibility wrapper for RoModularBuild.
# Target profiles continue to set the established FOUNDATION_* variables.

include("${CMAKE_CURRENT_LIST_DIR}/FoundationRoModularCompatibility.cmake")

foundation_forward_toolchain_cache(
    FOUNDATION_AVR_TOOLCHAIN_ROOT
    ROMODULAR_AVR_TOOLCHAIN_ROOT
    PATH
    ""
    "Optional AVR-GCC installation root"
)
foundation_forward_toolchain_cache(
    FOUNDATION_AVR_TOOLCHAIN_PREFIX
    ROMODULAR_AVR_TOOLCHAIN_PREFIX
    STRING
    "avr"
    "AVR-GCC compiler prefix"
)
foundation_forward_toolchain_cache(
    FOUNDATION_AVR_MCU
    ROMODULAR_AVR_MCU
    STRING
    ""
    "AVR MCU name accepted by -mmcu"
)
foundation_forward_toolchain_cache(
    FOUNDATION_AVR_ARCHITECTURE
    ROMODULAR_AVR_ARCHITECTURE
    STRING
    "avr"
    "AVR architecture used as CMAKE_SYSTEM_PROCESSOR metadata"
)
foundation_forward_toolchain_cache(
    FOUNDATION_AVR_ADDITIONAL_FLAGS
    ROMODULAR_AVR_ADDITIONAL_FLAGS
    STRING
    ""
    "Additional flags shared by C and C++"
)

set(FOUNDATION_ROMODULAR_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../../tools/RoModularBuild"
)
set(FOUNDATION_ROMODULAR_AVR_TOOLCHAIN
    "${FOUNDATION_ROMODULAR_ROOT}/cmake/toolchains/avr-gcc.cmake"
)
if(NOT EXISTS "${FOUNDATION_ROMODULAR_AVR_TOOLCHAIN}")
    message(FATAL_ERROR
        "RoModularBuild is not initialized; run "
        "'git submodule update --init --recursive'"
    )
endif()

include("${FOUNDATION_ROMODULAR_AVR_TOOLCHAIN}")
