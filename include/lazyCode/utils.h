
#ifndef INCLUDE_LAZYCODE_UTILS_H_
#define INCLUDE_LAZYCODE_UTILS_H_
#include <functional>
#include <sstream>
#include <tuple>
#include <utility>
#include "nonstd/optional.hpp"
#ifdef LAZY_CODE_NO_MACROS
#define lazyCodeMacro(x)  // nothing
#else
#define lazyCodeMacro(x) x
#endif

namespace LazyCode {

using nonstd::make_optional;
using nonstd::nullopt;
using nonstd::optional;

template <typename T>
using optionalref = optional<std::reference_wrapper<T>>;

namespace detail {

template <typename T>
using RmRef = typename std::remove_reference<T>::type;

template <typename T>
struct StdRefToRef {
    typedef T type;
};

template <typename T>
struct StdRefToRef<std::reference_wrapper<T>> {
    typedef T &type;
};

template <typename T>
struct StdRefToRef<std::reference_wrapper<T> &&> {
    typedef T &type;
};

template <typename T>
RmRef<T> &&wrapIfRef(T &&v) {
    return std::move(v);
}

template <typename T>
typename std::reference_wrapper<T> wrapIfRef(T &v) {
    return std::ref(v);
}

template <typename T1, typename T2>
using EnableIfType =
    typename std::enable_if<std::is_base_of<T1, RmRef<T2>>::value, int>::type;

template <typename T1, typename T2>
using EnableIfNotType =
    typename std::enable_if<!std::is_base_of<T1, RmRef<T2>>::value, int>::type;

// unpack tuple into arguments to function
template <class F, size_t... Is>
constexpr auto index_apply_impl(F &&f, std::index_sequence<Is...>) {
    return f(std::integral_constant<size_t, Is>{}...);
}

template <size_t N, class F>
constexpr auto index_apply(F &&f) {
    return index_apply_impl(f, std::make_index_sequence<N>{});
}

template <class F, class Tuple>
constexpr auto apply(F &&f, Tuple &&t) {
    return index_apply<std::tuple_size<RmRef<Tuple>>::value>([&](auto... Is) {
        return f(std::forward<decltype(std::get<Is>(t))>(std::get<Is>(t))...);
    });
}

template <class F, class Tuple>
constexpr auto applyWithoutForward(F &&f, Tuple &&t) {
    return index_apply<std::tuple_size<RmRef<Tuple>>::value>(
        [&](auto... Is) { return f(std::get<Is>(t)...); });
}

}  // namespace detail

template <typename Func>
auto unpack(Func &&func) {
    return [funcIn = std::forward<Func>(func)](auto &&tup) {
        return detail::apply(funcIn, tup);
    };
}

/** varadic to string function
 *
 */
template <typename... Args>
std::string toString(Args const &... args) {
    std::ostringstream result;
    int unpack[]{0, (result << args, 0)...};
    static_cast<void>(unpack);
    return result.str();
}

template <typename T>
struct OptionOrRef : public optional<T> {
    template <typename V>
    OptionOrRef(V &&v) : optional<T>::optional(std::forward<V>(v)) {}
};

template <typename T>
struct OptionOrRef<T &> : public optionalref<T> {
    template <typename V>
    OptionOrRef(V &&v) : optionalref<T>::optional(std::forward<V>(v)) {}

    decltype(auto) operator*() { return optionalref<T>::operator*().get(); }
};

template <typename T>
struct OptionOrRef<T &&> : public optional<T> {
    template <typename V>
    OptionOrRef(V &&v) : optional<T>::optional(std::forward<V>(v)) {}
};
namespace detail {
template <typename T>
struct YieldTypeImpl;

template <typename T>
struct YieldTypeImpl<optional<T>> {
    typedef T type;
};
template <typename T>
struct YieldTypeImpl<optionalref<T>> {
    typedef T &type;
};
template <typename T>
struct YieldTypeImpl<OptionOrRef<T>> {
    typedef T type;
};

template <typename T>
struct YieldTypeImpl<OptionOrRef<T &>> {
    typedef T &type;
};

template <typename T>
struct YieldTypeImpl<OptionOrRef<T &&>> {
    typedef T type;
};
template <typename T>
using YieldType = typename YieldTypeImpl<T>::type;

}  // namespace detail
}  // namespace LazyCode

lazyCodeMacro(
#define _completeLambda0(body) ) { return body; }
#define _completeLambda1(arg1, body) auto &&arg1 _completeLambda0(body)
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

#endif /* INCLUDE_LAZYCODE_UTILS_H_ */
