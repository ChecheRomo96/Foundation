#ifndef FOUNDATION_MATH_RATIO_H
#define FOUNDATION_MATH_RATIO_H

    #include <stdint.h>
    #include <Foundation_BuildSettings.h>
    #include <Foundation/Math/Arithmetic.h>

    namespace Foundation {
        namespace Math {

            /** @cond */
            namespace Detail {

                template <typename T>
                struct RatioRepresentationTraits {
                    static constexpr bool IsSupported = false;
                    static constexpr bool IsSigned = false;
                };

                template <>
                struct RatioRepresentationTraits<int32_t> {
                    static constexpr bool IsSupported = true;
                    static constexpr bool IsSigned = true;

                    static constexpr uint32_t Magnitude(int32_t value) noexcept {
                        return (value < 0)
                            ? 0u - static_cast<uint32_t>(value)
                            : static_cast<uint32_t>(value);
                    }

                    static constexpr int8_t Sign(int32_t num, int32_t den) noexcept {
                        return static_cast<int8_t>(
                            ((num > 0) ? 1 : (num < 0) ? -1 : 0) *
                            ((den > 0) ? 1 : (den < 0) ? -1 : 0)
                        );
                    }
                };

                template <>
                struct RatioRepresentationTraits<uint32_t> {
                    static constexpr bool IsSupported = true;
                    static constexpr bool IsSigned = false;

                    static constexpr uint32_t Magnitude(uint32_t value) noexcept {
                        return value;
                    }

                    static constexpr int8_t Sign(uint32_t num, uint32_t den) noexcept {
                        return (num == 0 || den == 0) ? 0 : 1;
                    }
                };

            }
            /** @endcond */

            /**
             * @brief Exact rational value backed by a supported 32-bit integer.
             * @ingroup Foundation_Math_Ratio
             * @tparam T `int32_t` for signed ratios or `uint32_t` for unsigned
             * ratios.
             *
             * A zero denominator is the only invalid BasicRatio state. A zero
             * numerator remains a valid mathematical ratio. Invalid conversion
             * returns zero, and reduction canonicalizes an invalid ratio to
             * `0 / 1`.
             */
            template <typename T>
            class BasicRatio {
                static_assert(
                    Detail::RatioRepresentationTraits<T>::IsSupported,
                    "BasicRatio supports only int32_t and uint32_t"
                );

            public:
                /** @brief Integer representation used by both terms. */
                using Representation = T;

            private:
                Representation _num;
                Representation _den;

                static constexpr uint32_t AbsoluteMagnitude(
                    Representation value
                ) noexcept {
                    return Detail::RatioRepresentationTraits<T>::Magnitude(value);
                }

                constexpr BasicRatio ReducedBy(uint32_t gcd) const noexcept {
                    return BasicRatio(
                        static_cast<Representation>(
                            static_cast<int64_t>(_num) / gcd
                        ),
                        static_cast<Representation>(
                            static_cast<int64_t>(_den) / gcd
                        )
                    );
                }

            public:

                /** @brief Creates the exact ratio `num / den`. */
                constexpr BasicRatio(
                    Representation num = 0,
                    Representation den = 1
                ) noexcept
                    : _num(num),
                    _den(den) {}

                /** @brief Returns the stored numerator. */
                constexpr Representation Numerator() const noexcept {
                    return _num;
                }

                /** @brief Returns the stored denominator. */
                constexpr Representation Denominator() const noexcept {
                    return _den;
                }

                /** @brief Replaces the numerator without reducing the ratio. */
                FOUNDATION_CONSTEXPR14 void SetNumerator(Representation num) noexcept {
                    _num = num;
                }

                /** @brief Replaces the denominator without reducing the ratio. */
                FOUNDATION_CONSTEXPR14 void SetDenominator(Representation den) noexcept {
                    _den = den;
                }

                /** @brief Replaces both terms without reducing the ratio. */
                FOUNDATION_CONSTEXPR14 void Set(
                    Representation num,
                    Representation den
                ) noexcept {
                    _num = num;
                    _den = den;
                }

                /** @brief Returns `-1`, `0`, or `1` according to the ratio sign. */
                constexpr int8_t Sign() const noexcept {
                    return Detail::RatioRepresentationTraits<T>::Sign(_num, _den);
                }

                /** @brief Reports whether the denominator is non-zero. */
                constexpr bool IsValid() const noexcept {
                    return _den != 0;
                }

                /** @brief Converts the ratio to float, or zero when invalid. */
                constexpr float ToFloat() const noexcept {
                    return IsValid()
                        ? static_cast<float>(_num) /
                          static_cast<float>(_den)
                        : 0.0f;
                }

                /**
                 * @brief Returns a reduced copy.
                 * @return `0 / 1` when this ratio is invalid.
                 */
                constexpr BasicRatio Reduced() const noexcept {
                    return IsValid()
                        ? ReducedBy(
                            Math::GCD(
                                AbsoluteMagnitude(_num),
                                AbsoluteMagnitude(_den)
                            )
                        )
                        : BasicRatio(0, 1);
                }

                /**
                 * @brief Reduces this ratio in place.
                 * @post An invalid ratio becomes the canonical `0 / 1` value.
                 */
                FOUNDATION_CONSTEXPR14 void Reduce() noexcept {
                    *this = Reduced();
                }
            };

            /** @brief Signed 32-bit mathematical ratio. */
            using Ratio = BasicRatio<int32_t>;

            /** @brief Unsigned 32-bit ratio for non-negative quantities. */
            using UnsignedRatio = BasicRatio<uint32_t>;

        }
    }

#endif
