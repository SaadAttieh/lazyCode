
#ifndef INCLUDE_LAZYCODE_UTILS_H_
#define INCLUDE_LAZYCODE_UTILS_H_
#include <functional>
#include <sstream>
#include <tuple>
#include <utility>
#include "rangeBase.h"
namespace LazyCode {
lazyCodeMacro(
#define _l1(i, b) [&](auto&& i) { return (b); }
#define _l2(i, j, b) [&](auto&& i, auto&& j) { return (b); }
    )

    // unpack tuple into arguments to function
    namespace detail {
    template <class F, size_t... Is>
    constexpr auto index_apply_impl(F && f, std::index_sequence<Is...>) {
        return f(std::integral_constant<size_t, Is>{}...);
    }

    template <size_t N, class F>
    constexpr auto index_apply(F && f) {
        return index_apply_impl(f, std::make_index_sequence<N>{});
    }

    template <class F, class Tuple>
    constexpr auto apply(F && f, Tuple && t) {
        return index_apply<std::tuple_size<RmRef<Tuple>>::value>(
            [&](auto... Is) {
                return f(std::forward<decltype(std::get<Is>(t))>(
                    std::get<Is>(t))...);
            });
    }
}

template <typename Func>
auto unpack(Func&& func) {
    return [funcIn = std::forward<Func>(func)](auto&& tup) {
        return detail::apply(funcIn, tup);
    };
}

/** varadic to string function
 *
 */
template <typename... Args>
std::string toString(Args const&... args) {
    std::ostringstream result;
    int unpack[]{0, (result << args, 0)...};
    static_cast<void>(unpack);
    return result.str();
}

}  // namespace LazyCode
#endif /* INCLUDE_LAZYCODE_UTILS_H_ */
