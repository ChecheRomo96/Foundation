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

                printer(
                    "[1] SIMPLE FRACTION / ONE HALF",
                    "Foundation::Math::Ratio(1, 2);",
                    half
                );
                printer(
                    "[2] INTEGER RATIO / 48,000 AUDIO SAMPLES PER SECOND",
                    "Foundation::Math::Ratio(48000, 1);",
                    audioRate
                );
                printer(
                    "[3] FRACTIONAL RATE / NTSC 30,000 DIVIDED BY 1,001",
                    "Foundation::Math::Ratio(30000, 1001);",
                    ntsc
                );
            }

        }
    }
}
