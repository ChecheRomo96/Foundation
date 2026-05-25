#ifndef FOUNDATION_TYPE_TRAITS_PRIMARY_TYPE_CATEGORIES_H
#define FOUNDATION_TYPE_TRAITS_PRIMARY_TYPE_CATEGORIES_H

    #include <Foundation_BuildSettings.h>

    #if defined(FOUNDATION_USING_STL)
        #include <type_traits>
    #endif

    namespace Foundation::TypeTraits {

        #if defined(FOUNDATION_USING_STL)

            template <typename T>
            using is_array = std::is_array<T>;

            template <typename T>
            using is_class = std::is_class<T>;

            template <typename T>
            using is_enum = std::is_enum<T>;

            template <typename T>
            using is_floating_point = std::is_floating_point<T>;

            template <typename T>
            using is_function = std::is_function<T>;

            template <typename T>
            using is_integral = std::is_integral<T>;

            template <typename T>
            using is_lvalue_reference = std::is_lvalue_reference<T>;

            template <typename T>
            using is_member_function_pointer = std::is_member_function_pointer<T>;

            template <typename T>
            using is_member_object_pointer = std::is_member_object_pointer<T>;

            template <typename T>
            using is_pointer = std::is_pointer<T>;

            template <typename T>
            using is_rvalue_reference = std::is_rvalue_reference<T>;

            template <typename T>
            using is_union = std::is_union<T>;

            template <typename T>
            using is_void = std::is_void<T>;

        #else

            template <class T>
            struct is_array : false_type {};

            template <class T>
            struct is_array<T[]> : true_type {};

            template <class T, size_t N>
            struct is_array<T[N]> : true_type {};

            template <typename T>
            struct is_class : bool_constant<__is_class(T)> {};

            template <typename T>
            struct is_enum : bool_constant<__is_enum(T)> {};

            template <typename T>
            struct is_floating_point : bool_constant<
                is_same<typename remove_cv<T>::type, float>::value ||
                is_same<typename remove_cv<T>::type, double>::value ||
                is_same<typename remove_cv<T>::type, long double>::value
            > {};

            template <typename T>
            struct is_function : false_type {};

            template <typename Ret, typename... Args>
            struct is_function<Ret(Args...)> : true_type {};

            template <typename Ret, typename... Args>
            struct is_function<Ret(Args..., ...)> : true_type {};

            template <typename Ret, typename... Args>
            struct is_function<Ret(Args...) const> : true_type {};

            template <typename Ret, typename... Args>
            struct is_function<Ret(Args..., ...) const> : true_type {};

            template <typename Ret, typename... Args>
            struct is_function<Ret(Args...) volatile> : true_type {};

            template <typename Ret, typename... Args>
            struct is_function<Ret(Args..., ...) volatile> : true_type {};

            template <typename Ret, typename... Args>
            struct is_function<Ret(Args...) const volatile> : true_type {};

            template <typename Ret, typename... Args>
            struct is_function<Ret(Args..., ...) const volatile> : true_type {};

            template <typename T>
            struct is_integral : bool_constant<
                is_same<typename remove_cv<T>::type, bool>::value ||
                is_same<typename remove_cv<T>::type, char>::value ||
                is_same<typename remove_cv<T>::type, signed char>::value ||
                is_same<typename remove_cv<T>::type, unsigned char>::value ||
                is_same<typename remove_cv<T>::type, wchar_t>::value ||
                is_same<typename remove_cv<T>::type, short>::value ||
                is_same<typename remove_cv<T>::type, unsigned short>::value ||
                is_same<typename remove_cv<T>::type, int>::value ||
                is_same<typename remove_cv<T>::type, unsigned int>::value ||
                is_same<typename remove_cv<T>::type, long>::value ||
                is_same<typename remove_cv<T>::type, unsigned long>::value ||
                is_same<typename remove_cv<T>::type, long long>::value ||
                is_same<typename remove_cv<T>::type, unsigned long long>::value
            > {};

            template <typename T>
            struct is_lvalue_reference : false_type {};

            template <typename T>
            struct is_lvalue_reference<T&> : true_type {};

            template <typename T>
            struct is_member_function_pointer : false_type {};

            template <typename T, typename C>
            struct is_member_function_pointer<T C::*> : is_function<T> {};

            template <typename T>
            struct is_member_object_pointer : false_type {};

            template <typename T, typename C>
            struct is_member_object_pointer<T C::*>
                : bool_constant<!is_function<T>::value> {};

            template <typename T>
            struct is_pointer_helper : false_type {};

            template <typename T>
            struct is_pointer_helper<T*> : true_type {};

            template <typename T>
            struct is_pointer : is_pointer_helper<typename remove_cv<T>::type> {};

            template <typename T>
            struct is_rvalue_reference : false_type {};

            template <typename T>
            struct is_rvalue_reference<T&&> : true_type {};

            template <typename T>
            struct is_union : bool_constant<__is_union(T)> {};

            template <typename T>
            struct is_void : is_same<void, typename remove_cv<T>::type> {};

        #endif

        #if FOUNDATION_CPLUSPLUS >= 201402L

            template <typename T>
            constexpr bool is_array_v = is_array<T>::value;

            template <typename T>
            constexpr bool is_class_v = is_class<T>::value;

            template <typename T>
            constexpr bool is_enum_v = is_enum<T>::value;

            template <typename T>
            constexpr bool is_floating_point_v = is_floating_point<T>::value;

            template <typename T>
            constexpr bool is_function_v = is_function<T>::value;

            template <typename T>
            constexpr bool is_integral_v = is_integral<T>::value;

            template <typename T>
            constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

            template <typename T>
            constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

            template <typename T>
            constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

            template <typename T>
            constexpr bool is_pointer_v = is_pointer<T>::value;

            template <typename T>
            constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

            template <typename T>
            constexpr bool is_union_v = is_union<T>::value;

            template <typename T>
            constexpr bool is_void_v = is_void<T>::value;

        #endif

    }

#endif // FOUNDATION_TYPE_TRAITS_PRIMARY_TYPE_CATEGORIES_H