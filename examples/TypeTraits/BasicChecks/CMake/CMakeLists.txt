add_executable(Foundation_TypeTraits_BasicChecks
    BasicChecks.cpp
)

target_link_libraries(Foundation_TypeTraits_BasicChecks
    PRIVATE
        Foundation::Foundation
)
