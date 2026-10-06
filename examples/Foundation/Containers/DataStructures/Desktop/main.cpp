#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::Containers::DataStructures::Result result =
        FoundationExamples::Containers::DataStructures::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Containers / DataStructures\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Show the containers std does not provide: a circular FIFO\n"
        << "  over caller storage and packed bits over an external\n"
        << "  buffer or an owned cpstd::vector.\n\n"
        << "[1] CIRCULAR BUFFER / WRAPAROUND\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    circularBuffer.Push(1);\n"
        << "    circularBuffer.Pop(first);\n"
        << "    circularBuffer.Push(4);\n"
        << "  Scenario ............ Push [1, 2, 3], pop 1, push 4, drain\n"
        << "  First removed ....... " << result.CircularBufferFirst << '\n'
        << "  Last removed ........ " << result.CircularBufferLast << '\n'
        << "  Empty after drain ... "
        << (result.CircularBufferEmpty ? "yes" : "no") << "\n\n"
        << "[2] BITVECTOR / EXTERNAL BUFFER\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    BitVector bits(storage, sizeof storage);  // non-null: external\n"
        << "    bits.PushBack(step == 'x');    // x..x..x. over one byte\n"
        << "  First byte .......... 0x" << std::hex << result.BitVectorFirstByte
        << std::dec << '\n'
        << "  Bits set ............ " << result.BitVectorOnes << '\n'
        << "  Ninth bit refused ... "
        << (result.ExternalBitsFull ? "yes, PushBack returned false" : "no")
        << "\n\n"
        << "[3] BITVECTOR / OWNED STORAGE\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    BitVector owned;               // cpstd::vector<uint8_t>\n"
        << "    owned.PushBack(i % 3 == 0);    // twelve times\n"
        << "  Bits / bytes ........ " << result.OwnedBitCount << " / "
        << result.OwnedByteCount << '\n'
        << "  Owns storage ........ "
        << (result.OwnedBitsOwnStorage ? "yes" : "no") << "\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  CircularBuffer removes the oldest value and never allocates.\n"
        << "  BitVector stays inside a buffer you provide, or grows on the\n"
        << "  heap through cpstd::vector. For vectors, stacks and queues\n"
        << "  use cpstd (CPSTL).\n"
        << "============================================================\n";

    return result.CircularBufferFirst == 1
        && result.CircularBufferLast == 4
        && result.CircularBufferEmpty
        && result.BitVectorFirstByte == 0x49
        && result.BitVectorOnes == 3
        && result.ExternalBitsFull
        && result.OwnedBitCount == 12
        && result.OwnedByteCount == 2
        && result.OwnedBitsOwnStorage
        ? 0
        : 1;
}
