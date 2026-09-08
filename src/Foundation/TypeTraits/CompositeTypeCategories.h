#ifndef FOUNDATION_TYPE_TRAITS_COMPOSITE_TYPE_CATEGORIES_H
#define FOUNDATION_TYPE_TRAITS_COMPOSITE_TYPE_CATEGORIES_H

#include <Foundation_BuildSettings.h>
#include "HelperClasses.h"
#include "TypeProperties.h"
#include "TypeRelationships.h"
#include "PrimaryTypeCategories.h"

#if defined(FOUNDATION_USING_STL)
    #include <type_traits>
#endif

namespace Foundation::TypeTraits {

#if defined(FOUNDATION_USING_STL)

    template <typename T> using is_reference = std::is_reference<T>;
    template <typename T> using is_member_pointer = std::is_member_pointer<T>;
    template <typename T> using is_object = std::is_object<T>;
    template <typename T> using is_scalar = std::is_scalar<T>;
    template <typename T> using is_fundamental = std::is_fundamental<T>;
    template <typename T> using is_compound = std::is_compound<T>;
    template <typename T> using is_arithmetic = std::is_arithmetic<T>;

#else

    template <typename T>
    struct is_reference : bool_constant<
        is_lvalue_reference<T>::value ||
        is_rvalue_reference<T>::value
    > {};

    template <typename T>
    struct is_member_pointer : bool_constant<
        is_member_object_pointer<T>::value ||
        is_member_function_pointer<T>::value
    > {};

    template <typename T>
    struct is_object : bool_constant<
        !is_function<T>::value &&
        !is_void<T>::value &&
        !is_reference<T>::value
    > {};

    template <typename T>
    struct is_arithmetic : bool_constant<
        is_integral<T>::value ||
        is_floating_point<T>::value
    > {};

    template <typename T>
    struct is_scalar : bool_constant<
        is_arithmetic<T>::value ||
        is_pointer<T>::value ||
        is_member_pointer<T>::value ||
        is_enum<T>::value ||
        is_same<decltype(nullptr), typename remove_cv<T>::type>::value
    > {};

    template <typename T>
    struct is_fundamental : bool_constant<
        is_integral<T>::value ||
        is_floating_point<T>::value ||
        is_void<T>::value ||
        is_same<decltype(nullptr), typename remove_cv<T>::type>::value
    > {};

    template <typename T>
    struct is_compound : bool_constant<
        !is_fundamental<T>::value
    > {};

#endif

#if FOUNDATION_CPLUSPLUS >= 201402L

    template <typename T>
    INLINE_MACRO constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

    template <typename T>
    INLINE_MACRO constexpr bool is_fundamental_v = is_fundamental<T>::value;

    template <typename T>
    INLINE_MACRO constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

    template <typename T>
    INLINE_MACRO constexpr bool is_object_v = is_object<T>::value;

    template <typename T>
    INLINE_MACRO constexpr bool is_reference_v = is_reference<T>::value;

    template <typename T>
    INLINE_MACRO constexpr bool is_scalar_v = is_scalar<T>::value;

    template <typename T>
    INLINE_MACRO constexpr bool is_compound_v = is_compound<T>::value;

#endif

}

#endif // FOUNDATION_TYPE_TRAITS_COMPOSITE_TYPE_CATEGORIES_H
