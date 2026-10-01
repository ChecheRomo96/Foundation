#include <gtest/gtest.h>

#include <stdint.h>

#include <Foundation/Utils/Flash.h>

namespace {

struct Entry {
    uint8_t id;
    uint16_t mask;
};

const Entry Entries[] FOUNDATION_FLASH = {{3, 0x7B56}, {9, 0x5ABC}};
const char Name[] FOUNDATION_FLASH = "Dorian";

}

TEST(FoundationUtilsFlash, ReadReturnsACopyOfEachEntry) {
    const Entry second = Foundation::Utils::Flash::Read(&Entries[1]);
    EXPECT_EQ(second.id, 9);
    EXPECT_EQ(second.mask, 0x5ABC);
    EXPECT_EQ(Foundation::Utils::Flash::Read(&Entries[0].mask), 0x7B56);
}

TEST(FoundationUtilsFlash, CopyTransfersTheRequestedBytes) {
    uint8_t bytes[sizeof(Entries)] = {};
    Foundation::Utils::Flash::Copy(bytes, Entries, sizeof(Entries));
    Entry copy[2];
    Foundation::Utils::Flash::Copy(copy, bytes, sizeof(copy));
    EXPECT_EQ(copy[0].id, 3);
    EXPECT_EQ(copy[1].mask, 0x5ABC);
}

TEST(FoundationUtilsFlash, CopyStringFitsAndTerminates) {
    char buffer[16];
    EXPECT_EQ(Foundation::Utils::Flash::StringLength(Name), 6u);
    EXPECT_EQ(Foundation::Utils::Flash::CopyString(buffer, sizeof(buffer), Name), 6u);
    EXPECT_STREQ(buffer, "Dorian");
}

TEST(FoundationUtilsFlash, CopyStringTruncatesLikeSnprintf) {
    char buffer[4] = {'x', 'x', 'x', 'x'};
    EXPECT_EQ(Foundation::Utils::Flash::CopyString(buffer, sizeof(buffer), Name), 6u);
    EXPECT_STREQ(buffer, "Dor");

    char untouched = 'x';
    EXPECT_EQ(Foundation::Utils::Flash::CopyString(&untouched, 0, Name), 6u);
    EXPECT_EQ(untouched, 'x');
    EXPECT_EQ(Foundation::Utils::Flash::CopyString(nullptr, 0, Name), 6u);

    char single = 'x';
    EXPECT_EQ(Foundation::Utils::Flash::CopyString(&single, 1, Name), 6u);
    EXPECT_EQ(single, '\0');
}
