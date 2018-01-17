#ifndef LAZYCODE_WRITE_H_
#define LAZYCODE_WRITE_H_
#include <iostream>
#include <string>
#include "rangeBase.h"
namespace LazyCode {
static constexpr const char EMPTY_STR[] = "";
template <typename OutputStream, typename String1, typename String2,
          typename String3>
class WriteerEvaluator : public RangeEvaluator {
    OutputStream os;
    String1 sep;
    String2 open;
    String3 close;

   public:
    WriteerEvaluator(OutputStream&& os, String1&& sep, String2&& open,
                     String3&& close)
        : os(std::forward<OutputStream>(os)),
          sep(std::forward<String1>(sep)),
          open(std::forward<String2>(open)),
          close(std::forward<String3>(close)) {}

    template <typename T, EnableIfType<RangeBase,T> = 0>
    inline decltype(auto) evaluate(T&& iterable) {
        bool first = true;
        os << open;
        while (iterable.hasValue()) {
            if (first) {
                first = false;
            } else {
                os << sep;
            }
            os << iterable.getValue();
            iterable.moveNext();
        }
        os << close;
        return std::forward<OutputStream>(os);
    }
};

template <typename OutputStream, typename String1 = std::string,
          typename String2 = std::string, typename String3 = std::string>
inline auto write(OutputStream&& os, String1&& sep = "", String2&& open = "",
                  String3&& close = "") {
    return WriteerEvaluator<OutputStream, String1, String2, String3>(
        std::forward<OutputStream>(os), std::forward<String1>(sep),
        std::forward<String2>(open), std::forward<String3>(close));
}

}  // namespace LazyCode

#endif /* LAZYCODE_WRITE_H_*/
