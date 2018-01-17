#ifndef LAZYCODE_MAPRANGE_H_
#define LAZYCODE_MAPRANGE_H_
#include "rangeBase.h"
namespace LazyCode {

template <typename Range, typename Func>
class MapRange : public RangeBase {
    Range range;
    Func func;

   public:
    MapRange(Range&& range, Func&& func)
        : range(std::forward<Range>(range)), func(std::forward<Func>(func)) {}

    inline bool hasValue() { return range.hasValue(); }
    inline void moveNext() { range.moveNext(); }
    inline decltype(auto) getValue() { return func(range.getValue()); }

    inline auto begin() { return RangeIterator<MapRange<Range, Func>>(*this); }
    inline auto end() { return RangeIterator<MapRange<Range, Func>>(*this); }
};

template <typename Func, typename Range, EnableIfRange<Range> = 0>
inline auto map(Func&& func, Range&& range) {
    return MapRange<Range, Func>(std::forward<Range>(range),
                                 std::forward<Func>(func));
}

template <typename Func, typename Container, EnableIfNotRange<Container> = 0>
inline auto map(Func&& func, Container&& container) {
    return map(std::forward<Func>(func),
               toRange(std::forward<Container>(container)));
}

template <typename Func>
class MapBuilder : public RangeBuilder {
    Func func;

   public:
    MapBuilder(Func&& func) : func(std::forward<Func>(func)) {}
    template <typename T>
    inline auto build(T&& iterable) {
        return map(std::forward<Func>(func), std::forward<T>(iterable));
    }
};

template <typename Func>
inline MapBuilder<Func> map(Func&& func) {
    return MapBuilder<Func>(std::forward<Func>(func));
}

}  // namespace LazyCode

#endif /* LAZYCODE_MAPRANGE_H_*/
