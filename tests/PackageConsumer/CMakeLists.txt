cmake_minimum_required(VERSION 3.25)

project(FoundationPackageConsumer LANGUAGES CXX)

if(CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR
        "PackageConsumer executes its binary and therefore requires a native "
        "preset; validate cross-compiled packages in target firmware"
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

find_package(Foundation 1.0 CONFIG REQUIRED
    PATHS "${FOUNDATION_PACKAGE_PREFIX}"
    NO_DEFAULT_PATH
)

if(NOT TARGET Foundation::Foundation)
    message(FATAL_ERROR
        "The exported package did not define Foundation::Foundation"
    )
endif()

add_executable(FoundationPackageConsumer
    main.cpp
    SelectiveHeaders.cpp
)

target_compile_features(FoundationPackageConsumer PRIVATE cxx_std_17)
target_link_libraries(FoundationPackageConsumer PRIVATE Foundation::Foundation)

enable_testing()
add_test(
    NAME FoundationPackageConsumer.Run
    COMMAND FoundationPackageConsumer
)

message(STATUS "Foundation package prefix = ${FOUNDATION_PACKAGE_PREFIX}")
message(STATUS "Foundation package config = ${Foundation_DIR}")
message(STATUS "Foundation package version = ${Foundation_VERSION}")
