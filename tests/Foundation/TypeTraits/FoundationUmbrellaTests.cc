#include <gtest/gtest.h>

#include <Foundation.h>

TEST(FoundationUmbrellaTypeTraitsTest, ExposesTypeTraits) {
    EXPECT_TRUE(Foundation::TypeTraits::is_integral<int>::value);
}
