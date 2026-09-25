#ifndef FOUNDATION_TYPE_TRAITS_TYPE_PROPERTIES_H
#define FOUNDATION_TYPE_TRAITS_TYPE_PROPERTIES_H

#include <Foundation_BuildSettings.h>
#include "HelperClasses.h"

namespace Foundation::TypeTraits {

    /** @cond FOUNDATION_INTERNAL */
    namespace Detail {

#if FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS

        template <typename T>
        using remove_const_backend = std::remove_const<T>;

        template <typename T>
        using remove_volatile_backend = std::remove_volatile<T>;

        template <typename T>
        using remove_cv_backend = std::remove_cv<T>;

        template <typename T>
        using is_const_backend = std::is_const<T>;

        template <typename T>
        using is_volatile_backend = std::is_volatile<T>;

        template <typename T>
        using is_integral_backend = std::is_integral<T>;

        template <typename T>
        using is_unsigned_backend = std::is_unsigned<T>;

#else

        template <typename T>
        struct remove_const_backend {
            using type = T;
        };

        template <typename T>
        struct remove_const_backend<const T> {
            using type = T;
        };

        template <typename T>
        struct remove_volatile_backend {
            using type = T;
        };

        template <typename T>
        struct remove_volatile_backend<volatile T> {
            using type = T;
        };

        template <typename T>
        struct remove_cv_backend {
            using type = typename remove_volatile_backend<
                typename remove_const_backend<T>::type
            >::type;
        };

        template <typename T>
        struct is_const_backend : false_type {};

        template <typename T>
        struct is_const_backend<const T> : true_type {};

        template <typename T>
        struct is_volatile_backend : false_type {};

        template <typename T>
        struct is_volatile_backend<volatile T> : true_type {};

        template <typename T>
        struct is_integral_backend : false_type {};

        template <> struct is_integral_backend<bool> : true_type {};
        template <> struct is_integral_backend<char> : true_type {};
        template <> struct is_integral_backend<signed char> : true_type {};
        template <> struct is_integral_backend<unsigned char> : true_type {};
        template <> struct is_integral_backend<wchar_t> : true_type {};
        template <> struct is_integral_backend<char16_t> : true_type {};
        template <> struct is_integral_backend<char32_t> : true_type {};
        template <> struct is_integral_backend<short> : true_type {};
        template <> struct is_integral_backend<unsigned short> : true_type {};
        template <> struct is_integral_backend<int> : true_type {};
        template <> struct is_integral_backend<unsigned int> : true_type {};
        template <> struct is_integral_backend<long> : true_type {};
        template <> struct is_integral_backend<unsigned long> : true_type {};
        template <> struct is_integral_backend<long long> : true_type {};
        template <> struct is_integral_backend<unsigned long long> : true_type {};

        template <typename T, bool = is_integral_backend<T>::value>
        struct is_unsigned_backend : false_type {};

        template <typename T>
        struct is_unsigned_backend<T, true>
            : bool_constant<(T(0) < T(-1))> {};

#endif

    }
    /** @endcond */

    /**
     * @brief Removes a top-level `const` qualifier from a type.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to transform.
     */
    template <typename T>
    struct remove_const {
        using type = typename Detail::remove_const_backend<T>::type;
    };

    /**
     * @brief Removes a top-level `volatile` qualifier from a type.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to transform.
     */
    template <typename T>
    struct remove_volatile {
        using type = typename Detail::remove_volatile_backend<T>::type;
    };

    /**
     * @brief Removes top-level `const` and `volatile` qualifiers.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to transform.
     */
    template <typename T>
    struct remove_cv {
        using type = typename Detail::remove_cv_backend<T>::type;
    };

    /** @brief Alias for `remove_const<T>::type`.
     * @ingroup Foundation_TypeTraits_Properties */
    template <typename T>
    using remove_const_t = typename remove_const<T>::type;

    /** @brief Alias for `remove_volatile<T>::type`.
     * @ingroup Foundation_TypeTraits_Properties */
    template <typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    /** @brief Alias for `remove_cv<T>::type`.
     * @ingroup Foundation_TypeTraits_Properties */
    template <typename T>
    using remove_cv_t = typename remove_cv<T>::type;

    /**
     * @brief Reports whether a type has a top-level `const` qualifier.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_const
        : bool_constant<Detail::is_const_backend<T>::value> {};

    /**
     * @brief Reports whether a type has a top-level `volatile` qualifier.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_volatile
        : bool_constant<Detail::is_volatile_backend<T>::value> {};

    /**
     * @brief Reports whether an arithmetic type is unsigned.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to inspect; top-level cv qualifiers are ignored.
     *
     * Non-arithmetic types, including `void`, classes, enumerations, pointers,
     * and floating-point types, report false.
     */
    template <typename T>
    struct is_unsigned
        : bool_constant<
            Detail::is_unsigned_backend<remove_cv_t<T>>::value
        > {};

    /** @brief `is_const<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Properties */
    template <typename T>
    inline constexpr bool is_const_v = is_const<T>::value;

    /** @brief `is_volatile<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Properties */
    template <typename T>
    inline constexpr bool is_volatile_v = is_volatile<T>::value;

    /** @brief `is_unsigned<T>::value` convenience variable.
     * @ingroup Foundation_TypeTraits_Properties */
    template <typename T>
    inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

}

#endif // FOUNDATION_TYPE_TRAITS_TYPE_PROPERTIES_H
