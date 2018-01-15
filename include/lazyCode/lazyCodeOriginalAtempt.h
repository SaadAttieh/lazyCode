#include <iterator>
#include <type_traits>

namespace LazyCode {

template <typename T>
using RmRef = typename std::remove_reference<T>::type;

template <typename Iterator>
class IterRange {
    Iterator first;
    Iterator last;

   public:
    IterRange(const Iterator& first, const Iterator& last)
        : first(first), last(last) {}

    inline Iterator begin() { return first; }
    inline Iterator end() { return last; }

    inline bool hasValue() { return first != last; }
    inline decltype(auto) getValue() { return *first; }
    inline void moveNext() { ++first; }
};

template <typename Iterator>
IterRange<Iterator> iterRange(const Iterator& first, const Iterator& last) {
    return IterRange<RmRef<Iterator>>(first, last);
}

template <typename Range>
class MapRange {
    Range range;

   public:
    template <typename std::enable_if<
                  std::is_same<RmRef<Range>, Range>::value int>::type = 0>
    MapRange(Range&& range) : range(std::move(range)) {}

    MapIterator(const Iterator& other) : Iterator(other) {}
    inline auto operator*() {
        // the auto is not a reference so return value will be copied
        return Iterator::operator*();
    }
};

template <typename Container>
auto map(Container&& container) {
    typedef RmRef<decltype(std::begin(container))> IterBeginType;
    typedef RmRef<decltype(std::end(container))> IterEndType;
    return iterRange(MapIterator<IterBeginType>(std::begin(container)),
                     MapIterator<IterEndType>(std::end(container)));
}

}  // namespace LazyCode