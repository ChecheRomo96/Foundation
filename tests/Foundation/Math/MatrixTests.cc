#include <gtest/gtest.h>

#include <cstdlib>
#include <new>
#include <type_traits>

#include <Foundation/Math/Matrix.h>

namespace Matrix = Foundation::Math::Matrix;

namespace {

    struct ControlledAllocationElement {
        int Value = 0;

        static bool FailAllocation;

        static void* operator new[](
            std::size_t size,
            const std::nothrow_t&
        ) noexcept {
            return FailAllocation ? nullptr : std::malloc(size);
        }

        static void operator delete[](void* pointer) noexcept {
            std::free(pointer);
        }

        static void operator delete[](
            void* pointer,
            const std::nothrow_t&
        ) noexcept {
            std::free(pointer);
        }
    };

    bool ControlledAllocationElement::FailAllocation = false;

}

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

TEST(MatrixTest, AllocatesOwnedDynamicStorage) {
    Matrix::Dynamic<int> matrix;

    ASSERT_TRUE(matrix.Allocate(2, 3));
    EXPECT_TRUE(matrix.IsValid());
    EXPECT_TRUE(matrix.OwnsData());
    EXPECT_EQ(matrix.RowsCount(), 2u);
    EXPECT_EQ(matrix.ColsCount(), 3u);
    EXPECT_EQ(matrix.Size(), 6u);
}

TEST(MatrixTest, ReusesCompatibleDynamicStorage) {
    Matrix::Dynamic<int> matrix(2, 2, 7);
    int* const originalStorage = matrix.Data();

    ASSERT_TRUE(matrix.Allocate(2, 2));
    EXPECT_EQ(matrix.Data(), originalStorage);
    EXPECT_EQ(matrix.At(0, 0), 7);
}

TEST(MatrixTest, PreservesDynamicStorageWhenDimensionsAreInvalid) {
    Matrix::Dynamic<int> matrix(1, 1, 42);
    int* const originalStorage = matrix.Data();
    const unsigned int maximum = static_cast<unsigned int>(-1);

    EXPECT_FALSE(matrix.Allocate(0, 1));
    EXPECT_FALSE(matrix.Allocate(maximum, 2));
    EXPECT_EQ(matrix.Data(), originalStorage);
    EXPECT_EQ(matrix.RowsCount(), 1u);
    EXPECT_EQ(matrix.ColsCount(), 1u);
    EXPECT_EQ(matrix.At(0, 0), 42);
}

TEST(MatrixTest, ReportsAllocationFailureWithoutChangingMatrix) {
    ControlledAllocationElement::FailAllocation = false;
    Matrix::Dynamic<ControlledAllocationElement> matrix(1, 1);
    ASSERT_TRUE(matrix.IsValid());
    ControlledAllocationElement* const originalStorage = matrix.Data();
    matrix.At(0, 0).Value = 42;

    ControlledAllocationElement::FailAllocation = true;
    EXPECT_FALSE(matrix.Allocate(2, 2));
    EXPECT_EQ(matrix.Data(), originalStorage);
    EXPECT_EQ(matrix.RowsCount(), 1u);
    EXPECT_EQ(matrix.ColsCount(), 1u);
    EXPECT_EQ(matrix.At(0, 0).Value, 42);
    ControlledAllocationElement::FailAllocation = false;
}

TEST(MatrixTest, CopyAssignmentPreservesDestinationOnAllocationFailure) {
    ControlledAllocationElement::FailAllocation = false;
    Matrix::Dynamic<ControlledAllocationElement> source(2, 2);
    Matrix::Dynamic<ControlledAllocationElement> destination(1, 1);
    ASSERT_TRUE(source.IsValid());
    ASSERT_TRUE(destination.IsValid());
    ControlledAllocationElement* const originalStorage = destination.Data();
    destination.At(0, 0).Value = 42;

    ControlledAllocationElement::FailAllocation = true;
    destination = source;
    EXPECT_EQ(destination.Data(), originalStorage);
    EXPECT_EQ(destination.RowsCount(), 1u);
    EXPECT_EQ(destination.ColsCount(), 1u);
    EXPECT_EQ(destination.At(0, 0).Value, 42);
    ControlledAllocationElement::FailAllocation = false;
}

TEST(MatrixTest, AttachesCallerOwnedStorage) {
    int storage[6] = {1, 2, 3, 4, 5, 6};
    Matrix::Dynamic<int> matrix;

    ASSERT_TRUE(matrix.Attach(2, 3, storage));
    EXPECT_TRUE(matrix.IsValid());
    EXPECT_FALSE(matrix.OwnsData());
    EXPECT_EQ(matrix.Data(), storage);

    matrix.At(1, 2) = 60;
    EXPECT_EQ(storage[5], 60);

    matrix.Clear();
    EXPECT_FALSE(matrix.IsValid());
    EXPECT_EQ(storage[5], 60);
}

TEST(MatrixTest, RejectsInvalidAttachmentWithoutChangingMatrix) {
    int storage[4] = {1, 2, 3, 4};
    Matrix::Dynamic<int> matrix(1, 1, 9);
    int* const originalStorage = matrix.Data();

    EXPECT_FALSE(matrix.Attach(2, 2, nullptr));
    EXPECT_FALSE(matrix.Attach(0, 2, storage));
    EXPECT_EQ(matrix.Data(), originalStorage);
    EXPECT_TRUE(matrix.OwnsData());
    EXPECT_EQ(matrix.At(0, 0), 9);
}

TEST(MatrixTest, RejectsAttachingItsOwnAllocation) {
    Matrix::Dynamic<int> matrix(2, 2, 5);
    int* const ownedStorage = matrix.Data();

    EXPECT_FALSE(matrix.Attach(2, 2, ownedStorage));
    EXPECT_TRUE(matrix.OwnsData());
    EXPECT_EQ(matrix.Data(), ownedStorage);
    EXPECT_EQ(matrix.At(1, 1), 5);
}

TEST(MatrixTest, DeepCopiesAttachedStorageIntoOwnedStorage) {
    int storage[4] = {1, 2, 3, 4};
    Matrix::Dynamic<int> attached(2, 2, storage);
    Matrix::Dynamic<int> copy(attached);

    ASSERT_TRUE(copy.IsValid());
    EXPECT_TRUE(copy.OwnsData());
    EXPECT_NE(copy.Data(), storage);
    EXPECT_EQ(copy.At(1, 1), 4);

    storage[3] = 40;
    EXPECT_EQ(attached.At(1, 1), 40);
    EXPECT_EQ(copy.At(1, 1), 4);
}

TEST(MatrixTest, CopyAssignmentReplacesAttachmentWithOwnedStorage) {
    int sourceStorage[4] = {1, 2, 3, 4};
    int destinationStorage[4] = {};
    Matrix::Dynamic<int> source(2, 2, sourceStorage);
    Matrix::Dynamic<int> destination(2, 2, destinationStorage);

    destination = source;

    ASSERT_TRUE(destination.IsValid());
    EXPECT_TRUE(destination.OwnsData());
    EXPECT_NE(destination.Data(), sourceStorage);
    EXPECT_NE(destination.Data(), destinationStorage);
    EXPECT_EQ(destination.At(1, 1), 4);
}

TEST(MatrixTest, MoveTransfersOwnedAndAttachedStorageWithoutThrowing) {
    static_assert(std::is_nothrow_move_constructible<
        Matrix::Dynamic<int>
    >::value);
    static_assert(std::is_nothrow_move_assignable<
        Matrix::Dynamic<int>
    >::value);

    Matrix::Dynamic<int> owned(1, 2, 3);
    int* const ownedStorage = owned.Data();
    Matrix::Dynamic<int> moved(static_cast<Matrix::Dynamic<int>&&>(owned));

    EXPECT_FALSE(owned.IsValid());
    EXPECT_TRUE(moved.OwnsData());
    EXPECT_EQ(moved.Data(), ownedStorage);

    int externalStorage[2] = {7, 8};
    Matrix::Dynamic<int> attached(1, 2, externalStorage);
    Matrix::Dynamic<int> assigned;
    assigned = static_cast<Matrix::Dynamic<int>&&>(attached);

    EXPECT_FALSE(attached.IsValid());
    EXPECT_TRUE(assigned.IsValid());
    EXPECT_FALSE(assigned.OwnsData());
    EXPECT_EQ(assigned.Data(), externalStorage);
}

TEST(MatrixTest, TransposesIntoAttachedStorage) {
    Matrix::Dynamic<int> source(2, 3);
    int value = 1;
    for(unsigned int row = 0; row < source.RowsCount(); ++row) {
        for(unsigned int col = 0; col < source.ColsCount(); ++col) {
            source.At(row, col) = value++;
        }
    }

    int outputStorage[6] = {};
    Matrix::Dynamic<int> result(3, 2, outputStorage);

    ASSERT_TRUE(source.TransposeTo(result));
    EXPECT_FALSE(result.OwnsData());
    EXPECT_EQ(result.Data(), outputStorage);
    EXPECT_EQ(result.At(0, 0), 1);
    EXPECT_EQ(result.At(0, 1), 4);
    EXPECT_EQ(result.At(2, 0), 3);
    EXPECT_EQ(result.At(2, 1), 6);
}

TEST(MatrixTest, RejectsOverlappingTransposeStorage) {
    Matrix::Dynamic<int> source(2, 2);
    source.At(0, 0) = 1;
    source.At(0, 1) = 2;
    source.At(1, 0) = 3;
    source.At(1, 1) = 4;

    EXPECT_FALSE(source.TransposeTo(source));
    EXPECT_EQ(source.At(0, 1), 2);
    EXPECT_EQ(source.At(1, 0), 3);

    Matrix::Dynamic<int> overlapping(2, 2, source.Data());
    EXPECT_FALSE(source.TransposeTo(overlapping));
    EXPECT_EQ(source.At(0, 1), 2);
    EXPECT_EQ(source.At(1, 0), 3);
}

TEST(MatrixTest, MultipliesIntoAttachedStorage) {
    Matrix::Dynamic<int> left(2, 2);
    Matrix::Dynamic<int> right(2, 2);
    left.At(0, 0) = 1;
    left.At(0, 1) = 2;
    left.At(1, 0) = 3;
    left.At(1, 1) = 4;
    right.At(0, 0) = 5;
    right.At(0, 1) = 6;
    right.At(1, 0) = 7;
    right.At(1, 1) = 8;

    int outputStorage[4] = {};
    Matrix::Dynamic<int> result(2, 2, outputStorage);

    ASSERT_TRUE(Matrix::Multiply(left, right, result));
    EXPECT_FALSE(result.OwnsData());
    EXPECT_EQ(result.Data(), outputStorage);
    EXPECT_EQ(result.At(0, 0), 19);
    EXPECT_EQ(result.At(0, 1), 22);
    EXPECT_EQ(result.At(1, 0), 43);
    EXPECT_EQ(result.At(1, 1), 50);
}

TEST(MatrixTest, RejectsOverlappingMultiplicationResult) {
    Matrix::Dynamic<int> left(2, 2, 2);
    Matrix::Dynamic<int> right(2, 2, 3);

    EXPECT_FALSE(Matrix::Multiply(left, right, left));
    EXPECT_EQ(left.At(0, 0), 2);
    EXPECT_EQ(left.At(1, 1), 2);

    Matrix::Dynamic<int> overlapping(2, 2, right.Data());
    EXPECT_FALSE(Matrix::Multiply(left, right, overlapping));
    EXPECT_EQ(right.At(0, 0), 3);
    EXPECT_EQ(right.At(1, 1), 3);
}
