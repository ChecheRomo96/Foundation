#ifndef FOUNDATION_TYPE_TRAITS_TYPE_RELATIONSHIPS_H
#define FOUNDATION_TYPE_TRAITS_TYPE_RELATIONSHIPS_H

#include <Foundation_BuildSettings.h>
#include "HelperClasses.h"
#include "TypeProperties.h"
#include "PrimaryTypeCategories.h"

namespace Foundation::TypeTraits {

    /** @cond FOUNDATION_INTERNAL */
    namespace Detail {

#if FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS

        template <typename T, typename U>
        using is_same_backend = std::is_same<T, U>;

        template <typename Base, typename Derived>
        using is_base_of_backend = std::is_base_of<Base, Derived>;

#else

        template <typename T, typename U>
        struct is_same_backend : false_type {};

        template <typename T>
        struct is_same_backend<T, T> : true_type {};

        template <typename Base, typename Derived>
        struct is_base_of_backend
            : bool_constant<__is_base_of(Base, Derived)> {};

#endif

    }
    /** @endcond */

    /**
     * @brief Reports whether two types are identical, including qualifiers.
     * @ingroup Foundation_TypeTraits_Relationships
     * @tparam T First type.
     * @tparam U Second type.
    */
    template <class T, class U>
    struct is_same
        : bool_constant<Detail::is_same_backend<T, U>::value> {};

    /** @brief `is_same<T, U>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Relationships */
    template <class T, class U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    /**
     * @brief Reports whether `Base` is a base class of `Derived`.
     * @ingroup Foundation_TypeTraits_Relationships
     * @tparam Base Candidate base type.
     * @tparam Derived Candidate derived type.
     *
     * Private and ambiguous base classes still report true, matching the
     * standard trait's type relationship rather than pointer accessibility.
    */
    template <class Base, class Derived>
    struct is_base_of
        : bool_constant<Detail::is_base_of_backend<Base, Derived>::value> {};

    /** @brief `is_base_of<Base, Derived>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Relationships */
    template <class Base, class Derived>
    inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

    /** @cond FOUNDATION_INTERNAL */
    namespace Detail {

        template <typename T>
        struct add_rvalue_reference {
            using type = T&&;
        };

        template <>
        struct add_rvalue_reference<void> {
            using type = void;
        };

        template <>
        struct add_rvalue_reference<const void> {
            using type = const void;
        };

        template <>
        struct add_rvalue_reference<volatile void> {
            using type = volatile void;
        };

        template <>
        struct add_rvalue_reference<const volatile void> {
            using type = const volatile void;
        };

        template <typename T>
        typename add_rvalue_reference<T>::type Declval() noexcept;

        template <typename To>
        void AcceptConversion(To) noexcept;

        template <typename To>
        static auto TestReturnable(int) -> decltype(
            static_cast<To (*)()>(nullptr),
            true_type()
        );

        template <typename>
        static false_type TestReturnable(...);

        template <typename From, typename To>
        static auto TestImplicitConversion(int) -> decltype(
            AcceptConversion<To>(Declval<From>()),
            true_type()
        );

        template <typename, typename>
        static false_type TestImplicitConversion(...);

        template <typename From, typename To>
        struct is_convertible_base : bool_constant<
            (
                decltype(TestReturnable<To>(0))::value &&
                decltype(TestImplicitConversion<From, To>(0))::value
            ) || (
                is_void<From>::value &&
                is_void<To>::value
            )
        > {};

        template <typename T>
        struct remove_reference {
            using type = T;
        };

        template <typename T>
        struct remove_reference<T&> {
            using type = T;
        };

        template <typename T>
        struct remove_reference<T&&> {
            using type = T;
        };

        template <typename T>
        using remove_cv_reference_t = remove_cv_t<
            typename remove_reference<T>::type
        >;

        struct priority_tag_0 {};
        struct priority_tag_1 : priority_tag_0 {};
        struct priority_tag_2 : priority_tag_1 {};

        template <typename Fn, typename Object, typename... Args>
        auto InvokeMemberFunction(
            priority_tag_2,
            Fn&& function,
            Object&& object,
            Args&&... args
        ) noexcept(noexcept(
            (static_cast<Object&&>(object).*static_cast<Fn&&>(function))(
                static_cast<Args&&>(args)...
            )
        )) -> decltype(
            (static_cast<Object&&>(object).*static_cast<Fn&&>(function))(
                static_cast<Args&&>(args)...
            )
        );

        template <typename Fn, typename Object, typename... Args>
        auto InvokeMemberFunction(
            priority_tag_1,
            Fn&& function,
            Object&& object,
            Args&&... args
        ) noexcept(noexcept(
            (static_cast<Object&&>(object).get().*static_cast<Fn&&>(function))(
                static_cast<Args&&>(args)...
            )
        )) -> decltype(
            (static_cast<Object&&>(object).get().*static_cast<Fn&&>(function))(
                static_cast<Args&&>(args)...
            )
        );

        template <typename Fn, typename Object, typename... Args>
        auto InvokeMemberFunction(
            priority_tag_0,
            Fn&& function,
            Object&& object,
            Args&&... args
        ) noexcept(noexcept(
            ((*static_cast<Object&&>(object)).*static_cast<Fn&&>(function))(
                static_cast<Args&&>(args)...
            )
        )) -> decltype(
            ((*static_cast<Object&&>(object)).*static_cast<Fn&&>(function))(
                static_cast<Args&&>(args)...
            )
        );

        template <typename Fn, typename Object>
        auto InvokeMemberObject(
            priority_tag_2,
            Fn&& function,
            Object&& object
        ) noexcept(noexcept(
            static_cast<Object&&>(object).*static_cast<Fn&&>(function)
        )) -> decltype(
            static_cast<Object&&>(object).*static_cast<Fn&&>(function)
        );

        template <typename Fn, typename Object>
        auto InvokeMemberObject(
            priority_tag_1,
            Fn&& function,
            Object&& object
        ) noexcept(noexcept(
            static_cast<Object&&>(object).get().*static_cast<Fn&&>(function)
        )) -> decltype(
            static_cast<Object&&>(object).get().*static_cast<Fn&&>(function)
        );

        template <typename Fn, typename Object>
        auto InvokeMemberObject(
            priority_tag_0,
            Fn&& function,
            Object&& object
        ) noexcept(noexcept(
            (*static_cast<Object&&>(object)).*static_cast<Fn&&>(function)
        )) -> decltype(
            (*static_cast<Object&&>(object)).*static_cast<Fn&&>(function)
        );

        template <typename Fn, typename... Args>
        auto InvokeDispatch(
            false_type,
            false_type,
            Fn&& function,
            Args&&... args
        ) noexcept(noexcept(
            static_cast<Fn&&>(function)(static_cast<Args&&>(args)...)
        )) -> decltype(
            static_cast<Fn&&>(function)(static_cast<Args&&>(args)...)
        );

        template <typename Fn, typename Object, typename... Args>
        auto InvokeDispatch(
            true_type,
            false_type,
            Fn&& function,
            Object&& object,
            Args&&... args
        ) noexcept(noexcept(
            InvokeMemberFunction(
                priority_tag_2(),
                static_cast<Fn&&>(function),
                static_cast<Object&&>(object),
                static_cast<Args&&>(args)...
            )
        )) -> decltype(
            InvokeMemberFunction(
                priority_tag_2(),
                static_cast<Fn&&>(function),
                static_cast<Object&&>(object),
                static_cast<Args&&>(args)...
            )
        );

        template <typename Fn, typename Object>
        auto InvokeDispatch(
            false_type,
            true_type,
            Fn&& function,
            Object&& object
        ) noexcept(noexcept(
            InvokeMemberObject(
                priority_tag_2(),
                static_cast<Fn&&>(function),
                static_cast<Object&&>(object)
            )
        )) -> decltype(
            InvokeMemberObject(
                priority_tag_2(),
                static_cast<Fn&&>(function),
                static_cast<Object&&>(object)
            )
        );

        template <typename Fn, typename... Args>
        auto Invoke(Fn&& function, Args&&... args) noexcept(noexcept(
            InvokeDispatch(
                is_member_function_pointer<remove_cv_reference_t<Fn>>(),
                is_member_object_pointer<remove_cv_reference_t<Fn>>(),
                static_cast<Fn&&>(function),
                static_cast<Args&&>(args)...
            )
        )) -> decltype(
            InvokeDispatch(
                is_member_function_pointer<remove_cv_reference_t<Fn>>(),
                is_member_object_pointer<remove_cv_reference_t<Fn>>(),
                static_cast<Fn&&>(function),
                static_cast<Args&&>(args)...
            )
        );

        template <typename Fn, typename... Args>
        static auto TestInvocable(int) -> decltype(
            Invoke(Declval<Fn>(), Declval<Args>()...),
            true_type()
        );

        template <typename, typename...>
        static false_type TestInvocable(...);

        template <bool IsInvocable, typename Fn, typename... Args>
        struct is_nothrow_invocable_base : false_type {};

        template <typename Fn, typename... Args>
        struct is_nothrow_invocable_base<true, Fn, Args...>
            : bool_constant<noexcept(
                Invoke(Declval<Fn>(), Declval<Args>()...)
            )> {};

        template <
            typename From,
            typename To,
            bool IsConvertible,
            bool IsVoidConversion
        >
        struct is_nothrow_convertible_base : false_type {};

        template <typename From, typename To>
        struct is_nothrow_convertible_base<From, To, true, true>
            : true_type {};

        template <typename From, typename To>
        struct is_nothrow_convertible_base<From, To, true, false>
            : bool_constant<noexcept(
                AcceptConversion<To>(Declval<From>())
            )> {};

#if FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS

        template <typename From, typename To>
        using is_convertible_backend = std::is_convertible<From, To>;

        template <typename Fn, typename... Args>
        using is_invocable_backend = std::is_invocable<Fn, Args...>;

        template <typename Fn, typename... Args>
        using is_nothrow_invocable_backend =
            std::is_nothrow_invocable<Fn, Args...>;

#else

        template <typename From, typename To>
        struct is_convertible_backend
            : is_convertible_base<From, To> {};

        template <typename Fn, typename... Args>
        struct is_invocable_backend
            : decltype(TestInvocable<Fn, Args...>(0)) {};

        template <typename Fn, typename... Args>
        struct is_nothrow_invocable_backend
            : is_nothrow_invocable_base<
                is_invocable_backend<Fn, Args...>::value,
                Fn,
                Args...
            > {};

#endif

    }
    /** @endcond */

    /**
     * @brief Reports whether `From` is implicitly convertible to `To`.
     * @ingroup Foundation_TypeTraits_Relationships
     * @tparam From Source type.
     * @tparam To Destination type.
    */
    template <class From, class To>
    struct is_convertible
        : bool_constant<Detail::is_convertible_backend<From, To>::value> {};

    /** @brief `is_convertible<From, To>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Relationships */
    template <class From, class To>
    inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

    /**
     * @brief Reports whether a callable can be invoked with the supplied arguments.
     * @ingroup Foundation_TypeTraits_Relationships
     * @tparam Fn Callable type.
     * @tparam Args Argument types.
     *
     * Supports function objects, function pointers, member-function pointers,
     * and member-object pointers with objects, pointers, smart-pointer-like
     * dereference, or reference-wrapper-like `get()` access.
    */
    template <class Fn, class... Args>
    struct is_invocable
        : bool_constant<Detail::is_invocable_backend<Fn, Args...>::value> {};

    /** @brief `is_invocable<Fn, Args...>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Relationships */
    template <class Fn, class... Args>
    inline constexpr bool is_invocable_v = is_invocable<Fn, Args...>::value;

    /**
     * @brief Reports whether an invocation is valid and non-throwing.
     * @ingroup Foundation_TypeTraits_Relationships
     * @tparam Fn Callable type.
     * @tparam Args Argument types.
    */
    template <class Fn, class... Args>
    struct is_nothrow_invocable
        : bool_constant<
            Detail::is_nothrow_invocable_backend<Fn, Args...>::value
        > {};

    /** @brief `is_nothrow_invocable<Fn, Args...>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Relationships */
    template <class Fn, class... Args>
    inline constexpr bool is_nothrow_invocable_v =
        is_nothrow_invocable<Fn, Args...>::value;

    /**
     * @brief Reports whether an implicit conversion is non-throwing.
     * @ingroup Foundation_TypeTraits_Relationships
     * @tparam From Source type.
     * @tparam To Destination type.
     * @note This C++20-shaped trait is provided by Foundation under its C++17
     * baseline for consistency with the rest of the relationship API.
     */
    template <class From, class To>
    struct is_nothrow_convertible
        : Detail::is_nothrow_convertible_base<
            From,
            To,
            is_convertible<From, To>::value,
            is_void<From>::value && is_void<To>::value
        > {};

    /** @brief `is_nothrow_convertible<From, To>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Relationships */
    template <class From, class To>
    inline constexpr bool is_nothrow_convertible_v =
        is_nothrow_convertible<From, To>::value;

}

#endif // FOUNDATION_TYPE_TRAITS_TYPE_RELATIONSHIPS_H
