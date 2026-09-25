#####################################################################################################
# Setting up the project

    #################################################################################################
    # CMake Minimum Version

        cmake_minimum_required(VERSION 3.25)
    #
    #################################################################################################
    # Project Name, Version, and Language

        project(Foundation VERSION 1.0.0 LANGUAGES C CXX)

        set(FOUNDATION_REQUIRED_CXX_STANDARD 17)

        file(STRINGS
            "${CMAKE_CURRENT_LIST_DIR}/library.properties"
            FOUNDATION_ARDUINO_VERSION_LINE
            REGEX "^version="
            LIMIT_COUNT 1
        )
        string(REGEX REPLACE "^version=" "" FOUNDATION_ARDUINO_VERSION
            "${FOUNDATION_ARDUINO_VERSION_LINE}"
        )
        if(NOT FOUNDATION_ARDUINO_VERSION STREQUAL PROJECT_VERSION)
            message(FATAL_ERROR
                "Version mismatch: CMake declares ${PROJECT_VERSION}, but "
                "library.properties declares ${FOUNDATION_ARDUINO_VERSION}"
            )
        endif()

        file(STRINGS
            "${CMAKE_CURRENT_LIST_DIR}/src/Foundation_BuildSettings.h"
            FOUNDATION_SOURCE_VERSION_LINE
            REGEX "^[ \t]*#define[ \t]+FOUNDATION_VERSION[ \t]+\"[^\"]+\""
            LIMIT_COUNT 1
        )
        if(NOT FOUNDATION_SOURCE_VERSION_LINE MATCHES "\"([^\"]+)\"")
            message(FATAL_ERROR
                "Foundation_BuildSettings.h does not declare FOUNDATION_VERSION"
            )
        endif()
        set(FOUNDATION_SOURCE_VERSION "${CMAKE_MATCH_1}")
        if(NOT FOUNDATION_SOURCE_VERSION STREQUAL PROJECT_VERSION)
            message(FATAL_ERROR
                "Version mismatch: CMake declares ${PROJECT_VERSION}, but "
                "Foundation_BuildSettings.h declares ${FOUNDATION_SOURCE_VERSION}"
            )
        endif()

        message(STATUS "Building ${PROJECT_NAME} Version = ${PROJECT_VERSION}")
        message(STATUS "Required C++ standard = C++${FOUNDATION_REQUIRED_CXX_STANDARD}")
        message(STATUS "System      = ${CMAKE_SYSTEM_NAME}")
        message(STATUS "Processor   = ${CMAKE_SYSTEM_PROCESSOR}")
        message(STATUS "C Compiler  = ${CMAKE_C_COMPILER}")
        message(STATUS "CXX Compiler= ${CMAKE_CXX_COMPILER}")
        message(STATUS "CXX ID      = ${CMAKE_CXX_COMPILER_ID}")

        set(FOUNDATION_EXPECTED_SYSTEM_PROCESSOR "" CACHE STRING
            "Regular expression for the expected target processor")
        if(FOUNDATION_EXPECTED_SYSTEM_PROCESSOR AND
           NOT CMAKE_SYSTEM_PROCESSOR MATCHES "${FOUNDATION_EXPECTED_SYSTEM_PROCESSOR}")
            message(FATAL_ERROR
                "Preset '${FOUNDATION_PLATFORM}' expects processor "
                "'${FOUNDATION_EXPECTED_SYSTEM_PROCESSOR}', but CMake detected "
                "'${CMAKE_SYSTEM_PROCESSOR}'"
            )
        endif()
    #
    #################################################################################################
    # Set up C++ Version

        set(CMAKE_CXX_STANDARD ${FOUNDATION_REQUIRED_CXX_STANDARD})
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)
    #
    #################################################################################################
    # Set output directory to the build folder's bin directory

        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    #
    #################################################################################################
#
#####################################################################################################
# Store this directory as a variable called FOUNDATION_ROOT_DIRECTORY to reference in deeper directories

    set(FOUNDATION_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
#
#####################################################################################################
# Include helper functions for Doxygen and macro management

    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)
    include(cmake/FoundationHelpers.cmake)
#
#####################################################################################################
# Compile Configuration

    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeOptions.cmake")
        include(${CMAKE_CURRENT_LIST_DIR}/CMakeOptions.cmake)
    else()
        message(WARNING "CMakeOptions.cmake not found! Using default configuration.")
    endif()

    include(cmake/FoundationDependencies.cmake)
    foundation_validate_dependencies()
#
######################################################################################################
# Append Source

    # Apply coverage flags only if FOUNDATION_COVERAGE is ON
    if(FOUNDATION_COVERAGE)
        message(STATUS "Enabling Code Coverage")

        set(COVERAGE_FLAGS "--coverage -O0 -g")

        string(APPEND CMAKE_C_FLAGS_DEBUG " ${COVERAGE_FLAGS}")
        string(APPEND CMAKE_CXX_FLAGS_DEBUG " ${COVERAGE_FLAGS}")
        string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " ${COVERAGE_FLAGS}")
    endif()

    # Add the src directory to the build
    add_subdirectory(src)
#
######################################################################################################
# Create the library

    foundation_add_macro(PUBLIC FOUNDATION_VERSION="${PROJECT_VERSION}")

#
######################################################################################################
# Append Examples

    if(FOUNDATION_EXAMPLES)
        message(STATUS "Building FOUNDATION Examples")
        add_subdirectory(examples)
    endif()
#
######################################################################################################
# Export Build Target

    # Build exactly the artifacts that can be installed by the export workflow.
    # This avoids building tests or documentation from a reused cache while still
    # including examples whenever FOUNDATION_EXAMPLES is enabled.
    add_custom_target(FoundationExportArtifacts)
    add_dependencies(FoundationExportArtifacts Foundation)
    if(TARGET FoundationExamples)
        add_dependencies(FoundationExportArtifacts FoundationExamples)
    endif()
#
######################################################################################################
# Enable Doxygen to build the documentation

    if(FOUNDATION_DOCS)
        message(STATUS "Doxygen documentation generation enabled")


        foundation_add_dox(
            ${CMAKE_CURRENT_SOURCE_DIR}/README.md
            ${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md
        )

        include(cmake/BuildDocs.cmake)
    endif()
#
######################################################################################################
# Enable Doxygen to build the documentation

    if(FOUNDATION_TESTING)
        message(STATUS "Building FOUNDATION Tests")
        include(cmake/BuildTests.cmake)
        add_subdirectory(tests)
    endif()
#
######################################################################################################
#
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FoundationConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Foundation"
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FoundationBuildInfo.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationBuildInfo.cmake"
        @ONLY
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationConfigVersion.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationBuildInfo.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Foundation"
        CONFIGURATIONS Release
    )
#
######################################################################################################
