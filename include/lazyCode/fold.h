#ifndef LAZYCODE_FOLD_H_
#define LAZYCODE_FOLD_H_
#include "rangeBase.h"
namespace LazyCode {
template <typename Func, typename Accumulator>
class FolderEvaluator : public RangeEvaluator<Accumulator> {
    Func func;

   public:
    FolderEvaluator(Func&& func, Accumulator accum)
        : RangeEvaluator<Accumulator>(std::move(accum)),
          func(std::forward<Func>(func)) {}
    template <typename T>
    inline bool push(T&& item) {
        this->result = std::move(func(this->result, item));
        return true;
    }
    inline void rangeEnd() {}
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
