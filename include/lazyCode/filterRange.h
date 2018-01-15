#ifndef LAZYCODE_FILTERRANGE_H_
#define LAZYCODE_FILTERRANGE_H_
#include <utility>
#include "rangeBase.h"
namespace LazyCode {
template <typename Child, typename RangeReturnType, bool isRef>
class FilterRangeValueOrRef;

template <typename Child, typename RangeReturnType>
class FilterRangeValueOrRef<Child, RangeReturnType, false> : public RangeBase {
    inline auto& thisChild() { return static_cast<Child&>(*this); }

    RmRef<RangeReturnType> value;

   protected:
    inline RangeReturnType& assignValue() {
        this->value = thisChild().range.getValue();
        return value;
    }

   public:
    inline RangeReturnType& getValue() { return value; }
};

template <typename Child, typename RangeReturnType>
class FilterRangeValueOrRef<Child, RangeReturnType, true> : public RangeBase {
    inline auto& thisChild() { return static_cast<Child&>(*this); }

   protected:
    inline auto& assignValue() { return thisChild().range.getValue(); }

   public:
    inline auto& getValue() { return thisChild().range.getValue(); }
};

template <typename Range>
using RangeReturnType = decltype(std::declval<Range>().getValue());

template <typename Range, typename Func>
class FilterRange
    : public FilterRangeValueOrRef<FilterRange<Range, Func>,
                                   RangeReturnType<Range>,
                                   IsRef<RangeReturnType<Range>>::value> {
    typedef FilterRangeValueOrRef<FilterRange<Range, Func>,
                                  RangeReturnType<Range>,
                                  IsRef<RangeReturnType<Range>>::value>
        BaseType;
    using BaseType::assignValue;

   public:
    Range range;
    Func func;
    bool valueFound = false;

   public:
    FilterRange(RmRef<Range>&& range, Func func)
        : range(std::move(range)), func(func) {}
    FilterRange(RmRef<Range>& range, Func func) : range(range), func(func) {}

    inline bool hasValue() {
        if (!range.hasValue()) {
            return false;
        } else if (valueFound) {
            return true;
        }
        valueFound = func(assignValue());
        if (!valueFound) {
            moveNext<false>();
        }
        return valueFound;
    }

    template <bool check = true>
    inline void moveNext() {
        valueFound = false;
        if (check && !range.hasValue()) {
            return;
        }
        bool rangeHasValue;
        do {
            range.moveNext();
            rangeHasValue = range.hasValue();
            if (rangeHasValue) {
                valueFound = func(assignValue());
            } else {
                valueFound = false;
            }
        } while (rangeHasValue && !valueFound);
    }

    inline auto begin() {
        return RangeIterator<FilterRange<Range, Func>>(*this);
    }
    inline auto end() { return RangeIterator<FilterRange<Range, Func>>(*this); }
};

template <typename Func, typename Range, EnableIfRange<Range> = 0>
inline auto filter(Func&& func, Range&& range) {
    return FilterRange<Range, RmRef<Func>>(std::forward<Range>(range),
                                           std::forward<Func>(func));
}

template <typename Func, typename Container, EnableIfNotRange<Container> = 0>
inline auto filter(Func&& func, Container&& container) {
    return filter(std::forward<Func>(func),
                  toRange(std::forward<Container>(container)));
}
template <typename Func>
class FilterBuilder {
    Func func;

   public:
    FilterBuilder(Func func) : func(func) {}
    template <typename T>
    friend inline auto operator|(T&& iterable,
                                 const FilterBuilder<Func>& builder) {
        return filter(builder.func, std::forward<T>(iterable));
    }
};

template <typename Func>
inline FilterBuilder<Func> filter(Func&& func) {
    return FilterBuilder<Func>(std::forward<Func>(func));
}

}  // namespace LazyCode
#endif /* LAZYCODE_FILTERRANGE_H_*/