#ifndef LAZYCODE_READRANGE_H_
#define LAZYCODE_READRANGE_H_
#include <iostream>
#include "rangeBase.h"

namespace LazyCode {

template <typename ReadType = char, typename InputStream>
inline auto read(InputStream& is) {
    return slice(std::istream_iterator<ReadType>(is),
                 std::istream_iterator<ReadType>());
}

template <typename ReadType = char, typename InputStream>
inline auto read(InputStream&& is) {
    return OwningSliceRange<RmRef<InputStream>,
                            std::istream_iterator<ReadType>>(
        std::forward<InputStream>(is),
        [](auto& input) { return std::istream_iterator<ReadType>(input); },
        [](auto&) { return std::istream_iterator<ReadType>(); });
}

}  // namespace LazyCode

#endif /* LAZYCODE_READRANGE_H_*/
