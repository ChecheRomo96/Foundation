#include <gtest/gtest.h>

#include <Foundation_TypeTraits.h>

TEST(FoundationTypeTraitsHeaderTest, ExposesTypeTraits) {
    EXPECT_TRUE(Foundation::TypeTraits::is_integral<int>::value);
}
