#ifndef FOUNDATION_TYPE_TRAITS_TYPE_PROPERTIES_H
#define FOUNDATION_TYPE_TRAITS_TYPE_PROPERTIES_H

#include <Foundation_BuildSettings.h>
#include "HelperClasses.h"

#if defined(FOUNDATION_USING_STL)
    #include <type_traits>
#endif

namespace Foundation::TypeTraits {

#if defined(FOUNDATION_USING_STL)

    template <typename T> using is_abstract = std::is_abstract<T>;
    template <typename T> using is_const = std::is_const<T>;
    template <typename T> using is_empty = std::is_empty<T>;
    template <typename T> using is_pod = std::is_pod<T>;
    template <typename T> using is_polymorphic = std::is_polymorphic<T>;
    template <typename T> using is_standard_layout = std::is_standard_layout<T>;
    template <typename T> using is_trivial = std::is_trivial<T>;
    template <typename T> using is_trivially_copyable = std::is_trivially_copyable<T>;
    template <typename T> using is_unsigned = std::is_unsigned<T>;
    template <typename T> using is_volatile = std::is_volatile<T>;

#else

    /**
     * @brief Removes a top-level `const` qualifier from a type.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to transform.
     */
    template <typename T>
    struct remove_const {
        using type = T;
    };

    template <typename T>
    struct remove_const<const T> {
        using type = T;
    };

    /**
     * @brief Removes a top-level `volatile` qualifier from a type.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to transform.
     */
    template <typename T>
    struct remove_volatile {
        using type = T;
    };

    template <typename T>
    struct remove_volatile<volatile T> {
        using type = T;
    };

    /**
     * @brief Removes top-level `const` and `volatile` qualifiers.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to transform.
     */
    template <typename T>
    struct remove_cv {
        using type = typename remove_volatile<typename remove_const<T>::type>::type;
    };

    /**
     * @brief Reports whether a type has a top-level `const` qualifier.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_const : false_type {};

    template <typename T>
    struct is_const<const T> : true_type {};

    /**
     * @brief Reports whether a type has a top-level `volatile` qualifier.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Type to inspect.
     */
    template <typename T>
    struct is_volatile : false_type {};

    template <typename T>
    struct is_volatile<volatile T> : true_type {};

    /**
     * @brief Reports whether an arithmetic type is unsigned.
     * @ingroup Foundation_TypeTraits_Properties
     * @tparam T Arithmetic type to inspect.
     * @warning The pre-1.0 custom implementation is not yet constrained for
     * non-arithmetic types; see API-007 in the public API audit.
     */
    template <typename T>
    struct is_unsigned : bool_constant<(T(0) < T(-1))> {};

#endif

}

#endif // FOUNDATION_TYPE_TRAITS_TYPE_PROPERTIES_H
