#include "Shared.h"

namespace FoundationExamples {
namespace Utils {
namespace MoveAndSwap {

    namespace {

        class MoveOnlyValue {
        public:
            explicit MoveOnlyValue(int value)
                : Value(value) {
            }

            MoveOnlyValue(const MoveOnlyValue&) = delete;
            MoveOnlyValue& operator=(const MoveOnlyValue&) = delete;

            MoveOnlyValue(MoveOnlyValue&& other)
                : Value(other.Value) {
                other.Value = 0;
            }

            MoveOnlyValue& operator=(MoveOnlyValue&& other) {
                Value = other.Value;
                other.Value = 0;
                return *this;
            }

            int Value;
        };

    } // namespace

    Result Run() {
        int first = 3;
        int second = 9;
        Foundation::Utils::Swap(first, second);

        MoveOnlyValue source(42);
        MoveOnlyValue destination(Foundation::Utils::Move(source));

        Result result;
        result.FirstAfterSwap = first;
        result.SecondAfterSwap = second;
        result.MovedFromValue = source.Value;
        result.MovedToValue = destination.Value;
        return result;
    }

} // namespace MoveAndSwap
} // namespace Utils
} // namespace FoundationExamples
