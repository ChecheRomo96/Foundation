#ifndef FOUNDATION_EXAMPLES_TYPE_TRAITS_BASIC_CHECKS_SHARED_H
#define FOUNDATION_EXAMPLES_TYPE_TRAITS_BASIC_CHECKS_SHARED_H

#include <Foundation_TypeTraits.h>

namespace FoundationExamples {
    namespace TypeTraits {
        namespace BasicChecks {

            struct SensorSample {
                int Channel;
                float Value;
            };

            class SensorPacket {
            public:
                int Id;
            };

            union RawSample {
                int IntegerValue;
                float FloatValue;
            };

            enum SensorState {
                SensorIdle,
                SensorActive
            };

            template <typename T>
            class NumericBuffer {
            public:
                static_assert(
                    Foundation::TypeTraits::is_arithmetic<T>::value,
                    "NumericBuffer only supports arithmetic types"
                );

            private:
                T* _data;
                unsigned int _size;

            public:
                NumericBuffer(T* data, unsigned int size)
                    : _data(data), _size(size) {
                }

                T Get(unsigned int index) const {
                    return _data[index];
                }

                void Set(unsigned int index, T value) {
                    _data[index] = value;
                }

                unsigned int Size() const {
                    return _size;
                }
            };

            struct Result {
                int BufferFirst;
                unsigned int BufferSize;
                SensorSample Sample;
                int Score;
            };

            Result Run();

        }
    }
}

#endif
