#ifndef LAZYCODE_COUNT_H_
#define LAZYCODE_COUNT_H_
#include "rangeBase.h"
namespace LazyCode {
template <typename Number>
class CounterEvaluator : public RangeEvaluator {
   public:
    template <typename T, EnableIfType<RangeBase, T> = 0>
    inline Number evaluate(T&& iterable) {
        Number count = 0;

        while (iterable.hasValue()) {
            ++count;
            iterable.getValue();
            iterable.moveNext();
        }
        return count;
    }
};

template <typename Number = size_t>
inline auto count() {
    return CounterEvaluator<Number>();
}
}  // namespace LazyCode

#endif /* LAZYCODE_COUNT_H_*/
