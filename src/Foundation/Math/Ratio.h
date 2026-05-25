#ifndef FOUNDATION_MATH_RATIO_H
#define FOUNDATION_MATH_RATIO_H

    #include <stdint.h>
    #include <Foundation/Math/Arithmetic.h>

    namespace Foundation{
        namespace Math{

            class Ratio {
                private:
                    uint32_t _num;
                    uint32_t _den;
                    int8_t _sign;

                public:

                    constexpr Ratio(
                        int32_t num = 0,
                        int32_t den = 1
                    )
                        : _num(0),
                        _den(1),
                        _sign(1)
                    {
                        Set(num, den);
                    }

                    constexpr uint32_t Num() const {
                        return _num;
                    }

                    constexpr uint32_t Den() const {
                        return _den;
                    }

                    constexpr int8_t Sign() const {
                        return _sign;
                    }

                    void SetNum(uint32_t num) {
                        _num = num;
                    }

                    void SetDen(uint32_t den) {
                        _den = den;
                    }

                    void Set(
                        int32_t num,
                        int32_t den
                    ) {
                        if (num < 0) {
                            _sign = -_sign;
                            num = -num;
                        }

                        if (den < 0) {
                            _sign = -_sign;
                            den = -den;
                        }

                        _num = static_cast<uint32_t>(num);
                        _den = static_cast<uint32_t>(den);
                    }

                    constexpr bool IsValid() const {
                        return (_num > 0) &&
                            (_den > 0);
                    }

                    constexpr float ToFloat() const {
                        return IsValid()
                            ? static_cast<float>(_sign) *
                                static_cast<float>(_num) /
                                static_cast<float>(_den)
                            : 0.0f;
                    }

                    Ratio Reduced() const {
                        if (!IsValid()) {
                            return Ratio(0, 1);
                        }

                        uint32_t gcd = Math::GCD(_num, _den);

                        Ratio result(
                            static_cast<int32_t>(_num / gcd),
                            static_cast<int32_t>(_den / gcd)
                        );

                        result._sign = _sign;

                        return result;
                    }

                    void Reduce() {
                        if (!IsValid()) {
                            _num = 0;
                            _den = 1;
                            return;
                        }
                        uint32_t gcd = Math::GCD(_num, _den);
                        _num /= gcd;
                        _den /= gcd;
                    }
                
            };
        }
    }

#endif