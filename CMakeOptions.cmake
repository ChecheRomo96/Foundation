#################################################################################################################################
# Output Targets Configuration

    option(FOUNDATION_EXAMPLES "Enable building examples" OFF)
    option(FOUNDATION_TESTING "Enable unit testing" OFF)
    option(FOUNDATION_DOCS "Generate API documentation using Doxygen" OFF)
    option(FOUNDATION_FULL_BUILD "Toggle on all modules" OFF)
    option(FOUNDATION_TESTING "Enable unit testing" OFF)
    option(FOUNDATION_COVERAGE "Enable coverage testing" OFF)
    
    option(FOUNDATION_DOCS "Build documentation" OFF)

#
#################################################################################################################################
# src CMake Options

    include(src/CMakeOptions.cmake)
#
#################################################################################################################################