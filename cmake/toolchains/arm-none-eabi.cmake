# cmake/toolchains/arm-none-eabi.cmake

set(CMAKE_SYSTEM_NAME Generic)

if(NOT DEFINED FOUNDATION_ARM_CPU)
    set(FOUNDATION_ARM_CPU cortex-m3)
endif()

if(NOT DEFINED FOUNDATION_FLOAT_ABI)
    set(FOUNDATION_FLOAT_ABI soft)
endif()

set(CMAKE_SYSTEM_PROCESSOR ${FOUNDATION_ARM_CPU})

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(ARM_FLAGS "-mcpu=${FOUNDATION_ARM_CPU} -mthumb -mfloat-abi=${FOUNDATION_FLOAT_ABI}")

if(DEFINED FOUNDATION_FPU AND NOT FOUNDATION_FPU STREQUAL "")
    set(ARM_FLAGS "${ARM_FLAGS} -mfpu=${FOUNDATION_FPU}")
endif()

set(COMMON_FLAGS
    "${ARM_FLAGS} -ffunction-sections -fdata-sections"
)

set(CMAKE_C_FLAGS_INIT
    "${COMMON_FLAGS}"
)

set(CMAKE_CXX_FLAGS_INIT
    "${COMMON_FLAGS} -fno-exceptions -fno-rtti"
)

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "${ARM_FLAGS} -Wl,--gc-sections"
)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)