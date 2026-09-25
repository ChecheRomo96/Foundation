if(FOUNDATION_FUNCTIONAL_CALLBACK)
    set(FOUNDATION_FUNCTIONAL_TEST_SOURCES
        FunctionalTests.cc
        FoundationUmbrellaTests.cc
        TopLevelFunctionalTests.cc
    )

    if(FOUNDATION_UTILS)
        list(APPEND FOUNDATION_FUNCTIONAL_TEST_SOURCES
            FunctionalThenUtilsTests.cc
            UtilsThenFunctionalTests.cc
        )
    endif()

    foundation_add_test(
        FoundationFunctionalTests
        ${FOUNDATION_FUNCTIONAL_TEST_SOURCES}
    )
endif()
