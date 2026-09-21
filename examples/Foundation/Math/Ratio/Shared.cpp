#include "Shared.h"

namespace FoundationExamples {
    namespace Math {
        namespace Ratio {

            void Run(Printer printer) {
                if (printer == nullptr) {
                    return;
                }

                const Foundation::Math::Ratio half(1, 2);
                const Foundation::Math::Ratio audioRate(48000, 1);
                const Foundation::Math::Ratio ntsc(30000, 1001);

                printer("Ratio half(1, 2)", half);
                printer("Ratio audioRate(48000, 1)", audioRate);
                printer("Ratio ntsc(30000, 1001)", ntsc);
            }

        }
    }
}
