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
        freeFunction.bind(Add);

        Accumulator accumulator;
        Foundation::Functional::Callback<int, int> memberFunction;
        memberFunction.bind<Accumulator, &Accumulator::AddValue>(&accumulator);
        memberFunction.invoke(5);

        Result result;
        result.FreeFunctionValue = freeFunction.invoke(4, 5);
        result.MemberFunctionValue = memberFunction.invoke(3);
        result.BoundBeforeUnbind = memberFunction.status();
        memberFunction.unbind();
        result.BoundAfterUnbind = memberFunction.status();
        return result;
    }

} // namespace Callback
} // namespace Functional
} // namespace FoundationExamples
