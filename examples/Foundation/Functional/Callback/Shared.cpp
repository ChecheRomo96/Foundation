#include "Shared.h"

namespace FoundationExamples {
namespace Functional {
namespace Callback {

    namespace {

        int Add(int first, int second) {
            return first + second;
        }

        class Accumulator {
        public:
            Accumulator()
                : _total(0) {
            }

            int AddValue(int value) {
                _total += value;
                return _total;
            }

        private:
            int _total;
        };

    } // namespace

    Result Run() {
        Foundation::Functional::Callback<int, int, int> freeFunction;
        freeFunction.Bind(Add);

        Accumulator accumulator;
        Foundation::Functional::Callback<int, int> memberFunction;
        memberFunction.Bind<Accumulator, &Accumulator::AddValue>(&accumulator);
        memberFunction.Invoke(5);

        Result result;
        result.FreeFunctionValue = freeFunction.Invoke(4, 5);
        result.MemberFunctionValue = memberFunction.Invoke(3);
        result.BoundBeforeUnbind = memberFunction.IsBound();
        memberFunction.Unbind();
        result.BoundAfterUnbind = memberFunction.IsBound();
        return result;
    }

} // namespace Callback
} // namespace Functional
} // namespace FoundationExamples
