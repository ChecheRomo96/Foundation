#include "Shared.h"

namespace FoundationExamples {
namespace Math {
namespace Matrix {

    Result Run() {
        const int firstValues[2][2] = {
            {1, 2},
            {3, 4}
        };
        const int secondValues[2][2] = {
            {5, 6},
            {7, 8}
        };

        const Foundation::Math::Matrix::Fixed<int, 2, 2> first(firstValues);
        const Foundation::Math::Matrix::Fixed<int, 2, 2> second(secondValues);
        Foundation::Math::Matrix::Fixed<int, 2, 2> fixedProduct;
        Foundation::Math::Matrix::Multiply(first, second, fixedProduct);

        const Foundation::Math::Matrix::Fixed<int, 2, 2> fixedTranspose =
            first.Transposed();

        Foundation::Math::Matrix::Dynamic<int> dynamicFirst(2, 2);
        Foundation::Math::Matrix::Dynamic<int> dynamicSecond(2, 2);
        Foundation::Math::Matrix::Dynamic<int> dynamicProduct;

        for (unsigned int row = 0; row < 2; ++row) {
            for (unsigned int col = 0; col < 2; ++col) {
                dynamicFirst.At(row, col) = firstValues[row][col];
                dynamicSecond.At(row, col) = secondValues[row][col];
            }
        }

        const bool dynamicMultiplySucceeded = Foundation::Math::Matrix::Multiply(
            dynamicFirst,
            dynamicSecond,
            dynamicProduct
        );

        Result result = {};
        result.DynamicMultiplySucceeded = dynamicMultiplySucceeded;
        result.DynamicResultOwnsData = dynamicProduct.OwnsData();

        for (unsigned int row = 0; row < 2; ++row) {
            for (unsigned int col = 0; col < 2; ++col) {
                result.First[row][col] = first.At(row, col);
                result.Second[row][col] = second.At(row, col);
                result.FixedProduct[row][col] = fixedProduct.At(row, col);
                result.FixedTranspose[row][col] = fixedTranspose.At(row, col);
                result.DynamicProduct[row][col] = dynamicMultiplySucceeded
                    ? dynamicProduct.At(row, col)
                    : 0;
            }
        }

        return result;
    }

} // namespace Matrix
} // namespace Math
} // namespace FoundationExamples
