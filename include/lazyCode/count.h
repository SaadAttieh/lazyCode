#ifndef LAZYCODE_COUNT_H_
#define LAZYCODE_COUNT_H_
#include "rangeBase.h"
namespace LazyCode {
template <typename Number>
class CounterEvaluator : public RangeEvaluator<Number> {
   public:
    CounterEvaluator() : RangeEvaluator<Number>(0) {}
    template <typename T>
    inline bool push(T&&) {
        ++this->result;
        return true;
    }
    inline void rangeEnd() {}
};

template <typename Number = size_t>
inline auto count() {
    return CounterEvaluator<Number>();
}
}  // namespace LazyCode

#endif /* LAZYCODE_COUNT_H_*/
