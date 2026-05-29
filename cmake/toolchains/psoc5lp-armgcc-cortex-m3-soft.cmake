# cmake/toolchains/psoc5lp-armgcc-cortex-m3-soft.cmake

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m3)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(FOUNDATION_TARGET_PSoC5LP ON)
set(FOUNDATION_TARGET_CORTEX_M3 ON)
set(FOUNDATION_TARGET_ARMGCC ON)

set(COMMON_FLAGS
    "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft -ffunction-sections -fdata-sections"
)

set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -fno-exceptions -fno-rtti")

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft -Wl,--gc-sections"
)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)