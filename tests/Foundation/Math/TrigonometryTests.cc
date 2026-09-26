#include <gtest/gtest.h>

#include <Foundation/Math/Trigonometry.h>

using Foundation::Math::DegToRad;
using Foundation::Math::Pi;
using Foundation::Math::RadToDeg;

TEST(TrigonometryTest, ExposesPiAndConstantAngleConversions) {
    static_assert(Pi > 3.14159f && Pi < 3.14160f);
    static_assert(DegToRad(0.0f) == 0.0f);
    static_assert(RadToDeg(0.0f) == 0.0f);

    constexpr float halfTurn = DegToRad(180.0f);
    constexpr float fullTurn = RadToDeg(2.0f * Pi);
    static_assert(halfTurn > 3.14159f && halfTurn < 3.14160f);
    static_assert(fullTurn > 359.999f && fullTurn < 360.001f);

    EXPECT_FLOAT_EQ(DegToRad(180.0f), Pi);
    EXPECT_FLOAT_EQ(RadToDeg(Pi), 180.0f);
}

TEST(TrigonometryTest, ConvertsPositiveNegativeAndUnwrappedAngles) {
    EXPECT_NEAR(DegToRad(90.0f), Pi / 2.0f, 0.000001f);
    EXPECT_NEAR(DegToRad(-90.0f), -Pi / 2.0f, 0.000001f);
    EXPECT_NEAR(DegToRad(720.0f), 4.0f * Pi, 0.000001f);

    EXPECT_NEAR(RadToDeg(Pi / 2.0f), 90.0f, 0.00001f);
    EXPECT_NEAR(RadToDeg(-Pi / 2.0f), -90.0f, 0.00001f);
    EXPECT_NEAR(RadToDeg(4.0f * Pi), 720.0f, 0.0001f);
}

TEST(TrigonometryTest, RoundTripsRepresentativeFiniteAngles) {
    const float angles[] = {
        -720.0f,
        -180.0f,
        -45.0f,
        0.0f,
        30.0f,
        90.0f,
        180.0f,
        360.0f,
        720.0f
    };

    for(const float angle : angles) {
        EXPECT_NEAR(RadToDeg(DegToRad(angle)), angle, 0.0001f)
            << "angle=" << angle;
    }
}
