#ifndef LAZYCODE_PRINT_H_
#define LAZYCODE_PRINT_H_
#include <iostream>
#include <string>
#include "rangeBase.h"
namespace LazyCode {
static constexpr const char EMPTY_STR[] = "";
template <typename OutputStream, typename String1, typename String2,
          typename String3>
class PrinterBuilder : public RangeBuilder {
    OutputStream os;
    String1 sep;
    String2 open;
    String3 close;

   public:
    PrinterBuilder(OutputStream&& os, String1&& sep, String2&& open,
                   String3&& close)
        : os(std::forward<OutputStream>(os)),
          sep(std::forward<String1>(sep)),
          open(std::forward<String2>(open)),
          close(std::forward<String3>(close)) {}

    template <typename T, EnableIfRange<T> = 0>
    inline decltype(auto) build(T&& iterable) {
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
inline auto print(OutputStream&& os, String1&& sep = "", String2&& open = "",
                  String3&& close = "") {
    return PrinterBuilder<OutputStream, String1, String2, String3>(
        std::forward<OutputStream>(os), std::forward<String1>(sep),
        std::forward<String2>(open), std::forward<String3>(close));
}

}  // namespace LazyCode

#endif /* LAZYCODE_PRINT_H_*/
