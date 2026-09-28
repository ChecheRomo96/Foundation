cmake_minimum_required(VERSION 3.25)

project(FoundationStm32G0B1Consumer LANGUAGES CXX ASM)

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m0plus")
    message(FATAL_ERROR
        "Stm32G0B1Consumer requires the Cortex-M0+ Arm toolchain file"
    )
endif()

if(NOT FOUNDATION_PACKAGE_PREFIX)
    message(FATAL_ERROR
        "FOUNDATION_PACKAGE_PREFIX must name an exported Foundation prefix"
    )
endif()

get_filename_component(
    FOUNDATION_PACKAGE_PREFIX
    "${FOUNDATION_PACKAGE_PREFIX}"
    ABSOLUTE
)

# The cross toolchain restricts package searches to the target root, so the
# host-side package prefix is exempted explicitly.
find_package(Foundation 1.2 CONFIG REQUIRED
    PATHS "${FOUNDATION_PACKAGE_PREFIX}"
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH
)

if(NOT Foundation_PLATFORM STREQUAL
   "stm32g0b1cbt6_armgcc_cortex_m0plus_soft")
    message(FATAL_ERROR
        "Expected the STM32G0B1CBT6 package, found ${Foundation_PLATFORM}"
    )
endif()

add_executable(FoundationStm32G0B1Consumer
    main.cpp
    startup_stm32g0b1.S
)

set_target_properties(FoundationStm32G0B1Consumer PROPERTIES
    SUFFIX ".elf"
)

target_compile_features(FoundationStm32G0B1Consumer PRIVATE cxx_std_17)
target_compile_options(FoundationStm32G0B1Consumer PRIVATE
    -Os
    -Wall
    -Wextra
    -Werror
    -ffreestanding
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
)
target_link_options(FoundationStm32G0B1Consumer PRIVATE
    -nostdlib
    -nostartfiles
    "-T${CMAKE_CURRENT_LIST_DIR}/stm32g0b1cbt6.ld"
    "-Wl,-Map,$<TARGET_FILE_DIR:FoundationStm32G0B1Consumer>/FoundationStm32G0B1Consumer.map"
    -Wl,--gc-sections
    -Wl,--no-warn-rwx-segments
    -Wl,--no-warn-execstack
    -Wl,--print-memory-usage
)
target_link_libraries(FoundationStm32G0B1Consumer PRIVATE
    Foundation::Foundation
    c
    gcc
)

get_filename_component(
    FOUNDATION_ARM_COMPILER_DIRECTORY
    "${CMAKE_CXX_COMPILER}"
    DIRECTORY
)
find_program(FOUNDATION_ARM_OBJCOPY
    arm-none-eabi-objcopy
    HINTS "${FOUNDATION_ARM_COMPILER_DIRECTORY}"
    NO_DEFAULT_PATH
    REQUIRED
)
find_program(FOUNDATION_ARM_SIZE
    arm-none-eabi-size
    HINTS "${FOUNDATION_ARM_COMPILER_DIRECTORY}"
    NO_DEFAULT_PATH
    REQUIRED
)

add_custom_command(TARGET FoundationStm32G0B1Consumer POST_BUILD
    COMMAND "${FOUNDATION_ARM_OBJCOPY}" -O ihex
        "$<TARGET_FILE:FoundationStm32G0B1Consumer>"
        "$<TARGET_FILE_DIR:FoundationStm32G0B1Consumer>/FoundationStm32G0B1Consumer.hex"
    COMMAND "${FOUNDATION_ARM_OBJCOPY}" -O binary
        "$<TARGET_FILE:FoundationStm32G0B1Consumer>"
        "$<TARGET_FILE_DIR:FoundationStm32G0B1Consumer>/FoundationStm32G0B1Consumer.bin"
    COMMAND "${FOUNDATION_ARM_SIZE}"
        "$<TARGET_FILE:FoundationStm32G0B1Consumer>"
    VERBATIM
)
