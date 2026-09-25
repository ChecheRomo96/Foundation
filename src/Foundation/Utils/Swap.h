#ifndef FOUNDATION_SWAP_H
#define FOUNDATION_SWAP_H

    #include <Foundation/Utils/Move.h>

    namespace Foundation::Utils {

        /**
         * @brief Exchanges two values using move construction and assignment.
         * @ingroup Foundation_Utils
         * @tparam T Value type; must support compatible move operations.
         * @param a First value, replaced by the original value of `b`.
         * @param b Second value, replaced by the original value of `a`.
         */
        template <typename T>
        void Swap(T& a, T& b) {
            T temp = Foundation::Utils::Move(a);
            a = Foundation::Utils::Move(b);
            b = Foundation::Utils::Move(temp);
        }

    }

#endif
