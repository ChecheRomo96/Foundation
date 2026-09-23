#include <gtest/gtest.h>

#include <Foundation/Math/Matrix.h>

namespace Matrix = Foundation::Math::Matrix;

TEST(MatrixTest, MultipliesFixedMatrices) {
    const int leftValues[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };
    const int rightValues[3][2] = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    const Matrix::Fixed<int, 2, 3> left(leftValues);
    const Matrix::Fixed<int, 3, 2> right(rightValues);
    Matrix::Fixed<int, 2, 2> result;

    Matrix::Multiply(left, right, result);

    EXPECT_EQ(result.At(0, 0), 58);
    EXPECT_EQ(result.At(0, 1), 64);
    EXPECT_EQ(result.At(1, 0), 139);
    EXPECT_EQ(result.At(1, 1), 154);
}

TEST(MatrixTest, MultipliesDynamicMatrices) {
    Matrix::Dynamic<int> left(2, 3);
    Matrix::Dynamic<int> right(3, 2);
    Matrix::Dynamic<int> result;

    int value = 1;
    for(unsigned int row = 0; row < left.RowsCount(); ++row) {
        for(unsigned int col = 0; col < left.ColsCount(); ++col) {
            left.At(row, col) = value++;
        }
    }

    const int rightValues[3][2] = {
        {7, 8},
        {9, 10},
        {11, 12}
    };
    for(unsigned int row = 0; row < right.RowsCount(); ++row) {
        for(unsigned int col = 0; col < right.ColsCount(); ++col) {
            right.At(row, col) = rightValues[row][col];
        }
    }

    ASSERT_TRUE(Matrix::Multiply(left, right, result));
    EXPECT_EQ(result.RowsCount(), 2u);
    EXPECT_EQ(result.ColsCount(), 2u);
    EXPECT_EQ(result.At(0, 0), 58);
    EXPECT_EQ(result.At(0, 1), 64);
    EXPECT_EQ(result.At(1, 0), 139);
    EXPECT_EQ(result.At(1, 1), 154);
}

TEST(MatrixTest, DeducesFixedByDynamicMultiplication) {
    const int leftValues[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };
    const Matrix::Fixed<int, 2, 3> left(leftValues);
    Matrix::Dynamic<int> right(3, 1);
    right.At(0, 0) = 7;
    right.At(1, 0) = 8;
    right.At(2, 0) = 9;
    Matrix::Dynamic<int> result;

    ASSERT_TRUE(Matrix::Multiply(left, right, result));
    EXPECT_EQ(result.RowsCount(), 2u);
    EXPECT_EQ(result.ColsCount(), 1u);
    EXPECT_EQ(result.At(0, 0), 50);
    EXPECT_EQ(result.At(1, 0), 122);
}

TEST(MatrixTest, MultipliesDynamicByFixed) {
    Matrix::Dynamic<int> left(1, 2);
    left.At(0, 0) = 2;
    left.At(0, 1) = 3;
    const int rightValues[2][2] = {
        {4, 5},
        {6, 7}
    };
    const Matrix::Fixed<int, 2, 2> right(rightValues);
    Matrix::Dynamic<int> result;

    ASSERT_TRUE(Matrix::Multiply(left, right, result));
    EXPECT_EQ(result.At(0, 0), 26);
    EXPECT_EQ(result.At(0, 1), 31);
}

TEST(MatrixTest, RejectsIncompatibleDynamicDimensions) {
    Matrix::Dynamic<int> left(2, 3);
    Matrix::Dynamic<int> right(2, 2);
    Matrix::Dynamic<int> result;

    EXPECT_FALSE(Matrix::Multiply(left, right, result));
    EXPECT_FALSE(result.IsValid());
}
