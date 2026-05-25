#ifndef FOUNDATION_TEST_MACROS_H
#define FOUNDATION_TEST_MACROS_H

#include <cstdlib>
#include <iostream>

#define FOUNDATION_EXPECT_TRUE(EXPR) do { if (!(EXPR)) { std::cerr << "FAILED: " #EXPR << std::endl; return 1; } } while (0)
#define FOUNDATION_EXPECT_FALSE(EXPR) FOUNDATION_EXPECT_TRUE(!(EXPR))
#define FOUNDATION_EXPECT_EQ(A, B) do { if (!((A) == (B))) { std::cerr << "FAILED: " #A " == " #B << std::endl; return 1; } } while (0)

#endif
