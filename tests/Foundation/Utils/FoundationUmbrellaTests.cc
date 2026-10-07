#include <gtest/gtest.h>

#include <stdint.h>

#include <Foundation.h>

TEST(FoundationUmbrellaUtilsTest, ExposesUtils) {
    const uint8_t source[2] = {1, 2};
    uint8_t copy[2] = {0, 0};
    Foundation::Utils::Flash::Copy(copy, source, sizeof copy);

    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
}
