cmake_minimum_required(VERSION 3.25)

project(FoundationAvrConsumer LANGUAGES CXX)

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "avr5")
    message(FATAL_ERROR
        "AvrConsumer requires the atmega328p AVR toolchain file"
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
find_package(Foundation 1.0 CONFIG REQUIRED
    PATHS "${FOUNDATION_PACKAGE_PREFIX}"
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH
)

if(NOT Foundation_PLATFORM STREQUAL "atmega328p_avrgcc_avr5")
    message(FATAL_ERROR
        "Expected the atmega328p_avrgcc_avr5 package, found ${Foundation_PLATFORM}"
    )
endif()

add_executable(FoundationAvrConsumer main.cpp)
set_target_properties(FoundationAvrConsumer PROPERTIES SUFFIX ".elf")
target_compile_features(FoundationAvrConsumer PRIVATE cxx_std_17)
target_compile_definitions(FoundationAvrConsumer PRIVATE F_CPU=16000000UL)
target_compile_options(FoundationAvrConsumer PRIVATE -Os -Wall -Wextra -Werror)
target_link_libraries(FoundationAvrConsumer PRIVATE Foundation::Foundation)

find_program(FOUNDATION_AVR_OBJCOPY avr-objcopy REQUIRED)
find_program(FOUNDATION_AVR_SIZE avr-size REQUIRED)

add_custom_command(TARGET FoundationAvrConsumer POST_BUILD
    COMMAND "${FOUNDATION_AVR_OBJCOPY}" -O ihex -R .eeprom
        "$<TARGET_FILE:FoundationAvrConsumer>"
        "$<TARGET_FILE_DIR:FoundationAvrConsumer>/FoundationAvrConsumer.hex"
    COMMAND "${FOUNDATION_AVR_SIZE}" --format=avr --mcu=atmega328p
        "$<TARGET_FILE:FoundationAvrConsumer>"
    VERBATIM
)
