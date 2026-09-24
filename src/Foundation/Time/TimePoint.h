#ifndef FOUNDATION_TIME_TIMEPOINT_H
#define FOUNDATION_TIME_TIMEPOINT_H

    #include <Foundation/Time/Tick.h>
    #include <Foundation/Time/Duration.h>

    namespace Foundation {
        namespace Time {

            template <typename TickType>
            class BasicClock;

            /**
             * @brief Tick value associated with the identity of one Clock.
             * @ingroup Foundation_Time_TimePoint
             * @tparam TickType Tick specialization shared with the Clock.
             *
             * The Clock pointer is non-owning and is used for compatibility
             * checks. Valid points can only be created by their Clock through
             * `Now()` or `At()`, and that Clock must outlive every operation
             * that inspects the point.
             */
            template <typename TickType = Tick32>
            class BasicTimePoint {
            public:
                using Representation = typename TickType::Representation;
                using DurationType = BasicDuration<TickType>;
                using ClockType = BasicClock<TickType>;

            private:
                Representation _ticks;
                const ClockType* _clock;

                friend class BasicClock<TickType>;

                constexpr BasicTimePoint(
                    Representation ticks,
                    const ClockType* clock
                )
                    : _ticks(ticks),
                    _clock(clock) {}

                bool IsComparable(const BasicTimePoint& rhs) const {
                    return IsValid() &&
                        rhs.IsValid() &&
                        (_clock == rhs._clock);
                }

            public:

                /** @brief Creates an invalid point with no Clock identity. */
                constexpr BasicTimePoint()
                    : _ticks(0),
                    _clock(nullptr) {}

                /** @brief Returns the stored tick value. */
                constexpr Representation Ticks() const {
                    return _ticks;
                }

                /** @brief Returns the non-owning Clock identity, which may be null. */
                const ClockType* GetClock() const {
                    return _clock;
                }

                /** @brief Reports whether this point has a non-null Clock identity. */
                bool IsValid() const {
                    return _clock != nullptr;
                }

                /**
                 * @brief Returns the modular elapsed duration from `rhs` to this point.
                 * @return Invalid Duration for incompatible clocks, invalid points,
                 * reverse ordering, or the ambiguous half-range distance.
                 */
                DurationType operator-(const BasicTimePoint& rhs) const {
                    if (!IsComparable(rhs)) {
                        return DurationType::Invalid();
                    }

                    const Representation difference =
                        static_cast<Representation>(_ticks - rhs._ticks);

                    if (difference >= TickType::HalfRange()) {
                        return DurationType::Invalid();
                    }

                    return DurationType(difference);
                }

                /** @brief Adds a valid Duration while preserving Clock identity. */
                BasicTimePoint operator+(const DurationType& duration) const {
                    if (!IsValid() || !duration.IsValid()) {
                        return BasicTimePoint();
                    }

                    return BasicTimePoint(
                        static_cast<Representation>(
                            _ticks + duration.Ticks()
                        ),
                        _clock
                    );
                }

                /**
                 * @brief Subtracts a valid Duration while preserving Clock identity.
                 */
                BasicTimePoint operator-(const DurationType& duration) const {
                    if (!IsValid() || !duration.IsValid()) {
                        return BasicTimePoint();
                    }

                    return BasicTimePoint(
                        static_cast<Representation>(
                            _ticks - duration.Ticks()
                        ),
                        _clock
                    );
                }

                /** @brief Reports whether two points store the same Clock pointer. */
                bool SameClock(const BasicTimePoint& rhs) const {
                    return _clock == rhs._clock;
                }

                /** @brief Compares Clock identity and tick value for equality. */
                bool operator==(const BasicTimePoint& rhs) const {
                    return (_clock == rhs._clock) &&
                        (_ticks == rhs._ticks);
                }

                /** @brief Negates equality. */
                bool operator!=(const BasicTimePoint& rhs) const {
                    return !(*this == rhs);
                }

                /** @brief Orders compatible points within the modular half-range. */
                bool operator<(const BasicTimePoint& rhs) const {
                    if (!IsComparable(rhs)) {
                        return false;
                    }

                    const Representation forward =
                        static_cast<Representation>(rhs._ticks - _ticks);
                    return forward != 0 && forward < TickType::HalfRange();
                }

                /** @brief Orders compatible points within the modular half-range. */
                bool operator>(const BasicTimePoint& rhs) const {
                    return rhs < *this;
                }

                /** @brief Orders compatible points within the modular half-range. */
                bool operator<=(const BasicTimePoint& rhs) const {
                    return IsComparable(rhs) &&
                        ((*this == rhs) || (*this < rhs));
                }

                /** @brief Orders compatible points within the modular half-range. */
                bool operator>=(const BasicTimePoint& rhs) const {
                    return IsComparable(rhs) &&
                        ((*this == rhs) || (*this > rhs));
                }
            };

            /** @brief Default 32-bit TimePoint specialization. */
            using TimePoint = BasicTimePoint<>;
        }
    }

#endif
