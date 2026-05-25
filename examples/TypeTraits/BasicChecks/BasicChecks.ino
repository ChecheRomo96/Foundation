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

void setup() {

    Serial.begin(115200);

    static_assert(is_integral<int>::value, "int should be integral");
    static_assert(is_floating_point<float>::value, "float should be floating point");
    static_assert(is_arithmetic<double>::value, "double should be arithmetic");

    static_assert(is_class<SensorPacket>::value, "SensorPacket should be class");
    static_assert(is_union<RawSample>::value, "RawSample should be union");
    static_assert(is_enum<SensorState>::value, "SensorState should be enum");

    static_assert(is_pointer<int*>::value, "int* should be pointer");
    static_assert(is_reference<int&>::value, "int& should be reference");

    static_assert(is_object<SensorSample>::value, "SensorSample should be object");
    static_assert(!is_scalar<SensorSample>::value, "SensorSample should not be scalar");

    int Data[4] = {1, 2, 3, 4};

    NumericBuffer<int> Buffer(Data, 4);
    Buffer.Set(0, 10);

    SensorSample Sample;
    Sample.Channel = 1;
    Sample.Value = 0.5f;

    Serial.println("Foundation TypeTraits BasicChecks example");
    Serial.print("Buffer[0]: ");
    Serial.println(Buffer.Get(0));
    Serial.print("Sample channel: ");
    Serial.println(Sample.Channel);
    Serial.print("Sample value: ");
    Serial.println(Sample.Value);
}

void loop() {
}
