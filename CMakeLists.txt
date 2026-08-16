#####################################################################################################
# Setting up the project

    #################################################################################################
    # CMake Minimum Version
        
        cmake_minimum_required(VERSION 3.25)
    #
    #################################################################################################
    # Project Name, Version, and Language

        project(Foundation VERSION 1.0.0 LANGUAGES C CXX)
        message(STATUS "Building ${PROJECT_NAME} Version = ${PROJECT_VERSION}")
        message(STATUS "System      = ${CMAKE_SYSTEM_NAME}")
        message(STATUS "Processor   = ${CMAKE_SYSTEM_PROCESSOR}")
        message(STATUS "C Compiler  = ${CMAKE_C_COMPILER}")
        message(STATUS "CXX Compiler= ${CMAKE_CXX_COMPILER}")
        message(STATUS "CXX ID      = ${CMAKE_CXX_COMPILER_ID}")
    #
    #################################################################################################
    # Set up C++ Version

        set(CMAKE_CXX_STANDARD 17) 
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
    include(cmake/FoundationHelpers.cmake)
#
#####################################################################################################
# Compile Configuration
    
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeOptions.cmake")
        include(${CMAKE_CURRENT_LIST_DIR}/CMakeOptions.cmake)
    else()
        message(WARNING "CMakeOptions.cmake not found! Using default configuration.")
    endif()
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

    foundation_add_macro(PUBLIC Foundation_VERSION="${PROJECT_VERSION}")

#
######################################################################################################
# Append Examples

    if(FOUNDATION_EXAMPLES)
        message(STATUS "Building FOUNDATION Examples")
        add_subdirectory(examples)
    endif()
#
######################################################################################################
# Enable Doxygen to build the documentation

    if(FOUNDATION_DOCS)
        message(STATUS "Doxygen documentation generation enabled")

        
        foundation_add_dox(
            ${CMAKE_CURRENT_SOURCE_DIR}/README.md
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
        INSTALL_DESTINATION lib/cmake/Foundation
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/FoundationConfigVersion.cmake"
        DESTINATION lib/cmake/Foundation
    )
#
######################################################################################################
