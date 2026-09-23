#ifndef FOUNDATION_TYPE_TRAITS_HELPER_CLASSES_H
#define FOUNDATION_TYPE_TRAITS_HELPER_CLASSES_H

    #include <Foundation_BuildSettings.h>
    
    #if defined(FOUNDATION_USING_STL)
        #include <type_traits>
    #endif

    namespace Foundation::TypeTraits {
    
        #if defined(FOUNDATION_USING_STL)

            template <typename T, T v>
            using integral_constant = std::integral_constant<T, v>;

            template< bool B >
            using bool_constant = integral_constant<bool, B>;

            using true_type = integral_constant<bool, true>;
            using false_type = integral_constant<bool, false>;

        #else

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

        #endif
    }

#endif//FOUNDATION_TYPE_TRAITS_HELPER_CLASSES_H
