#ifndef FOUNDATION_MATH_ARITHMETIC_H
#define FOUNDATION_MATH_ARITHMETIC_H

    #include <stdint.h>

    namespace Foundation {
        namespace Math {

            constexpr uint32_t GCD(
                uint32_t a,
                uint32_t b
            ) {
                while (b != 0) {

                    uint32_t temp = b;

                    b = a % b;

                    a = temp;
                }

                return a;
            }

        }
    }

#endif