#ifndef FOUNDATION_TYPE_TRAITS_HELPER_CLASSES_H
#define FOUNDATION_TYPE_TRAITS_HELPER_CLASSES_H

    #include <Foundation_BuildSettings.h>

    /*
     * Prefer the standard C++17 traits backend when it is complete enough for
     * Foundation's supported surface. Toolchains without that library support
     * transparently use the local fallback. The force macro is internal and is
     * used by the conformance suite to compile the fallback on desktop hosts.
     */
    #if defined(FOUNDATION_DETAIL_FORCE_CUSTOM_TYPE_TRAITS)
        #define FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS 0
    #elif defined(__has_include)
        #if __has_include(<type_traits>)
            #include <type_traits>
            #if defined(__cpp_lib_is_invocable) && \
                (__cpp_lib_is_invocable >= 201703L)
                #define FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS 1
            #else
                #define FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS 0
            #endif
        #else
            #define FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS 0
        #endif
    #else
        #define FOUNDATION_DETAIL_HAS_STD_TYPE_TRAITS 0
    #endif

    namespace Foundation::TypeTraits {

            /**
             * @brief Wraps a typed compile-time constant.
             * @ingroup Foundation_TypeTraits_Helpers
             * @tparam T Type of the stored constant.
             * @tparam v Constant value.
             *
             * Provides `value`, `value_type`, and `type`, plus constexpr
             * conversion and call operators.
             */
            template <class T, T v>
            struct integral_constant {
                static constexpr T value = v;
                using value_type = T;
                using type = integral_constant;

                constexpr operator T() const noexcept { return v; }
                constexpr T operator()() const noexcept { return v; }
            };

            /**
             * @brief Boolean specialization of integral_constant.
             * @ingroup Foundation_TypeTraits_Helpers
             * @tparam B Boolean value represented by the type.
             */
            template< bool B >
            using bool_constant = integral_constant<bool, B>;

            /**
             * @brief Compile-time boolean true.
             * @ingroup Foundation_TypeTraits_Helpers
             */
            using true_type = integral_constant<bool, true>;

            /**
             * @brief Compile-time boolean false.
             * @ingroup Foundation_TypeTraits_Helpers
             */
            using false_type = integral_constant<bool, false>;
    }

#endif//FOUNDATION_TYPE_TRAITS_HELPER_CLASSES_H
