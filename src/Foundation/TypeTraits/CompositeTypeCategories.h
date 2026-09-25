#ifndef FOUNDATION_TYPE_TRAITS_COMPOSITE_TYPE_CATEGORIES_H
#define FOUNDATION_TYPE_TRAITS_COMPOSITE_TYPE_CATEGORIES_H

#include <Foundation_BuildSettings.h>
#include "HelperClasses.h"
#include "TypeProperties.h"
#include "PrimaryTypeCategories.h"
#include "TypeRelationships.h"

namespace Foundation::TypeTraits {

    /** @cond FOUNDATION_INTERNAL */
    namespace Detail {

#if FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS

        template <typename T>
        using is_reference_backend = std::is_reference<T>;
        template <typename T>
        using is_member_pointer_backend = std::is_member_pointer<T>;
        template <typename T>
        using is_object_backend = std::is_object<T>;
        template <typename T>
        using is_arithmetic_backend = std::is_arithmetic<T>;
        template <typename T>
        using is_scalar_backend = std::is_scalar<T>;
        template <typename T>
        using is_fundamental_backend = std::is_fundamental<T>;
        template <typename T>
        using is_compound_backend = std::is_compound<T>;

#else

        template <typename T>
        struct is_reference_backend : bool_constant<
            is_lvalue_reference<T>::value ||
            is_rvalue_reference<T>::value
        > {};

        template <typename T>
        struct is_member_pointer_backend : bool_constant<
            is_member_object_pointer<T>::value ||
            is_member_function_pointer<T>::value
        > {};

        template <typename T>
        struct is_object_backend : bool_constant<
            !is_function<T>::value &&
            !is_void<T>::value &&
            !is_reference_backend<T>::value
        > {};

        template <typename T>
        struct is_arithmetic_backend : bool_constant<
            is_integral<T>::value ||
            is_floating_point<T>::value
        > {};

        template <typename T>
        struct is_scalar_backend : bool_constant<
            is_arithmetic_backend<T>::value ||
            is_pointer<T>::value ||
            is_member_pointer_backend<T>::value ||
            is_enum<T>::value ||
            is_same<decltype(nullptr), remove_cv_t<T>>::value
        > {};

        template <typename T>
        struct is_fundamental_backend : bool_constant<
            is_integral<T>::value ||
            is_floating_point<T>::value ||
            is_void<T>::value ||
            is_same<decltype(nullptr), remove_cv_t<T>>::value
        > {};

        template <typename T>
        struct is_compound_backend : bool_constant<
            !is_fundamental_backend<T>::value
        > {};

#endif

    }
    /** @endcond */

    /**
     * @brief Reports whether a type is an lvalue or rvalue reference.
     * @ingroup Foundation_TypeTraits_Composite
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_reference
        : bool_constant<Detail::is_reference_backend<T>::value> {};

    /**
     * @brief Reports whether a type is any pointer-to-member type.
     * @ingroup Foundation_TypeTraits_Composite
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_member_pointer
        : bool_constant<Detail::is_member_pointer_backend<T>::value> {};

    /**
     * @brief Reports whether a type is an object type.
     * @ingroup Foundation_TypeTraits_Composite
     * @tparam T Type to inspect.
     *
     * Object types exclude functions, `void`, and references.
     */
    template <typename T>
    struct is_object
        : bool_constant<Detail::is_object_backend<T>::value> {};

    /**
     * @brief Reports whether a type is integral or floating-point.
     * @ingroup Foundation_TypeTraits_Composite
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_arithmetic
        : bool_constant<Detail::is_arithmetic_backend<T>::value> {};

    /**
     * @brief Reports whether a type is scalar.
     * @ingroup Foundation_TypeTraits_Composite
     * @tparam T Type to inspect.
     *
     * Scalar types include arithmetic, pointer, member-pointer, enumeration,
     * and `nullptr_t` types.
     */
    template <typename T>
    struct is_scalar
        : bool_constant<Detail::is_scalar_backend<T>::value> {};

    /**
     * @brief Reports whether a type is fundamental.
     * @ingroup Foundation_TypeTraits_Composite
     * @tparam T Type to inspect.
     *
     * Fundamental types include integral, floating-point, `void`, and
     * `nullptr_t` types.
     */
    template <typename T>
    struct is_fundamental
        : bool_constant<Detail::is_fundamental_backend<T>::value> {};

    /**
     * @brief Reports whether a type is not fundamental.
     * @ingroup Foundation_TypeTraits_Composite
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_compound
        : bool_constant<Detail::is_compound_backend<T>::value> {};

    /**
     * @brief Reports whether a type is an unsigned integer other than `bool`.
     * @ingroup Foundation_TypeTraits_Composite
     * @tparam T Type to inspect; top-level cv qualifiers are ignored.
     *
     * Unlike `is_unsigned`, this Foundation-specific composite trait excludes
     * `bool`, making it suitable for counters, sizes, and numeric storage.
     */
    template <typename T>
    struct is_unsigned_integer
        : bool_constant<
            is_integral<T>::value &&
            is_unsigned<T>::value &&
            !is_same<remove_cv_t<T>, bool>::value
        > {};

    /** @brief `is_arithmetic<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Composite */
    template <typename T>
    inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

    /** @brief `is_fundamental<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Composite */
    template <typename T>
    inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

    /** @brief `is_member_pointer<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Composite */
    template <typename T>
    inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

    /** @brief `is_object<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Composite */
    template <typename T>
    inline constexpr bool is_object_v = is_object<T>::value;

    /** @brief `is_reference<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Composite */
    template <typename T>
    inline constexpr bool is_reference_v = is_reference<T>::value;

    /** @brief `is_scalar<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Composite */
    template <typename T>
    inline constexpr bool is_scalar_v = is_scalar<T>::value;

    /** @brief `is_compound<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Composite */
    template <typename T>
    inline constexpr bool is_compound_v = is_compound<T>::value;

    /** @brief `is_unsigned_integer<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Composite */
    template <typename T>
    inline constexpr bool is_unsigned_integer_v =
        is_unsigned_integer<T>::value;

}

#endif // FOUNDATION_TYPE_TRAITS_COMPOSITE_TYPE_CATEGORIES_H
