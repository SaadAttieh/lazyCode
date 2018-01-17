#ifndef LAZYCODE_LIMITRANGE_H_
#define LAZYCODE_LIMITRANGE_H_
#include "rangeBase.h"
namespace LazyCode {

template <typename Range>
class LimitRange : public RangeBase {
    Range range;
    size_t limit;

   public:
    LimitRange(Range&& range, size_t limit)
        : range(std::forward<Range>(range)), limit(limit) {}

    inline bool hasValue() { return limit > 0 && range.hasValue(); }
    inline void moveNext() {
        --limit;
        if (limit == 0) {
            return;
        }
        range.moveNext();
    }
    inline decltype(auto) getValue() { return range.getValue(); }

    inline auto begin() { return RangeIterator<LimitRange<Range>>(*this); }
    inline auto end() { return RangeIterator<LimitRange<Range>>(*this); }
};

template <typename Range, EnableIfType<RangeBase, Range> = 0>
inline auto limit(size_t limit, Range&& range) {
    return LimitRange<Range>(std::forward<Range>(range), limit);
}

class LimitBuilder : public RangeBuilder {
    size_t limitValue;

   public:
    LimitBuilder(size_t limit) : limitValue(limit) {}
    template <typename T>
    inline auto build(T&& iterable) {
        return limit(limitValue, std::forward<T>(iterable));
    }
};

inline LimitBuilder limit(size_t limit) { return LimitBuilder(limit); }

}  // namespace LazyCode

#endif /* LAZYCODE_LIMITRANGE_H_*/
