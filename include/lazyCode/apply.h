#ifndef LAZYCODE_APPLY_H_
#define LAZYCODE_APPLY_H_
#include "mapRange.h"
#include "rangeBase.h"
namespace LazyCode {
template <typename Func>
class ApplyEvaluator : public RangeEvaluator<void> {
    Func func;

   public:
    ApplyEvaluator(Func&& func) : func(std::forward<Func>(func)) {}
    template <typename T>
    inline bool push(T&& item) {
        func(item);
    }
};

template <typename Func>
inline auto apply(Func&& func) {
    return ApplyEvaluator<Func>(std::forward<Func>(func));
}
}  // namespace LazyCode

#endif /* LAZYCODE_APPLY_H_*/
