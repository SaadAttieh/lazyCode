#ifndef LAZYCODE_ENUMERATEDRANGE_H_
#define LAZYCODE_ENUMERATEDRANGE_H_
#include <tuple>
#include <utility>
#include "rangeBase.h"
namespace LazyCode {

template <typename Range>
class EnumeratedRange : public RangeBase {
    Range range;
    size_t count = 0;
    typedef std::pair<size_t, decltype(std::declval<Range>().getValue())>
        ReturnType;

   public:
    EnumeratedRange(Range&& range) : range(std::forward<Range>(range)) {}

    inline bool hasValue() { return range.hasValue(); }
    inline void moveNext() {
        range.moveNext();
        ++count;
    }
    inline ReturnType getValue() { return ReturnType(count, range.getValue()); }

    inline auto begin() { return RangeIterator<EnumeratedRange<Range>>(*this); }
    inline auto end() { return RangeIterator<EnumeratedRange<Range>>(*this); }
};

template <typename Range, EnableIfType<RangeBase, Range> = 0>
inline auto enumerate(Range&& range) {
    return EnumeratedRange<Range>(std::forward<Range>(range));
}

template <typename Container, EnableIfNotType<RangeBase, Container> = 0>
inline auto enumerate(Container&& container) {
    return enumerate(toRange(std::forward<Container>(container)));
}

class EnumeratedBuilder : public RangeBuilder {
   public:
    template <typename T>
    inline auto build(T&& iterable) {
        return enumerate(std::forward<T>(iterable));
    }
};

inline EnumeratedBuilder enumerate() { return EnumeratedBuilder(); }

}  // namespace LazyCode

#endif /* LAZYCODE_ENUMERATEDRANGE_H_*/
