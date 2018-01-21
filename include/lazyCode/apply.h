#ifndef LAZYCODE_APPLY_H_
#define LAZYCODE_APPLY_H_
#include "mapRange.h"
#include "rangeBase.h"
namespace LazyCode {
template <typename Func>
class ApplyEvaluator : public RangeEvaluator {
    Func func;

   public:
    ApplyEvaluator(Func&& func) : func(std::forward<Func>(func)) {}
    template <typename T, EnableIfType<RangeBase, T> = 0>
    inline void evaluate(T&& iterable) {
        iterable | map(func) | eval();
    }
};

template <typename Func>
inline auto apply(Func&& func) {
    return ApplyEvaluator<Func>(std::forward<Func>(func));
}
}  // namespace LazyCode

#endif /* LAZYCODE_APPLY_H_*/
