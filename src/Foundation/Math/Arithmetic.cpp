#include "Arithmetic.h"

namespace Foundation {
namespace Math {
    
    uint32_t GCD(uint32_t a, uint32_t b) {
        while(b != 0) {
            uint32_t temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

}
}