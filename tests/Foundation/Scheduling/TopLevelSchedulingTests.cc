#include <gtest/gtest.h>

#include <Foundation_Scheduling.h>

TEST(FoundationSchedulingHeaderTest, ExposesScheduling) {
    Foundation::Scheduling::Task* task = nullptr;

    EXPECT_EQ(task, nullptr);
}
