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
find_package(Foundation 2.0 CONFIG REQUIRED
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

add_executable(FoundationStm32G0B1Consumer main.cpp)
target_compile_features(FoundationStm32G0B1Consumer PRIVATE cxx_std_17)
target_compile_options(FoundationStm32G0B1Consumer PRIVATE
    -Os -Wall -Wextra -Werror -ffreestanding
    -fno-unwind-tables -fno-asynchronous-unwind-tables)
target_link_libraries(FoundationStm32G0B1Consumer PRIVATE Foundation::Foundation)

# The consumer selects its memory budget; shared infrastructure owns CRT,
# linker generation and ELF/HEX/BIN/map mechanics.
set(ROMODULAR_STM32_HEAP_BYTES 4096 CACHE STRING "Validation firmware heap bytes")
include("${CMAKE_CURRENT_LIST_DIR}/../../tools/RoModularBuild/cmake/boards/STM32G0B1CBT6.cmake")
romodular_configure_stm32g0b1cbt6(FoundationStm32G0B1Consumer)
