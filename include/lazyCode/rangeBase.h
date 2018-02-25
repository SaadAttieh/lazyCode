#ifndef LAZYCODE_RANGEBASE_H_
#define LAZYCODE_RANGEBASE_H_
#include <iterator>
#include <type_traits>
#include <utility>
#ifdef LAZY_CODE_NO_MACROS
#define lazyCodeMacro(x)  // nothing
#else
#define lazyCodeMacro(x) x
#endif

namespace LazyCode {

template <typename T>
using RmRef = typename std::remove_reference<T>::type;

template <typename T>
using IsRef = typename std::is_same<RmRef<T>&, T>;

struct RangeBase {};
struct RangeBuilder {};

template <typename T1, typename T2>
using EnableIfType =
    typename std::enable_if<std::is_base_of<T1, RmRef<T2>>::value, int>::type;

template <typename T1, typename T2>
using EnableIfNotType =
    typename std::enable_if<!std::is_base_of<T1, RmRef<T2>>::value, int>::type;
struct RangeEvaluatorBase {};

template <typename T>
using EnableIfNotAnyRangeObject = typename std::enable_if<
    !std::is_base_of<RangeBase, RmRef<T>>::value &&
        !std::is_base_of<RangeBuilder, RmRef<T>>::value &&
        !std::is_base_of<RangeEvaluatorBase, RmRef<T>>::value,
    int>::type;

template <typename Iterator>
class SliceRange : public RangeBase {
   protected:
    Iterator first;
    Iterator last;

   public:
    SliceRange(const Iterator& first, const Iterator& last)
        : first(first), last(last) {}

    inline Iterator begin() { return first; }
    inline Iterator end() { return last; }

    inline bool hasValue() { return first != last; }
    inline decltype(auto) getValue() { return *first; }
    inline void moveNext() { ++first; }
};

template <typename Iterator>
inline SliceRange<Iterator> slice(const Iterator& first, const Iterator& last) {
    return SliceRange<RmRef<Iterator>>(first, last);
}

template <typename IteratorSource, typename Iterator>
class OwningSliceRange : public RangeBase {
   protected:
    IteratorSource source;
    Iterator first;
    Iterator last;

   public:
    template <typename Func1, typename Func2>
    OwningSliceRange(IteratorSource&& source, Func1 firstProvider,
                     Func2 lastProvider)
        : source(std::forward<IteratorSource>(source)),
          first(firstProvider(this->source)),
          last(lastProvider(this->source)) {}

    inline Iterator begin() { return first; }
    inline Iterator end() { return last; }

    inline bool hasValue() { return first != last; }
    inline decltype(auto) getValue() { return *first; }
    inline void moveNext() { ++first; }
};

template <typename Container>
inline auto containerRange(Container&& container) {
    return OwningSliceRange<RmRef<Container>,
                            typename RmRef<Container>::iterator>(
        std::forward<Container>(container),
        [](auto& cont) { return std::begin(cont); },
        [](auto& cont) { return std::end(cont); });
}

template <typename Container>
inline SliceRange<typename Container::iterator> toRange(Container& container) {
    return slice(std::begin(container), std::end(container));
}

template <typename Container>
inline SliceRange<typename Container::iterator> toRange(Container&& container) {
    return containerRange(std::forward<Container>(container));
}

template <typename Range>
class RangeIterator
    : public std::iterator<
          std::input_iterator_tag,
          RmRef<decltype(std::declval<Range>().getValue())>, std::ptrdiff_t,
          const RmRef<decltype(std::declval<Range>().getValue())>,
          decltype(std::declval<Range>().getValue())> {
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

template <typename Range, typename Container,
          EnableIfType<RangeBase, Range> = 0,
          EnableIfNotAnyRangeObject<Container> = 0>
decltype(auto) operator|(Range&& range, Container&& container) {
    while (range.hasValue()) {
        container.emplace_back(std::move(range.getValue()));
        range.moveNext();
    }
    return std::forward<Container>(container);
}

template <typename Range, typename Builder, EnableIfType<RangeBase, Range> = 0,
          EnableIfType<RangeBuilder, Builder> = 0>
inline decltype(auto) operator|(Range&& range, Builder&& builder) {
    return builder.build(std::forward<Range>(range));
}
template <typename Container, typename Builder,
          EnableIfNotAnyRangeObject<Container> = 0,
          EnableIfType<RangeBuilder, Builder> = 0>
inline decltype(auto) operator|(Container&& container, Builder&& builder) {
    return builder.build(toRange(std::forward<Container>(container)));
}

/* evaluates ranges, other classes will extend and override push function to
 * determine how the range should be evaluated. */
template <typename Result>
struct RangeEvaluator : public RangeEvaluatorBase {
    Result result;
    RangeEvaluator() {}
    template <typename T>
    RangeEvaluator(T&& res) : result(std::forward<T>(res)) {}
    inline Result get() { return std::move(result); }
    inline operator Result() { return get(); }
};
template <>
struct RangeEvaluator<void> : public RangeEvaluatorBase {};

struct DoNothingEvaluator : public RangeEvaluator<void> {
    template <typename T>
    inline bool push(T&&) {
        return true;
    }
    inline void rangeEnd() {}
};
inline auto eval() { return DoNothingEvaluator(); }

template <typename Range, typename Evaluator,
          EnableIfType<RangeBase, Range> = 0,
          EnableIfType<RangeEvaluatorBase, Evaluator> = 0>
inline decltype(auto) operator|(Range&& range, Evaluator&& evaluator) {
    while (range.hasValue() && evaluator.push(range.getValue())) {
        range.moveNext();
    }
    evaluator.rangeEnd();
    return std::forward<Evaluator>(evaluator);
}
template <typename Container, typename Evaluator,
          EnableIfNotAnyRangeObject<Container> = 0,
          EnableIfType<RangeEvaluatorBase, Evaluator> = 0>
inline decltype(auto) operator|(Container&& container, Evaluator&& evaluator) {
    return operator|(toRange(std::forward<Container>(container)),
                     std::forward<Evaluator>(evaluator));
}
}  // namespace LazyCode
#endif /*LAZYCODE_RANGEBASE_H_*/
