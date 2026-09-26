#include <gtest/gtest.h>

#include <Foundation/Math/Matrix.h>

namespace Matrix = Foundation::Math::Matrix;

TEST(FixedMatrixTest, InitializesDimensionsAndMutableStorage) {
    Matrix::Fixed<int, 2, 3> matrix;

    EXPECT_EQ(matrix.RowCount(), 2u);
    EXPECT_EQ(matrix.ColumnCount(), 3u);
    for(unsigned int index = 0; index < 6; ++index) {
        EXPECT_EQ(matrix.Data()[index], 0);
    }

    matrix.Data()[4] = 9;
    matrix[0][2] = 7;
    EXPECT_EQ(matrix.At(1, 1), 9);
    EXPECT_EQ(matrix.At(0, 2), 7);

    const Matrix::Fixed<int, 2, 3>& constant = matrix;
    EXPECT_EQ(constant.Data()[4], 9);
    EXPECT_EQ(constant[0][2], 7);
    EXPECT_EQ(constant.At(1, 1), 9);
}

TEST(FixedMatrixTest, CopiesAndAssignsWithoutSharingState) {
    const int values[2][2] = {
        {1, 2},
        {3, 4}
    };
    Matrix::Fixed<int, 2, 2> original(values);
    Matrix::Fixed<int, 2, 2> copy(original);
    Matrix::Fixed<int, 2, 2> assigned;
    assigned = original;

    original.At(0, 0) = 99;

    EXPECT_EQ(copy.At(0, 0), 1);
    EXPECT_EQ(assigned.At(0, 0), 1);
    EXPECT_EQ(copy, assigned);
}

TEST(FixedMatrixTest, AppliesElementWiseArithmetic) {
    const int leftValues[2][2] = {
        {1, 2},
        {3, 4}
    };
    const int rightValues[2][2] = {
        {5, 6},
        {7, 8}
    };
    const Matrix::Fixed<int, 2, 2> left(leftValues);
    const Matrix::Fixed<int, 2, 2> right(rightValues);

    const Matrix::Fixed<int, 2, 2> sum = left + right;
    const Matrix::Fixed<int, 2, 2> difference = right - left;
    EXPECT_EQ(sum.At(0, 0), 6);
    EXPECT_EQ(sum.At(1, 1), 12);
    EXPECT_EQ(difference.At(0, 1), 4);
    EXPECT_EQ(difference.At(1, 0), 4);

    Matrix::Fixed<int, 2, 2> accumulated(left);
    EXPECT_EQ(&(accumulated += right), &accumulated);
    EXPECT_EQ(accumulated, sum);
    EXPECT_EQ(&(accumulated -= right), &accumulated);
    EXPECT_EQ(accumulated, left);
}

TEST(FixedMatrixTest, AppliesScalarArithmeticFromEitherSide) {
    const int values[2][2] = {
        {2, 4},
        {6, 8}
    };
    const Matrix::Fixed<int, 2, 2> matrix(values);

    const Matrix::Fixed<int, 2, 2> doubled = matrix * 2;
    const Matrix::Fixed<int, 2, 2> tripled = 3 * matrix;
    const Matrix::Fixed<int, 2, 2> halved = matrix / 2;
    EXPECT_EQ(doubled.At(1, 1), 16);
    EXPECT_EQ(tripled.At(1, 0), 18);
    EXPECT_EQ(halved.At(0, 1), 2);

    Matrix::Fixed<int, 2, 2> changed(matrix);
    EXPECT_EQ(&(changed *= 3), &changed);
    EXPECT_EQ(changed, tripled);
    EXPECT_EQ(&(changed /= 3), &changed);
    EXPECT_EQ(changed, matrix);
}

TEST(FixedMatrixTest, ComparesFillsZerosAndUsesFactories) {
    using Matrix2x2 = Matrix::Fixed<int, 2, 2>;

    Matrix2x2 matrix(7);
    EXPECT_EQ(matrix, Matrix2x2::Filled(7));
    EXPECT_NE(matrix, Matrix2x2::Zeros());

    matrix.Fill(-3);
    EXPECT_EQ(matrix.At(0, 0), -3);
    EXPECT_EQ(matrix.At(1, 1), -3);

    matrix.Zero();
    EXPECT_EQ(matrix, Matrix2x2::Zeros());
}

TEST(FixedMatrixTest, ReturnsATransposedCopy) {
    const int values[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };
    const Matrix::Fixed<int, 2, 3> matrix(values);
    const Matrix::Fixed<int, 3, 2> transposed = matrix.Transposed();

    EXPECT_EQ(transposed.RowCount(), 3u);
    EXPECT_EQ(transposed.ColumnCount(), 2u);
    EXPECT_EQ(transposed.At(0, 0), 1);
    EXPECT_EQ(transposed.At(0, 1), 4);
    EXPECT_EQ(transposed.At(2, 0), 3);
    EXPECT_EQ(transposed.At(2, 1), 6);
}

TEST(DynamicMatrixTest, InitializesAndExposesMutableAndConstantRows) {
    Matrix::Dynamic<int> matrix(2, 3, 5);

    ASSERT_TRUE(matrix.IsValid());
    EXPECT_EQ(matrix.RowCount(), 2u);
    EXPECT_EQ(matrix.ColumnCount(), 3u);
    EXPECT_EQ(matrix.Size(), 6u);
    matrix.Data()[4] = 9;
    matrix[0][2] = 7;

    const Matrix::Dynamic<int>& constant = matrix;
    EXPECT_EQ(constant.Data()[4], 9);
    EXPECT_EQ(constant[0][2], 7);
    EXPECT_EQ(constant.At(1, 1), 9);
}

TEST(DynamicMatrixTest, AppliesElementWiseArithmetic) {
    Matrix::Dynamic<int> left(2, 2);
    Matrix::Dynamic<int> right(2, 2);
    for(unsigned int index = 0; index < 4; ++index) {
        left.Data()[index] = static_cast<int>(index + 1);
        right.Data()[index] = static_cast<int>(index + 5);
    }

    const Matrix::Dynamic<int> sum = left + right;
    const Matrix::Dynamic<int> difference = right - left;
    ASSERT_TRUE(sum.IsValid());
    ASSERT_TRUE(difference.IsValid());
    EXPECT_EQ(sum.At(0, 0), 6);
    EXPECT_EQ(sum.At(1, 1), 12);
    EXPECT_EQ(difference.At(0, 1), 4);
    EXPECT_EQ(difference.At(1, 0), 4);

    Matrix::Dynamic<int> accumulated(left);
    EXPECT_EQ(&(accumulated += right), &accumulated);
    EXPECT_EQ(accumulated, sum);
    EXPECT_EQ(&(accumulated -= right), &accumulated);
    EXPECT_EQ(accumulated, left);
}

TEST(DynamicMatrixTest, RejectsIncompatibleElementWiseArithmetic) {
    Matrix::Dynamic<int> matrix(2, 2, 7);
    const Matrix::Dynamic<int> incompatible(1, 2, 3);
    const Matrix::Dynamic<int> empty;

    EXPECT_FALSE((matrix + incompatible).IsValid());
    EXPECT_FALSE((matrix - incompatible).IsValid());
    EXPECT_FALSE((matrix + empty).IsValid());

    const Matrix::Dynamic<int> original(matrix);
    matrix += incompatible;
    EXPECT_EQ(matrix, original);
    matrix -= empty;
    EXPECT_EQ(matrix, original);
}

TEST(DynamicMatrixTest, AppliesScalarArithmeticFromEitherSide) {
    Matrix::Dynamic<int> matrix(2, 2);
    matrix.At(0, 0) = 2;
    matrix.At(0, 1) = 4;
    matrix.At(1, 0) = 6;
    matrix.At(1, 1) = 8;

    const Matrix::Dynamic<int> doubled = matrix * 2;
    const Matrix::Dynamic<int> tripled = 3 * matrix;
    const Matrix::Dynamic<int> halved = matrix / 2;
    ASSERT_TRUE(doubled.IsValid());
    ASSERT_TRUE(tripled.IsValid());
    ASSERT_TRUE(halved.IsValid());
    EXPECT_EQ(doubled.At(1, 1), 16);
    EXPECT_EQ(tripled.At(1, 0), 18);
    EXPECT_EQ(halved.At(0, 1), 2);

    Matrix::Dynamic<int> changed(matrix);
    EXPECT_EQ(&(changed *= 3), &changed);
    EXPECT_EQ(changed, tripled);
    EXPECT_EQ(&(changed /= 3), &changed);
    EXPECT_EQ(changed, matrix);

    const Matrix::Dynamic<int> empty;
    EXPECT_FALSE((empty * 2).IsValid());
    EXPECT_FALSE((empty / 2).IsValid());
}

TEST(DynamicMatrixTest, ComparesFillsAndZerosValues) {
    Matrix::Dynamic<int> first(2, 2, 7);
    Matrix::Dynamic<int> second(2, 2, 7);
    const Matrix::Dynamic<int> differentShape(1, 4, 7);

    EXPECT_EQ(first, second);
    EXPECT_NE(first, differentShape);
    second.At(1, 1) = 8;
    EXPECT_NE(first, second);

    first.Fill(-3);
    EXPECT_EQ(first.At(0, 0), -3);
    EXPECT_EQ(first.At(1, 1), -3);
    first.Zero();
    EXPECT_EQ(first.At(0, 0), 0);
    EXPECT_EQ(first.At(1, 1), 0);

    EXPECT_EQ(Matrix::Dynamic<int>(), Matrix::Dynamic<int>());
}

TEST(DynamicMatrixTest, ReturnsAnOwningTransposedCopy) {
    Matrix::Dynamic<int> matrix(2, 3);
    for(unsigned int index = 0; index < matrix.Size(); ++index) {
        matrix.Data()[index] = static_cast<int>(index + 1);
    }

    const Matrix::Dynamic<int> transposed = matrix.Transposed();
    ASSERT_TRUE(transposed.IsValid());
    EXPECT_TRUE(transposed.OwnsData());
    EXPECT_EQ(transposed.RowCount(), 3u);
    EXPECT_EQ(transposed.ColumnCount(), 2u);
    EXPECT_EQ(transposed.At(0, 0), 1);
    EXPECT_EQ(transposed.At(0, 1), 4);
    EXPECT_EQ(transposed.At(2, 0), 3);
    EXPECT_EQ(transposed.At(2, 1), 6);

    const Matrix::Dynamic<int> empty;
    EXPECT_FALSE(empty.Transposed().IsValid());
}
