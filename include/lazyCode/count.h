#ifndef LAZYCODE_COUNT_H_
#define LAZYCODE_COUNT_H_
#include "rangeBase.h"
namespace LazyCode {
template <typename Number>
class CounterBuilder : public RangeBuilder {
    Number count = 0;

   public:
    template <typename T, EnableIfRange<T> = 0>
    inline Number build(T&& iterable) {
        while (iterable.hasValue()) {
            ++count;
            iterable.getValue();
            iterable.moveNext();
        }
        return count;
    }
    inline operator Number() const { return count; }
    inline Number get() { return count; }
};

template <typename Number = size_t>
inline auto count() {
    return CounterBuilder<Number>();
}
}  // namespace LazyCode

#endif /* LAZYCODE_COUNT_H_*/
