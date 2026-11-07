cmake_minimum_required(VERSION 3.25)

project(FoundationSourceConsumer LANGUAGES CXX)

if(NOT FOUNDATION_SOURCE_DIR)
    message(FATAL_ERROR "FOUNDATION_SOURCE_DIR must identify a Foundation checkout")
endif()

get_filename_component(FOUNDATION_SOURCE_DIR "${FOUNDATION_SOURCE_DIR}" ABSOLUTE)

include(FetchContent)
FetchContent_Declare(Foundation
    SOURCE_DIR "${FOUNDATION_SOURCE_DIR}"
)
FetchContent_MakeAvailable(Foundation)

if(NOT TARGET Foundation::Foundation)
    message(FATAL_ERROR "FetchContent did not define Foundation::Foundation")
endif()

add_executable(FoundationSourceConsumer main.cpp)
target_compile_features(FoundationSourceConsumer PRIVATE cxx_std_17)
target_link_libraries(FoundationSourceConsumer PRIVATE Foundation::Foundation)

enable_testing()
add_test(
    NAME FoundationSourceConsumer.Run
    COMMAND FoundationSourceConsumer
)
