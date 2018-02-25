
#ifndef INCLUDE_LAZYCODE_UTILS_H_
#define INCLUDE_LAZYCODE_UTILS_H_
#include <functional>
#include <sstream>
#include <tuple>
#include <utility>
#include "rangeBase.h"
namespace LazyCode {
lazyCodeMacro(
#define _completeLambda0(body) ) { return body; }
#define _completeLambda1(arg1, body) auto&& arg1 _completeLambda0(body)
#define _completeLambda2(arg1, arg2, body) \
    auto &&arg1, _completeLambda1(arg2, body)
#define _completeLambda3(arg1, arg2, arg3, body) \
    auto &&arg1, _completeLambda2(arg2, arg3, body)
#define _completeLambda4(arg1, arg2, arg3, arg4, body) \
    auto &&arg1, _completeLambda3(arg2, arg3, arg4, body)
#define _completeLambda5(arg1, arg2, arg3, arg4, arg5, body) \
    auto &&arg1, _completeLambda4(arg2, arg3, arg4, arg5, body)
#define _getCompleteLambdaOverload(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define _completeLambda(...)                                               \
    _getCompleteLambdaOverload(                                            \
        __VA_ARGS__, _completeLambda5, _completeLambda4, _completeLambda3, \
        _completeLambda2, _completeLambda1, _completeLambda0)(__VA_ARGS__)
#define lambda(...) [&]( _completeLambda(__VA_ARGS__)

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
