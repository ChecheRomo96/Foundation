#ifndef FOUNDATION_TYPE_TRAITS_TYPE_RELATIONSHIPS_H
#define FOUNDATION_TYPE_TRAITS_TYPE_RELATIONSHIPS_H

#include <Foundation_BuildSettings.h>
#include "HelperClasses.h"

#if defined(FOUNDATION_USING_STL)
    #include <type_traits>
#endif

namespace Foundation::TypeTraits {

#if defined(FOUNDATION_USING_STL)

    template <class T, class U>
    using is_same = std::is_same<T, U>;

#else

    template <class T, class U>
    struct is_same : false_type {};

    template <class T>
    struct is_same<T, T> : true_type {};

#endif

#if FOUNDATION_CPLUSPLUS >= 201402L
    template <class T, class U>
    constexpr bool is_same_v = is_same<T, U>::value;
#endif


#if defined(FOUNDATION_USING_STL)

    template <class Base, class Derived>
    using is_base_of = std::is_base_of<Base, Derived>;

#else

    template <class Base, class Derived>
    struct is_base_of {
    private:
        static true_type test(Base*);
        static false_type test(...);

        static Derived* Get();

    public:
        static const bool value = decltype(test(Get()))::value;
        typedef integral_constant<bool, value> type;
    };

#endif

#if FOUNDATION_CPLUSPLUS >= 201402L
    template <class Base, class Derived>
    constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
#endif


#if defined(FOUNDATION_USING_STL)

    template <class From, class To>
    using is_convertible = std::is_convertible<From, To>;

#else

    template <class From, class To>
    struct is_convertible {
    private:
        static From&& Declval() noexcept;
        static void TestConvertible(To);

        template <class F, class T>
        static auto Test(int) -> decltype(
            TestConvertible(Declval()),
            true_type()
        );

        template <class, class>
        static false_type Test(...);

    public:
        static const bool value = decltype(Test<From, To>(0))::value;
        typedef integral_constant<bool, value> type;
    };

#endif

#if FOUNDATION_CPLUSPLUS >= 201402L
    template <class From, class To>
    constexpr bool is_convertible_v = is_convertible<From, To>::value;
#endif


#if defined(FOUNDATION_USING_STL) && (FOUNDATION_CPLUSPLUS >= 201703L)

    template <class Fn, class... Args>
    using is_invocable = std::is_invocable<Fn, Args...>;

#else

    template <class Fn, class... Args>
    struct is_invocable {
    private:
        template <class T>
        static T&& Declval() noexcept;

        template <class F, class... A>
        static auto Test(int) -> decltype(
            Declval<F>()(Declval<A>()...),
            true_type()
        );

        template <class, class...>
        static false_type Test(...);

    public:
        static const bool value = decltype(Test<Fn, Args...>(0))::value;
        typedef integral_constant<bool, value> type;
    };

#endif

#if FOUNDATION_CPLUSPLUS >= 201402L
    template <class Fn, class... Args>
    constexpr bool is_invocable_v = is_invocable<Fn, Args...>::value;
#endif


#if defined(FOUNDATION_USING_STL) && (FOUNDATION_CPLUSPLUS >= 201703L)

    template <class Fn, class... Args>
    using is_nothrow_invocable = std::is_nothrow_invocable<Fn, Args...>;

#else

    template <class Fn, class... Args>
    struct is_nothrow_invocable {
    private:
        template <class T>
        static T&& Declval() noexcept;

        template <class F, class... A>
        static integral_constant<
            bool,
            noexcept(Declval<F>()(Declval<A>()...))
        > Test(int);

        template <class, class...>
        static false_type Test(...);

        typedef decltype(Test<Fn, Args...>(0)) result_type;

    public:
        static const bool value =
            result_type::value && is_invocable<Fn, Args...>::value;

        typedef integral_constant<bool, value> type;
    };

#endif

#if FOUNDATION_CPLUSPLUS >= 201402L
    template <class Fn, class... Args>
    constexpr bool is_nothrow_invocable_v =
        is_nothrow_invocable<Fn, Args...>::value;
#endif


#if defined(FOUNDATION_USING_STL) && (FOUNDATION_CPLUSPLUS >= 202002L)

    template <class From, class To>
    using is_nothrow_convertible = std::is_nothrow_convertible<From, To>;

#else

    template <class From, class To>
    struct is_nothrow_convertible {
    private:
        static From&& Declval() noexcept;
        static void TestConvertible(To);

        static const bool convertible = is_convertible<From, To>::value;

    public:
        static const bool value =
            convertible &&
            noexcept(TestConvertible(Declval()));

        typedef integral_constant<bool, value> type;
    };

#endif

#if FOUNDATION_CPLUSPLUS >= 201402L
    template <class From, class To>
    constexpr bool is_nothrow_convertible_v =
        is_nothrow_convertible<From, To>::value;
#endif

}

#endif // FOUNDATION_TYPE_TRAITS_TYPE_RELATIONSHIPS_H