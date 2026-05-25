#include <Foundation_TypeTraits.h>

using namespace Foundation::TypeTraits;

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

int ExampleFunction(int Value) {
    return Value + 1;
}

template <typename T>
class NumericBuffer {
public:
    static_assert(
        is_arithmetic<T>::value,
        "NumericBuffer only supports arithmetic types"
    );

private:
    T* _Data;
    unsigned int _Size;

public:
    NumericBuffer(T* Data, unsigned int Size)
        : _Data(Data), _Size(Size) {
    }

    T Get(unsigned int Index) const {
        return _Data[Index];
    }

    void Set(unsigned int Index, T Value) {
        _Data[Index] = Value;
    }

    unsigned int Size() const {
        return _Size;
    }
};

template <typename T>
int ProcessScalar(T Value) {
    static_assert(
        is_scalar<T>::value,
        "ProcessScalar only supports scalar types"
    );

    return Value == T() ? 0 : 1;
}

template <typename T>
int ProcessObject(const T& Object) {
    static_assert(
        is_object<T>::value,
        "ProcessObject requires an object type"
    );

    (void)Object;
    return 1;
}

int main() {

    static_assert(is_integral<int>::value, "int should be integral");
    static_assert(is_floating_point<float>::value, "float should be floating point");
    static_assert(is_arithmetic<double>::value, "double should be arithmetic");

    static_assert(is_class<SensorPacket>::value, "SensorPacket should be class");
    static_assert(is_class<SensorSample>::value, "SensorSample should be class");
    static_assert(is_union<RawSample>::value, "RawSample should be union");
    static_assert(is_enum<SensorState>::value, "SensorState should be enum");

    static_assert(is_pointer<int*>::value, "int* should be pointer");
    static_assert(is_lvalue_reference<int&>::value, "int& should be lvalue reference");
    static_assert(is_rvalue_reference<int&&>::value, "int&& should be rvalue reference");
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

    static_assert(is_function<decltype(ExampleFunction)>::value, "ExampleFunction type should be function");

    int Data[4] = {1, 2, 3, 4};

    NumericBuffer<int> Buffer(Data, 4);
    Buffer.Set(0, 10);

    SensorSample Sample;
    Sample.Channel = 1;
    Sample.Value = 0.5f;

    int Result = 0;
    Result += Buffer.Get(0);
    Result += static_cast<int>(Buffer.Size());
    Result += ProcessScalar<int>(5);
    Result += ProcessObject<SensorSample>(Sample);

    return Result == 16 ? 0 : 1;
}
