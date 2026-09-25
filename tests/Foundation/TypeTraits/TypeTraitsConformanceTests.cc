#include <gtest/gtest.h>

#include <functional>
#include <type_traits>

#include <Foundation/TypeTraits.h>

namespace {

namespace Traits = Foundation::TypeTraits;

#if defined(FOUNDATION_TEST_EXPECT_CUSTOM_TYPE_TRAITS)
static_assert(
    FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS == 0,
    "The fallback conformance target must compile the custom backend"
);
#elif defined(FOUNDATION_TEST_EXPECT_STD_TYPE_TRAITS)
static_assert(
    FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS == 1,
    "The default desktop target must compile the standard backend"
);
#endif

struct Base {};
struct Derived : Base {};
struct PrivateDerived : private Base {};

union SampleUnion {
    int integer;
    float real;
};

enum class SampleEnum {
    Value
};

struct Callable {
    int value;

    int Throwing(int input) {
        return input;
    }

    int NoThrow(int input) const noexcept {
        return input;
    }
};

struct ImplicitNoThrowConversion {
    operator int() const noexcept {
        return 1;
    }
};

struct ImplicitThrowingConversion {
    operator int() const {
        return 1;
    }
};

struct ExplicitConversion {
    explicit operator int() const noexcept {
        return 1;
    }
};

int ThrowingFunction(int input) {
    return input;
}

int NoThrowFunction(int input) noexcept {
    return input;
}

using QualifiedFunction = int(int) const & noexcept;

template <typename T>
constexpr bool MatchesCxx17UnaryTraits() {
    return
        Traits::is_array<T>::value == std::is_array<T>::value &&
        Traits::is_class<T>::value == std::is_class<T>::value &&
        Traits::is_enum<T>::value == std::is_enum<T>::value &&
        Traits::is_floating_point<T>::value ==
            std::is_floating_point<T>::value &&
        Traits::is_function<T>::value == std::is_function<T>::value &&
        Traits::is_integral<T>::value == std::is_integral<T>::value &&
        Traits::is_lvalue_reference<T>::value ==
            std::is_lvalue_reference<T>::value &&
        Traits::is_member_function_pointer<T>::value ==
            std::is_member_function_pointer<T>::value &&
        Traits::is_member_object_pointer<T>::value ==
            std::is_member_object_pointer<T>::value &&
        Traits::is_pointer<T>::value == std::is_pointer<T>::value &&
        Traits::is_rvalue_reference<T>::value ==
            std::is_rvalue_reference<T>::value &&
        Traits::is_union<T>::value == std::is_union<T>::value &&
        Traits::is_void<T>::value == std::is_void<T>::value &&
        Traits::is_const<T>::value == std::is_const<T>::value &&
        Traits::is_unsigned<T>::value == std::is_unsigned<T>::value &&
        Traits::is_volatile<T>::value == std::is_volatile<T>::value &&
        Traits::is_arithmetic<T>::value == std::is_arithmetic<T>::value &&
        Traits::is_compound<T>::value == std::is_compound<T>::value &&
        Traits::is_fundamental<T>::value == std::is_fundamental<T>::value &&
        Traits::is_member_pointer<T>::value ==
            std::is_member_pointer<T>::value &&
        Traits::is_object<T>::value == std::is_object<T>::value &&
        Traits::is_reference<T>::value == std::is_reference<T>::value &&
        Traits::is_scalar<T>::value == std::is_scalar<T>::value;
}

} // namespace

TEST(TypeTraitsConformanceTest, MatchesPrimaryAndCompositeCxx17Traits) {
    static_assert(Traits::is_integral<char16_t>::value ==
        std::is_integral<char16_t>::value);
    static_assert(Traits::is_integral<char32_t>::value ==
        std::is_integral<char32_t>::value);
    static_assert(Traits::is_integral<const unsigned long>::value ==
        std::is_integral<const unsigned long>::value);
    static_assert(Traits::is_integral<float>::value ==
        std::is_integral<float>::value);

    static_assert(Traits::is_function<decltype(NoThrowFunction)>::value ==
        std::is_function<decltype(NoThrowFunction)>::value);
    static_assert(Traits::is_function<QualifiedFunction>::value ==
        std::is_function<QualifiedFunction>::value);
    static_assert(Traits::is_function<decltype(&NoThrowFunction)>::value ==
        std::is_function<decltype(&NoThrowFunction)>::value);

    static_assert(Traits::is_unsigned<unsigned int>::value ==
        std::is_unsigned<unsigned int>::value);
    static_assert(Traits::is_unsigned<bool>::value ==
        std::is_unsigned<bool>::value);
    static_assert(Traits::is_unsigned<int>::value ==
        std::is_unsigned<int>::value);
    static_assert(Traits::is_unsigned<void>::value ==
        std::is_unsigned<void>::value);
    static_assert(Traits::is_unsigned<Callable>::value ==
        std::is_unsigned<Callable>::value);

    static_assert(Traits::is_scalar<decltype(nullptr)>::value ==
        std::is_scalar<decltype(nullptr)>::value);
    static_assert(Traits::is_object<int[2]>::value ==
        std::is_object<int[2]>::value);
    static_assert(Traits::is_fundamental<const void>::value ==
        std::is_fundamental<const void>::value);
    static_assert(Traits::is_compound<Derived>::value ==
        std::is_compound<Derived>::value);

    SUCCEED();
}

TEST(TypeTraitsConformanceTest, MatchesEverySupportedUnaryPredicate) {
    static_assert(MatchesCxx17UnaryTraits<int>());
    static_assert(MatchesCxx17UnaryTraits<const unsigned long>());
    static_assert(MatchesCxx17UnaryTraits<volatile double>());
    static_assert(MatchesCxx17UnaryTraits<void>());
    static_assert(MatchesCxx17UnaryTraits<decltype(nullptr)>());
    static_assert(MatchesCxx17UnaryTraits<Callable>());
    static_assert(MatchesCxx17UnaryTraits<SampleUnion>());
    static_assert(MatchesCxx17UnaryTraits<SampleEnum>());
    static_assert(MatchesCxx17UnaryTraits<int*>());
    static_assert(MatchesCxx17UnaryTraits<int[3]>());
    static_assert(MatchesCxx17UnaryTraits<int&>());
    static_assert(MatchesCxx17UnaryTraits<int&&>());
    static_assert(MatchesCxx17UnaryTraits<decltype(ThrowingFunction)>());
    static_assert(MatchesCxx17UnaryTraits<QualifiedFunction>());
    static_assert(MatchesCxx17UnaryTraits<decltype(&Callable::Throwing)>());
    static_assert(MatchesCxx17UnaryTraits<decltype(&Callable::value)>());

    SUCCEED();
}

TEST(TypeTraitsConformanceTest, MatchesRelationshipsAndConversions) {
    static_assert(Traits::is_base_of<Base, Derived>::value ==
        std::is_base_of<Base, Derived>::value);
    static_assert(Traits::is_base_of<Base, PrivateDerived>::value ==
        std::is_base_of<Base, PrivateDerived>::value);
    static_assert(Traits::is_base_of<Derived, Base>::value ==
        std::is_base_of<Derived, Base>::value);

    static_assert(Traits::is_convertible<int, long>::value ==
        std::is_convertible<int, long>::value);
    static_assert(Traits::is_convertible<void, void>::value ==
        std::is_convertible<void, void>::value);
    static_assert(Traits::is_convertible<int, void>::value ==
        std::is_convertible<int, void>::value);
    static_assert(Traits::is_convertible<ImplicitNoThrowConversion, int>::value ==
        std::is_convertible<ImplicitNoThrowConversion, int>::value);
    static_assert(Traits::is_convertible<ExplicitConversion, int>::value ==
        std::is_convertible<ExplicitConversion, int>::value);

    static_assert(Traits::is_nothrow_convertible<int, long>::value);
    static_assert(Traits::is_nothrow_convertible<void, void>::value);
    static_assert(Traits::is_nothrow_convertible<
        ImplicitNoThrowConversion,
        int
    >::value);
    static_assert(!Traits::is_nothrow_convertible<
        ImplicitThrowingConversion,
        int
    >::value);
    static_assert(!Traits::is_nothrow_convertible<ExplicitConversion, int>::value);

    SUCCEED();
}

TEST(TypeTraitsConformanceTest, SupportsTheCxx17InvokeModel) {
    using ThrowingMember = decltype(&Callable::Throwing);
    using NoThrowMember = decltype(&Callable::NoThrow);
    using MemberObject = decltype(&Callable::value);

    static_assert(Traits::is_invocable<decltype(&ThrowingFunction), int>::value ==
        std::is_invocable<decltype(&ThrowingFunction), int>::value);
    static_assert(Traits::is_invocable<ThrowingMember, Callable&, int>::value ==
        std::is_invocable<ThrowingMember, Callable&, int>::value);
    static_assert(Traits::is_invocable<ThrowingMember, Callable*, int>::value ==
        std::is_invocable<ThrowingMember, Callable*, int>::value);
    static_assert(Traits::is_invocable<
        ThrowingMember,
        std::reference_wrapper<Callable>,
        int
    >::value == std::is_invocable<
        ThrowingMember,
        std::reference_wrapper<Callable>,
        int
    >::value);
    static_assert(Traits::is_invocable<NoThrowMember, const Callable&, int>::value ==
        std::is_invocable<NoThrowMember, const Callable&, int>::value);
    static_assert(Traits::is_invocable<MemberObject, Callable&>::value ==
        std::is_invocable<MemberObject, Callable&>::value);
    static_assert(Traits::is_invocable<MemberObject, Callable*>::value ==
        std::is_invocable<MemberObject, Callable*>::value);
    static_assert(!Traits::is_invocable<ThrowingMember, int, int>::value);

    static_assert(Traits::is_nothrow_invocable<
        decltype(&NoThrowFunction),
        int
    >::value == std::is_nothrow_invocable<
        decltype(&NoThrowFunction),
        int
    >::value);
    static_assert(Traits::is_nothrow_invocable<
        NoThrowMember,
        const Callable&,
        int
    >::value == std::is_nothrow_invocable<
        NoThrowMember,
        const Callable&,
        int
    >::value);
    static_assert(Traits::is_nothrow_invocable<MemberObject, Callable&>::value ==
        std::is_nothrow_invocable<MemberObject, Callable&>::value);
    static_assert(!Traits::is_nothrow_invocable<
        decltype(&ThrowingFunction),
        int
    >::value);

    SUCCEED();
}

TEST(TypeTraitsConformanceTest, ProvidesStandardShapedResultsAndAliases) {
    static_assert(Traits::is_same_v<
        Traits::remove_const_t<const int>,
        int
    >);
    static_assert(Traits::is_same_v<
        Traits::remove_volatile_t<volatile int>,
        int
    >);
    static_assert(Traits::is_same_v<
        Traits::remove_cv_t<const volatile int>,
        int
    >);
    static_assert(Traits::is_same_v<
        typename Traits::is_integral<int>::value_type,
        bool
    >);
    static_assert(Traits::is_integral_v<int>);
    static_assert(Traits::is_const_v<const int>);
    static_assert(Traits::is_volatile_v<volatile int>);
    static_assert(Traits::is_unsigned_v<unsigned int>);
    static_assert(Traits::is_invocable_v<decltype(&NoThrowFunction), int>);

    SUCCEED();
}
