#include "Shared.h"

namespace FoundationExamples {
    namespace TypeTraits {
        namespace BasicChecks {

            namespace {

                int ExampleFunction(int value) {
                    return value + 1;
                }

                template <typename T>
                int ProcessScalar(T value) {
                    static_assert(
                        Foundation::TypeTraits::is_scalar<T>::value,
                        "ProcessScalar only supports scalar types"
                    );

                    return value == T() ? 0 : 1;
                }

                template <typename T>
                int ProcessObject(const T& object) {
                    static_assert(
                        Foundation::TypeTraits::is_object<T>::value,
                        "ProcessObject requires an object type"
                    );

                    (void)object;
                    return 1;
                }

            }

            Result Run() {
                using namespace Foundation::TypeTraits;

                static_assert(is_integral<int>::value, "int should be integral");
                static_assert(
                    is_floating_point<float>::value,
                    "float should be floating point"
                );
                static_assert(
                    is_arithmetic<double>::value,
                    "double should be arithmetic"
                );

                static_assert(is_class<SensorPacket>::value, "SensorPacket should be class");
                static_assert(is_class<SensorSample>::value, "SensorSample should be class");
                static_assert(is_union<RawSample>::value, "RawSample should be union");
                static_assert(is_enum<SensorState>::value, "SensorState should be enum");

                static_assert(is_pointer<int*>::value, "int* should be pointer");
                static_assert(
                    is_lvalue_reference<int&>::value,
                    "int& should be lvalue reference"
                );
                static_assert(
                    is_rvalue_reference<int&&>::value,
                    "int&& should be rvalue reference"
                );
                static_assert(is_reference<int&>::value, "int& should be reference");

                static_assert(is_object<int>::value, "int should be object");
                static_assert(is_object<SensorSample>::value, "SensorSample should be object");
                static_assert(!is_object<void>::value, "void should not be object");

                static_assert(is_scalar<int>::value, "int should be scalar");
                static_assert(is_scalar<int*>::value, "int* should be scalar");
                static_assert(is_scalar<SensorState>::value, "SensorState should be scalar");
                static_assert(!is_scalar<SensorSample>::value, "SensorSample should not be scalar");

                static_assert(is_fundamental<int>::value, "int should be fundamental");
                static_assert(is_fundamental<void>::value, "void should be fundamental");
                static_assert(is_compound<SensorSample>::value, "SensorSample should be compound");
                static_assert(
                    is_function<decltype(ExampleFunction)>::value,
                    "ExampleFunction type should be function"
                );

                int data[4] = {1, 2, 3, 4};
                NumericBuffer<int> buffer(data, 4);
                buffer.Set(0, 10);

                SensorSample sample;
                sample.Channel = 1;
                sample.Value = 0.5f;

                Result result;
                result.BufferFirst = buffer.Get(0);
                result.BufferSize = buffer.Size();
                result.Sample = sample;
                result.Score = result.BufferFirst
                    + static_cast<int>(result.BufferSize)
                    + ProcessScalar<int>(5)
                    + ProcessObject<SensorSample>(sample);

                return result;
            }

        }
    }
}
