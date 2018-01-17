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

template <typename InputStream>
class ReadLinesRange : public RangeBase {
    InputStream is;
    bool finished = false;

   public:
    ReadLinesRange(InputStream&& is) : is(std::forward<InputStream>(is)) {}

    inline bool hasValue() { return static_cast<bool>(is); }
    inline void moveNext() {}
    inline std::string getValue() {
        std::string result;
        std::getline(is, result);
        return result;
    }

    inline auto begin() {
        return RangeIterator<ReadLinesRange<InputStream>>(*this);
    }
    inline auto end() {
        return RangeIterator<ReadLinesRange<InputStream>>(*this);
    }
};

template <typename InputStream>
inline auto readLines(InputStream&& is) {
    return ReadLinesRange<InputStream>(std::forward<InputStream>(is));
}

}  // namespace LazyCode

#endif /* LAZYCODE_READRANGE_H_*/
