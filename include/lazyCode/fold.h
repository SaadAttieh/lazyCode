#ifndef LAZYCODE_FOLD_H_
#define LAZYCODE_FOLD_H_
#include "rangeBase.h"
namespace LazyCode {
template <typename Func, typename Accumulator>
class FolderBuilder : public RangeBuilder {
    Func func;
    Accumulator accum;

   public:
    FolderBuilder(Func&& func, Accumulator accum)
        : func(std::forward<Func>(func)), accum(std::move(accum)) {}
    template <typename T, EnableIfRange<T> = 0>
    inline Accumulator& build(T&& iterable) {
        while (iterable.hasValue()) {
            accum = std::move(func(accum, iterable.getValue()));
            iterable.moveNext();
        }
        return accum;
    }
    inline operator Accumulator&() const { return accum; }
    inline Accumulator& get() { return accum; }
};

template <typename Func, typename Accumulator>
inline auto fold(Func&& func, Accumulator&& accum) {
    return FolderBuilder<Func, RmRef<Accumulator>>(
        std::forward<Func>(func), std::forward<Accumulator>(accum));
}

template <typename Number = int>
decltype(auto) sum() {
    return fold([](Number i, Number j) -> Number { return i + j; }, (Number)0);
}
template <typename Number = int>
decltype(auto) product() {
    return fold([](Number i, Number j) -> Number { return i * j; }, (Number)1);
}

}  // namespace LazyCode

#endif /* LAZYCODE_FOLD_H_*/
