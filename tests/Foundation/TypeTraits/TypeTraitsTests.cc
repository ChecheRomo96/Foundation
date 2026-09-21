#include <gtest/gtest.h>

#include <Foundation/TypeTraits/TypeTraits.h>

namespace {

struct Sample {
    int value;
};

union Storage {
    int integer;
    float real;
};

enum State {
    Idle,
    Active
};

int Transform(int value) {
    return value + 1;
}

} // namespace

TEST(TypeTraitsTest, ClassifiesPrimaryAndCompositeTypes) {
    using namespace Foundation::TypeTraits;

    static_assert(is_integral<int>::value, "int must be integral");
    static_assert(is_floating_point<float>::value, "float must be floating point");
    static_assert(is_arithmetic<double>::value, "double must be arithmetic");
    static_assert(is_class<Sample>::value, "Sample must be a class type");
    static_assert(is_union<Storage>::value, "Storage must be a union");
    static_assert(is_enum<State>::value, "State must be an enum");
    static_assert(is_pointer<int*>::value, "int* must be a pointer");
    static_assert(is_reference<int&>::value, "int& must be a reference");
    static_assert(is_scalar<State>::value, "State must be scalar");
    static_assert(is_object<Sample>::value, "Sample must be an object type");
    static_assert(!is_object<void>::value, "void must not be an object type");
    static_assert(is_function<decltype(Transform)>::value, "Transform must be a function");

    SUCCEED();
}

TEST(TypeTraitsTest, IdentifiesTypeRelationships) {
    using Foundation::TypeTraits::is_same;

    EXPECT_TRUE((is_same<int, int>::value));
    EXPECT_FALSE((is_same<int, long>::value));
}
