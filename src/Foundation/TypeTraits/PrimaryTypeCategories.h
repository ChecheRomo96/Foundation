#ifndef FOUNDATION_TYPE_TRAITS_PRIMARY_TYPE_CATEGORIES_H
#define FOUNDATION_TYPE_TRAITS_PRIMARY_TYPE_CATEGORIES_H

#include <Foundation_BuildSettings.h>
#include <stddef.h>
#include "HelperClasses.h"
#include "TypeProperties.h"

namespace Foundation::TypeTraits {

    /** @cond FOUNDATION_INTERNAL */
    namespace Detail {

#if FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS

        template <typename T> using is_array_backend = std::is_array<T>;
        template <typename T> using is_class_backend = std::is_class<T>;
        template <typename T> using is_enum_backend = std::is_enum<T>;
        template <typename T>
        using is_floating_point_backend = std::is_floating_point<T>;
        template <typename T> using is_function_backend = std::is_function<T>;
        template <typename T>
        using is_lvalue_reference_backend = std::is_lvalue_reference<T>;
        template <typename T>
        using is_member_function_pointer_backend =
            std::is_member_function_pointer<T>;
        template <typename T>
        using is_member_object_pointer_backend =
            std::is_member_object_pointer<T>;
        template <typename T> using is_pointer_backend = std::is_pointer<T>;
        template <typename T>
        using is_rvalue_reference_backend = std::is_rvalue_reference<T>;
        template <typename T> using is_union_backend = std::is_union<T>;
        template <typename T> using is_void_backend = std::is_void<T>;

#else

        template <typename T>
        struct is_array_backend : false_type {};

        template <typename T>
        struct is_array_backend<T[]> : true_type {};

        template <typename T, size_t N>
        struct is_array_backend<T[N]> : true_type {};

        template <typename T>
        struct is_class_backend : bool_constant<__is_class(T)> {};

        template <typename T>
        struct is_enum_backend : bool_constant<__is_enum(T)> {};

        template <typename T>
        struct is_floating_point_backend : false_type {};

        template <> struct is_floating_point_backend<float> : true_type {};
        template <> struct is_floating_point_backend<double> : true_type {};
        template <>
        struct is_floating_point_backend<long double> : true_type {};

        template <typename T>
        struct is_function_reference_backend : false_type {};

        template <typename T>
        struct is_function_reference_backend<T&> : true_type {};

        template <typename T>
        struct is_function_reference_backend<T&&> : true_type {};

        template <typename T>
        struct is_function_backend : bool_constant<
            !is_const<const T>::value &&
            !is_function_reference_backend<T>::value
        > {};

        template <typename T>
        struct is_lvalue_reference_backend : false_type {};

        template <typename T>
        struct is_lvalue_reference_backend<T&> : true_type {};

        template <typename T>
        struct is_member_function_pointer_backend : false_type {};

        template <typename T, typename C>
        struct is_member_function_pointer_backend<T C::*>
            : is_function_backend<T> {};

        template <typename T>
        struct is_member_object_pointer_backend : false_type {};

        template <typename T, typename C>
        struct is_member_object_pointer_backend<T C::*>
            : bool_constant<!is_function_backend<T>::value> {};

        template <typename T>
        struct is_pointer_backend : false_type {};

        template <typename T>
        struct is_pointer_backend<T*> : true_type {};

        template <typename T>
        struct is_rvalue_reference_backend : false_type {};

        template <typename T>
        struct is_rvalue_reference_backend<T&&> : true_type {};

        template <typename T>
        struct is_union_backend : bool_constant<__is_union(T)> {};

        template <typename T>
        struct is_void_backend : false_type {};

        template <> struct is_void_backend<void> : true_type {};

#endif

    }
    /** @endcond */

    /**
     * @brief Reports whether a type is an array of known or unknown bound.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_array
        : bool_constant<Detail::is_array_backend<T>::value> {};

    /**
     * @brief Reports whether a type is a non-union class type.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_class
        : bool_constant<Detail::is_class_backend<T>::value> {};

    /**
     * @brief Reports whether a type is an enumeration.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_enum
        : bool_constant<Detail::is_enum_backend<T>::value> {};

    /**
     * @brief Reports whether a type is `float`, `double`, or `long double`.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect; top-level cv qualifiers are ignored.
     */
    template <typename T>
    struct is_floating_point
        : bool_constant<
            Detail::is_floating_point_backend<remove_cv_t<T>>::value
        > {};

    /**
     * @brief Reports whether a type is a function type.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect.
     *
     * This includes variadic, cv-qualified, ref-qualified, and `noexcept`
     * function types.
     */
    template <typename T>
    struct is_function
        : bool_constant<Detail::is_function_backend<T>::value> {};

    /**
     * @brief Reports whether a type is integral.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect; top-level cv qualifiers are ignored.
     */
    template <typename T>
    struct is_integral
        : bool_constant<
            Detail::is_integral_backend<remove_cv_t<T>>::value
        > {};

    /**
     * @brief Reports whether a type is an lvalue reference.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_lvalue_reference
        : bool_constant<Detail::is_lvalue_reference_backend<T>::value> {};

    /**
     * @brief Reports whether a type is a pointer to a member function.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect; top-level cv qualifiers are ignored.
     */
    template <typename T>
    struct is_member_function_pointer
        : bool_constant<
            Detail::is_member_function_pointer_backend<remove_cv_t<T>>::value
        > {};

    /**
     * @brief Reports whether a type is a pointer to a non-function member.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect; top-level cv qualifiers are ignored.
     */
    template <typename T>
    struct is_member_object_pointer
        : bool_constant<
            Detail::is_member_object_pointer_backend<remove_cv_t<T>>::value
        > {};

    /**
     * @brief Reports whether a type is a pointer to an object or function.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect; top-level cv qualifiers are ignored.
     */
    template <typename T>
    struct is_pointer
        : bool_constant<Detail::is_pointer_backend<remove_cv_t<T>>::value> {};

    /**
     * @brief Reports whether a type is an rvalue reference.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_rvalue_reference
        : bool_constant<Detail::is_rvalue_reference_backend<T>::value> {};

    /**
     * @brief Reports whether a type is a union.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_union
        : bool_constant<Detail::is_union_backend<T>::value> {};

    /**
     * @brief Reports whether a type is `void`.
     * @ingroup Foundation_TypeTraits_Primary
     * @tparam T Type to inspect; top-level cv qualifiers are ignored.
     */
    template <typename T>
    struct is_void
        : bool_constant<Detail::is_void_backend<remove_cv_t<T>>::value> {};

    /** @brief `is_array<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_array_v = is_array<T>::value;

    /** @brief `is_class<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_class_v = is_class<T>::value;

    /** @brief `is_enum<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_enum_v = is_enum<T>::value;

    /** @brief `is_floating_point<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

    /** @brief `is_function<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_function_v = is_function<T>::value;

    /** @brief `is_integral<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_integral_v = is_integral<T>::value;

    /** @brief `is_lvalue_reference<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_lvalue_reference_v =
        is_lvalue_reference<T>::value;

    /** @brief `is_member_function_pointer<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_member_function_pointer_v =
        is_member_function_pointer<T>::value;

    /** @brief `is_member_object_pointer<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_member_object_pointer_v =
        is_member_object_pointer<T>::value;

    /** @brief `is_pointer<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_pointer_v = is_pointer<T>::value;

    /** @brief `is_rvalue_reference<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_rvalue_reference_v =
        is_rvalue_reference<T>::value;

    /** @brief `is_union<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_union_v = is_union<T>::value;

    /** @brief `is_void<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Primary */
    template <typename T>
    inline constexpr bool is_void_v = is_void<T>::value;

}

#endif // FOUNDATION_TYPE_TRAITS_PRIMARY_TYPE_CATEGORIES_H
