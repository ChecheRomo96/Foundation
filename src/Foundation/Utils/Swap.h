#ifndef FOUNDATION_SWAP_H
#define FOUNDATION_SWAP_H

    #include <Foundation/Utils/Move.h>

    namespace Foundation::Utils {

        template <typename T>
        void Swap(T& a, T& b) {
            T temp = Foundation::Utils::Move(a);
            a = Foundation::Utils::Move(b);
            b = Foundation::Utils::Move(temp);
        }

    }

#endif
