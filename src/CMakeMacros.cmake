######################################################################################################
# Add Compiler Definitions

target_compile_definitions(Foundation PUBLIC ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}")

# Add optional features as compile definitions and update the macros list

message(STATUS "Enabled ${PROJECT_NAME} features:")

include(${CMAKE_CURRENT_LIST_DIR}/Foundation/CMakeMacros.cmake)

# Other conditional flags can be added here as needed
######################################################################################################