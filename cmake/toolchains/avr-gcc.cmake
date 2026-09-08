# Generic AVR-GCC toolchain.
# Target profile files set FOUNDATION_AVR_MCU and FOUNDATION_AVR_ARCHITECTURE
# before including this file.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(FOUNDATION_AVR_TOOLCHAIN_ROOT "" CACHE PATH
    "Optional AVR-GCC installation root")
set(FOUNDATION_AVR_TOOLCHAIN_PREFIX "avr" CACHE STRING
    "AVR-GCC compiler prefix")
set(FOUNDATION_AVR_MCU "" CACHE STRING
    "AVR MCU name accepted by -mmcu")
set(FOUNDATION_AVR_ARCHITECTURE "avr" CACHE STRING
    "AVR architecture used as CMAKE_SYSTEM_PROCESSOR metadata")
set(FOUNDATION_AVR_ADDITIONAL_FLAGS "" CACHE STRING
    "Additional flags shared by C and C++")

if(FOUNDATION_AVR_MCU STREQUAL "")
    message(FATAL_ERROR
        "FOUNDATION_AVR_MCU must name the exact AVR target (for example atmega328p)"
    )
endif()

set(CMAKE_SYSTEM_PROCESSOR "${FOUNDATION_AVR_ARCHITECTURE}")

set(_foundation_avr_program_hints)
if(NOT FOUNDATION_AVR_TOOLCHAIN_ROOT STREQUAL "")
    list(APPEND _foundation_avr_program_hints
        "${FOUNDATION_AVR_TOOLCHAIN_ROOT}/bin"
    )
endif()

find_program(CMAKE_C_COMPILER
    NAMES "${FOUNDATION_AVR_TOOLCHAIN_PREFIX}-gcc"
    HINTS ${_foundation_avr_program_hints}
    REQUIRED
)
find_program(CMAKE_CXX_COMPILER
    NAMES "${FOUNDATION_AVR_TOOLCHAIN_PREFIX}-g++"
    HINTS ${_foundation_avr_program_hints}
    REQUIRED
)
find_program(CMAKE_AR
    NAMES "${FOUNDATION_AVR_TOOLCHAIN_PREFIX}-ar"
    HINTS ${_foundation_avr_program_hints}
    REQUIRED
)
find_program(CMAKE_RANLIB
    NAMES "${FOUNDATION_AVR_TOOLCHAIN_PREFIX}-ranlib"
    HINTS ${_foundation_avr_program_hints}
    REQUIRED
)

set(_foundation_avr_flags "-mmcu=${FOUNDATION_AVR_MCU}")
if(NOT FOUNDATION_AVR_ADDITIONAL_FLAGS STREQUAL "")
    string(APPEND _foundation_avr_flags
        " ${FOUNDATION_AVR_ADDITIONAL_FLAGS}"
    )
endif()

set(_foundation_avr_common_flags
    "${_foundation_avr_flags} -ffunction-sections -fdata-sections"
)

set(CMAKE_C_FLAGS_INIT "${_foundation_avr_common_flags}")
set(CMAKE_CXX_FLAGS_INIT
    "${_foundation_avr_common_flags} -fno-exceptions -fno-rtti"
)
set(CMAKE_EXE_LINKER_FLAGS_INIT
    "${_foundation_avr_flags} -Wl,--gc-sections"
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
