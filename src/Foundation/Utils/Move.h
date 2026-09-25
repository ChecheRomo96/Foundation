#ifndef FOUNDATION_UTILS_MOVE_H
#define FOUNDATION_UTILS_MOVE_H

    namespace Foundation::Utils {

        /// @cond INTERNAL
        namespace Detail {

            template<typename T>
            struct RemoveReference {
                typedef T Type;
            };

            template<typename T>
            struct RemoveReference<T&> {
                typedef T Type;
            };

            template<typename T>
            struct RemoveReference<T&&> {
                typedef T Type;
            };

        }
        /// @endcond

        /**
         * @brief Casts a value to an rvalue reference.
         * @ingroup Foundation_Utils
         * @tparam T Deduced source type.
         * @param value Value whose resources may be transferred by a later operation.
         * @return An rvalue reference with reference qualifiers removed.
         *
         * Move itself transfers no data and leaves `value` unchanged. The move
         * constructor or move assignment receiving the result defines the
         * transferred state and the valid state of the source afterward.
         */
        template <typename T>
        constexpr typename Detail::RemoveReference<T>::Type&&
        Move(T&& value) noexcept {
            return static_cast<typename Detail::RemoveReference<T>::Type&&>(value);
        }
    }

#endif
