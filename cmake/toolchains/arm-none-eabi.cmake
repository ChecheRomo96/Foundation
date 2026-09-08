# Generic GNU Arm Embedded toolchain.
# Target profile files set FOUNDATION_ARM_CPU, FOUNDATION_FLOAT_ABI, and
# optionally FOUNDATION_FPU before including this file.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(FOUNDATION_ARM_TOOLCHAIN_ROOT "" CACHE PATH
    "Optional GNU Arm Embedded installation root")
set(FOUNDATION_ARM_TOOLCHAIN_PREFIX "arm-none-eabi" CACHE STRING
    "GNU Arm Embedded compiler prefix")
set(FOUNDATION_ARM_CPU "cortex-m3" CACHE STRING "Target Arm CPU")
set(FOUNDATION_FLOAT_ABI "soft" CACHE STRING "Target floating-point ABI")
set(FOUNDATION_FPU "" CACHE STRING "Target FPU name")
set(FOUNDATION_ARM_ADDITIONAL_FLAGS "" CACHE STRING
    "Additional flags shared by C and C++")
set(FOUNDATION_ARM_SYSROOT "" CACHE PATH
    "Optional target sysroot containing the C runtime headers and libraries")

set(CMAKE_SYSTEM_PROCESSOR "${FOUNDATION_ARM_CPU}")

if(NOT FOUNDATION_ARM_SYSROOT STREQUAL "")
    set(CMAKE_SYSROOT "${FOUNDATION_ARM_SYSROOT}")
endif()

set(_foundation_arm_program_hints)
if(NOT FOUNDATION_ARM_TOOLCHAIN_ROOT STREQUAL "")
    list(APPEND _foundation_arm_program_hints
        "${FOUNDATION_ARM_TOOLCHAIN_ROOT}/bin"
    )
endif()

find_program(CMAKE_C_COMPILER
    NAMES "${FOUNDATION_ARM_TOOLCHAIN_PREFIX}-gcc"
    HINTS ${_foundation_arm_program_hints}
    REQUIRED
)
find_program(CMAKE_CXX_COMPILER
    NAMES "${FOUNDATION_ARM_TOOLCHAIN_PREFIX}-g++"
    HINTS ${_foundation_arm_program_hints}
    REQUIRED
)
find_program(CMAKE_AR
    NAMES "${FOUNDATION_ARM_TOOLCHAIN_PREFIX}-ar"
    HINTS ${_foundation_arm_program_hints}
    REQUIRED
)
find_program(CMAKE_RANLIB
    NAMES "${FOUNDATION_ARM_TOOLCHAIN_PREFIX}-ranlib"
    HINTS ${_foundation_arm_program_hints}
    REQUIRED
)

if(FOUNDATION_FLOAT_ABI STREQUAL "hard" AND FOUNDATION_FPU STREQUAL "")
    message(FATAL_ERROR "FOUNDATION_FPU is required when FOUNDATION_FLOAT_ABI=hard")
endif()

set(_foundation_arm_flags
    "-mcpu=${FOUNDATION_ARM_CPU} -mthumb -mfloat-abi=${FOUNDATION_FLOAT_ABI}"
)
if(NOT FOUNDATION_FPU STREQUAL "")
    string(APPEND _foundation_arm_flags " -mfpu=${FOUNDATION_FPU}")
endif()
if(NOT FOUNDATION_ARM_ADDITIONAL_FLAGS STREQUAL "")
    string(APPEND _foundation_arm_flags " ${FOUNDATION_ARM_ADDITIONAL_FLAGS}")
endif()

set(_foundation_common_flags
    "${_foundation_arm_flags} -ffunction-sections -fdata-sections"
)

set(CMAKE_C_FLAGS_INIT "${_foundation_common_flags}")
set(CMAKE_CXX_FLAGS_INIT
    "${_foundation_common_flags} -fno-exceptions -fno-rtti"
)
set(CMAKE_EXE_LINKER_FLAGS_INIT
    "${_foundation_arm_flags} -Wl,--gc-sections"
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
