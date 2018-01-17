#ifndef LAZYCODE_FOLD_H_
#define LAZYCODE_FOLD_H_
#include "rangeBase.h"
namespace LazyCode {
template <typename Func, typename Accumulator>
class FolderEvaluator : public RangeEvaluator {
    Func func;
    Accumulator accum;

   public:
    FolderEvaluator(Func&& func, Accumulator accum)
        : func(std::forward<Func>(func)), accum(std::move(accum)) {}
    template <typename T, EnableIfType<RangeBase, T> = 0>
    inline decltype(auto) evaluate(T&& iterable) {
        while (iterable.hasValue()) {
            accum = std::move(func(accum, iterable.getValue()));
            iterable.moveNext();
        }
        return accum;
    }
};

template <typename Func, typename Accumulator>
inline auto fold(Func&& func, Accumulator&& accum) {
    return FolderEvaluator<Func, RmRef<Accumulator>>(
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
