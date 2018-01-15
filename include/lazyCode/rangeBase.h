#ifndef LAZYCODE_RANGEBASE_H_
#define LAZYCODE_RANGEBASE_H_
#include <iterator>
#include <type_traits>
#include <utility>

namespace LazyCode {

template <typename T>
using RmRef = typename std::remove_reference<T>::type;

template <typename T>
using IsRef = typename std::is_same<RmRef<T>&, T>;

struct RangeBase {};
struct RangeBuilder {};

template <typename T>
using EnableIfRange =
    typename std::enable_if<std::is_base_of<RangeBase, RmRef<T>>::value,
                            int>::type;

template <typename T>
using EnableIfNotRange =
    typename std::enable_if<!std::is_base_of<RangeBase, RmRef<T>>::value,
                            int>::type;

template <typename T>
using EnableIfRangeBuilder =
    typename std::enable_if<std::is_base_of<RangeBuilder, RmRef<T>>::value,
                            int>::type;

template <typename T>
using EnableIfNotRangeBuilder =
    typename std::enable_if<!std::is_base_of<RangeBuilder, RmRef<T>>::value,
                            int>::type;

template <typename Iterator>
class IterRange : public RangeBase {
   protected:
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
inline IterRange<Iterator> iterRange(const Iterator& first,
                                     const Iterator& last) {
    return IterRange<RmRef<Iterator>>(first, last);
}

template <typename Container>
struct IterRangeWithContainer : public IterRange<typename Container::iterator> {
    typedef IterRange<typename Container::iterator> IterRangeBase;
    Container container;

    IterRangeWithContainer(Container& container)
        : IterRangeBase(std::begin(container), std::end(container)),
          container(std::move(container)) {
        // need to override first and last as container was moved
        IterRangeBase::first = std::begin(container);
        IterRangeBase::last = std::end(container);
    }
};

template <typename Container>
inline IterRangeWithContainer<Container> iterRangeWithBase(
    Container&& container) {
    return IterRangeWithContainer<RmRef<Container>>(
        std::forward<Container>(container));
}

template <typename Container>
inline IterRange<typename Container::iterator> toRange(Container& container) {
    return iterRange(std::begin(container), std::end(container));
}

template <typename Container>
inline IterRange<typename Container::iterator> toRange(Container&& container) {
    return iterRangeWithContainer(std::forward<Container>(container));
}

template <typename Range>
class RangeIterator
    : public std::iterator<std::input_iterator_tag,
                           decltype(std::declval<Range>().getValue()),
                           std::ptrdiff_t, void, void> {
    Range& range;

   public:
    RangeIterator(Range& range) : range(range) {}
    inline RangeIterator& operator++() {
        range.moveNext();
        return *this;
    }
    inline decltype(auto) operator*() { return range.getValue(); }
    inline bool operator!=(const RangeIterator<Range>&) {
        return range.hasValue();
    }
};

template <typename Range, typename Container, EnableIfRange<Range> = 0,
          EnableIfNotRange<Container> = 0,
          EnableIfNotRangeBuilder<Container> = 0>
decltype(auto) operator|(Range&& range, Container&& container) {
    while (range.hasValue()) {
        container.emplace_back(std::move(range.getValue()));
        range.moveNext();
    }
    return std::forward<Container>(container);
}

template <typename Range, typename Builder, EnableIfRangeBuilder<Builder> = 0>
inline decltype(auto) operator|(Range&& range, Builder&& builder) {
    return builder.build(std::forward<Range>(range));
}
enum EvalEnum { eval };

template <typename Range, EnableIfRange<Range> = 0>
void operator|(Range&& range, EvalEnum) {
    while (range.hasValue()) {
        range.getValue();
        range.moveNext();
    }
}

}  // namespace LazyCode
#endif /*LAZYCODE_RANGEBASE_H_*/
