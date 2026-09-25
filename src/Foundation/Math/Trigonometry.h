#ifndef FOUNDATION_MATH_TRIGONOMETRY_H
#define FOUNDATION_MATH_TRIGONOMETRY_H

/**
 * @file Foundation/Math/Trigonometry.h
 * @brief Single-precision angle constants and unit conversions.
 */

namespace Foundation {
    namespace Math {

        /**
         * @brief Single-precision value of the mathematical constant pi.
         * @ingroup Foundation_Math
         *
         * The literal is rounded to the nearest `float`.
         */
        constexpr float Pi = 3.14159265358979323846f;

        /**
         * @brief Converts an angle from degrees to radians.
         * @ingroup Foundation_Math
         * @param degrees Angle in degrees. Any finite value is accepted; the
         * angle is not wrapped into a principal range.
         * @return The same angle in radians, computed in single precision.
         */
        constexpr float DegToRad(
            float degrees
        ) {
            return degrees * (Pi / 180.0f);
        }

        /**
         * @brief Converts an angle from radians to degrees.
         * @ingroup Foundation_Math
         * @param radians Angle in radians. Any finite value is accepted; the
         * angle is not wrapped into a principal range.
         * @return The same angle in degrees, computed in single precision.
         */
        constexpr float RadToDeg(
            float radians
        ) {
            return radians * (180.0f / Pi);
        }

    }
}

#endif