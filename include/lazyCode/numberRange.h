
#ifndef LAZYCODE_NUMBERRANGE_H_
#define LAZYCODE_NUMBERRANGE_H_
#include "rangeBase.h"
namespace LazyCode {

template <typename Child>
class NumberRangeBase : public RangeBase {
    inline auto& thisChild() { return static_cast<Child&>(*this); }

   public:
    inline bool hasValue() { return thisChild().first < thisChild().last; }
    inline void moveNext() { thisChild().first += thisChild().increment; }
    inline auto& getValue() { return thisChild().first; }

    inline auto begin() { return RangeIterator<Child>(thisChild()); }
    inline auto end() { return RangeIterator<Child>(thisChild()); }
};

template <typename Number>
struct StandardNumberRange
    : public NumberRangeBase<StandardNumberRange<Number>> {
    friend class NumberRangeBase<StandardNumberRange<Number>>;
    Number first;
    Number last;
    static constexpr Number increment = 1;
    StandardNumberRange(Number first, Number last) : first(first), last(last) {}
};

template <typename Number>
struct SpecialisedNumberRange
    : public NumberRangeBase<SpecialisedNumberRange<Number>> {
    friend class NumberRangeBase<SpecialisedNumberRange<Number>>;
    Number first;
    Number last;
    Number increment;
    SpecialisedNumberRange(Number first, Number last, Number increment)
        : first(first), last(last), increment(increment) {}
};

template <typename Number>
struct InfiniteNumberRange
    : public NumberRangeBase<InfiniteNumberRange<Number>> {
    friend class NumberRangeBase<SpecialisedNumberRange<Number>>;
    Number first;
    Number increment;
    InfiniteNumberRange(Number first, Number increment)
        : first(first), increment(increment) {}

    inline bool hasValue() { return true; }
};

template <typename Number>
inline auto range(Number last) {
    return StandardNumberRange<Number>(0, last);
}

template <typename Number>
inline auto range(Number first, Number last) {
    return StandardNumberRange<Number>(first, last);
}

template <typename Number>
inline auto infRange(Number first, Number increment) {
    return InfiniteNumberRange<Number>(first, increment);
}

template <typename Number>
inline auto range(Number first, Number last, Number increment) {
    return SpecialisedNumberRange<Number>(first, last, increment);
}
}  // namespace LazyCode

#endif /* LAZYCODE_NUMBERRANGE_H_*/
