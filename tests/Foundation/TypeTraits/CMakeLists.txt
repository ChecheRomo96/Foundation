foundation_add_test(FoundationTypeTraitsTests
    TypeTraitsTests.cc
    TypeTraitsConformanceTests.cc
    FoundationUmbrellaTests.cc
    TopLevelTypeTraitsTests.cc
)

target_compile_definitions(FoundationTypeTraitsTests
    PRIVATE FOUNDATION_TEST_EXPECT_STD_TYPE_TRAITS=1
)

foundation_add_test(FoundationTypeTraitsFallbackTests
    TypeTraitsTests.cc
    TypeTraitsConformanceTests.cc
)

target_compile_definitions(FoundationTypeTraitsFallbackTests
    PRIVATE
        FOUNDATION_DETAIL_FORCE_CUSTOM_TYPE_TRAITS=1
        FOUNDATION_TEST_EXPECT_CUSTOM_TYPE_TRAITS=1
)
