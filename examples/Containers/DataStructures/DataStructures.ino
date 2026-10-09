#include <Foundation.h>
#include "Shared.h"

/*
  Goal: Fixed-capacity circular buffering and compact boolean storage.
  Interfaces: Foundation::Containers::CircularBuffer<T> and Foundation::Containers::BitVector.
  Observe: The trace contrasts caller-owned storage with BitVector-owned storage.
*/

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Containers::DataStructures::Result result =
        FoundationExamples::Containers::DataStructures::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Containers / DataStructures");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Show the containers std does not provide: a circular FIFO");
    Serial.println("  over caller storage and packed bits over an external");
    Serial.println("  buffer or an owned cpstd::vector.");
    Serial.println();
    Serial.println("[1] CIRCULAR BUFFER / WRAPAROUND");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    circularBuffer.Push(1);");
    Serial.println("    circularBuffer.Pop(first);");
    Serial.println("    circularBuffer.Push(4);");
    Serial.println("  Scenario ............ Push [1, 2, 3], pop 1, push 4, drain");
    Serial.print("  First removed ....... ");
    Serial.println(result.CircularBufferFirst);
    Serial.print("  Last removed ........ ");
    Serial.println(result.CircularBufferLast);
    Serial.print("  Empty after drain ... ");
    Serial.println(result.CircularBufferEmpty ? "yes" : "no");
    Serial.println();
    Serial.println("[2] BITVECTOR / EXTERNAL BUFFER");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    BitVector bits(storage, sizeof storage);  // non-null: external");
    Serial.println("    bits.PushBack(step == 'x');    // x..x..x. over one byte");
    Serial.print("  First byte .......... 0x");
    Serial.println(result.BitVectorFirstByte, HEX);
    Serial.print("  Bits set ............ ");
    Serial.println(result.BitVectorOnes);
    Serial.print("  Ninth bit refused ... ");
    Serial.println(result.ExternalBitsFull ? "yes, PushBack returned false" : "no");
    Serial.println();
    Serial.println("[3] BITVECTOR / OWNED STORAGE");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    BitVector owned;               // cpstd::vector<uint8_t>");
    Serial.println("    owned.PushBack(i % 3 == 0);    // twelve times");
    Serial.print("  Bits / bytes ........ ");
    Serial.print(result.OwnedBitCount);
    Serial.print(" / ");
    Serial.println(result.OwnedByteCount);
    Serial.print("  Owns storage ........ ");
    Serial.println(result.OwnedBitsOwnStorage ? "yes" : "no");
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  CircularBuffer removes the oldest value and never allocates.");
    Serial.println("  BitVector stays inside a buffer you provide, or grows on the");
    Serial.println("  heap through cpstd::vector. For vectors, stacks and queues");
    Serial.println("  use cpstd (CPSTL).");
    Serial.println("============================================================");
}

void loop() {
}
