#include "Shared.h"

namespace FoundationExamples {
namespace Containers {
namespace DataStructures {

    Result Run() {
        int circularStorage[3] = {};
        Foundation::Containers::CircularBuffer<int> circularBuffer(
            circularStorage,
            3
        );

        circularBuffer.Push(1);
        circularBuffer.Push(2);
        circularBuffer.Push(3);

        int circularFirst = 0;
        circularBuffer.Pop(circularFirst);
        circularBuffer.Push(4);

        int discarded = 0;
        int circularLast = 0;
        circularBuffer.Pop(discarded);
        circularBuffer.Pop(discarded);
        circularBuffer.Pop(circularLast);

        // BitVector over an external buffer: eight steps per byte, here the
        // tresillo x..x..x. The buffer never grows, so a ninth step is refused.
        uint8_t bitStorage[Foundation::Containers::BitVector::BytesFor(8)] = {};
        Foundation::Containers::BitVector bits(bitStorage, sizeof bitStorage);
        const char* tresillo = "x..x..x.";
        for (const char* step = tresillo; *step != '\0'; ++step) {
            bits.PushBack(*step == 'x');
        }
        const bool externalFull = !bits.PushBack(true);

        // BitVector with owned storage: a cpstd::vector<uint8_t> that grows.
        Foundation::Containers::BitVector owned;
        for (int i = 0; i < 12; ++i) {
            owned.PushBack(i % 3 == 0);
        }

        Result result;
        result.CircularBufferFirst = circularFirst;
        result.CircularBufferLast = circularLast;
        result.CircularBufferEmpty = circularBuffer.IsEmpty();
        result.BitVectorFirstByte = bitStorage[0];
        result.BitVectorOnes = static_cast<unsigned int>(bits.CountOnes());
        result.ExternalBitsFull = externalFull;
        result.OwnedBitCount = static_cast<unsigned int>(owned.GetCount());
        result.OwnedByteCount = static_cast<unsigned int>(owned.GetByteCount());
        result.OwnedBitsOwnStorage = owned.OwnsStorage();
        return result;
    }

} // namespace DataStructures
} // namespace Containers
} // namespace FoundationExamples
