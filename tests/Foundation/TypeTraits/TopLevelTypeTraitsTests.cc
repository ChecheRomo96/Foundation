#include <gtest/gtest.h>

#include <Foundation_TypeTraits.h>

#ifdef PROGMEM_MACRO
    #error "Foundation must not expose the legacy PROGMEM_MACRO"
#endif

static_assert(FOUNDATION_CPLUSPLUS >= 201703L);

TEST(FoundationTypeTraitsHeaderTest, ExposesTypeTraits) {
    EXPECT_TRUE(Foundation::TypeTraits::is_integral<int>::value);
    EXPECT_STRNE(FOUNDATION_VERSION, "");
}
