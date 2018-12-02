#ifndef LAZYCODE_BASICCOLLECTORS_H_
#define LAZYCODE_BASICCOLLECTORS_H_
#include "collector.h"
namespace LazyCode {

/*
 *  Apply the given function to each value produced by the
 * given generator.  The return value of the function is ignored and may be
 * void.  If the generator is not specified, a collector is returned. Collectors
 * remember the operation to be executed.  The operation is executed when a
 * generator is piped `|` to it.
 */
template <typename Func, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) forEach(Func&& func, Generator&& gen = GeneratorHole()) {
    return buildOrApply(collector(std::forward<Func>(func)),
                        std::forward<Generator>(gen));
}

/*
 *  Count the number of values yielded by the given generator.  If the generator
 * is not specified, a collector is returned. Collectors remember the operation
 * to be executed.  The operation is executed when a generator is piped `|` to
 * it.
 */
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) count(Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        collector(((size_t)0), [](size_t& count, auto&&) { ++count; }),
        std::forward<Generator>(gen));
}

/*
 *  Combine the values produced by a generator using the
 * specified function and return the result.  The function should take two
 * arguments and return a single value.  The accum parameter (accumulator)
 * specifies the initial value. The return type of the function must be
 * convertible to the accum type. If the generator is not specified, a collector
 * is returned. Collectors remember the operation to be executed.  The operation
 * is executed when a generator is piped `|` to it.
 */
template <typename Accum, typename Func, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) fold(Func&& func, Accum accum,
                    Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        collector(std::move(accum),
                  [func = std::forward<Func>(func)](auto& accum, auto&& val) {
                      accum = func(accum, val);
                  }),
        std::forward<Generator>(gen));
}

/*
 *  Return the sum of the values produced by a generator.  If
 * the generator is not specified, a collector is returned.
 * Collectors remember the operation to be executed.  The operation is executed
 * when a generator is piped `|` to it.
 */
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) sum(Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        [](auto&& gen) {
            typedef decltype(gen) Gen;
            typedef typename detail::RmRef<Gen>::NoRefYieldType AccumType;
            return fold<AccumType>(
                [](const auto& i, const auto& j) { return i + j; }, 0);
        },
        std::forward<Generator>(gen));
}

/*
 *  Return the product (multiplication) of the values produced by a generator.If
 * the generator is not specified, a collector is returned.
 * Collectors remember the operation to be executed.  The operation is executed
 * when a generator is piped `|` to it.
 */
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) product(Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        [](auto&& gen) {
            typedef decltype(gen) Gen;
            typedef typename detail::RmRef<Gen>::NoRefYieldType AccumType;
            return fold<AccumType>(
                [](const auto& i, const auto& j) { return i * j; }, 1);
        },
        std::forward<Generator>(gen));
}

/*
 *  Return the minimum value produced by a generator.  If the generator yields
 * no values, the default value is returned.  If a default value is not given,
 * an optional is returned.  The optional holds a value in the case the
 * generator yielded a value, otherwise the optional will be empty (nullopt).
 * If the generator is not specified, a collector is returned. Collectors
 * remember the operation to be executed.  The operation is executed when a
 * generator is piped `|` to it.
 */
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) min(Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        [](auto&& gen) {
            typedef decltype(gen) Gen;
            typedef typename detail::RmRef<Gen>::NoRefYieldType AccumType;
            return fold<optional<AccumType>>(

                [](const auto& i, const auto& j) {
                    return (i && *i < j) ? *i : j;
                },
                nullopt);
        },
        std::forward<Generator>(gen));
}

template <typename Val, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) min(Val defaultVal, Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        [defaultVal = std::move(defaultVal)](auto&& gen) {
            typedef decltype(gen) Gen;
            typedef typename detail::RmRef<Gen>::NoRefYieldType AccumType;
            return fold<AccumType>(
                [](const auto& i, const auto& j) { return (i < j) ? i : j; },
                defaultVal);
        },
        std::forward<Generator>(gen));
}

/*
 *  Return the maximum value produced by a generator.  If the generator yields
 * no values, the default value is returned.  If a default value is not given,
 * an optional is returned.  The optional holds a value in the case the
 * generator yielded a value, otherwise the optional will be empty (nullopt).
 * If the generator is not specified, a collector is returned. Collectors
 * remember the operation to be executed.  The operation is executed when a
 * generator is piped `|` to it.
 */
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) max(Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        [](auto&& gen) {
            typedef decltype(gen) Gen;
            typedef typename detail::RmRef<Gen>::NoRefYieldType AccumType;
            return fold<optional<AccumType>>(

                [](const auto& i, const auto& j) {
                    return (i && *i > j) ? *i : j;
                },
                nullopt);
        },
        std::forward<Generator>(gen));
}

template <typename Val, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) max(Val defaultVal, Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        [defaultVal = std::move(defaultVal)](auto&& gen) {
            typedef decltype(gen) Gen;
            typedef typename detail::RmRef<Gen>::NoRefYieldType AccumType;
            return fold<AccumType>(
                [](const auto& i, const auto& j) { return (i > j) ? i : j; },
                defaultVal);
        },
        std::forward<Generator>(gen));
}

/*
 *  Write each value produced by the given generator to the
 * given stream.  An interleave value can be optionally specified, in which case
 * the interleave value will be written to the stream before each generated
 * value apart from the first.  This is sometimes known as join.  Note that a
 * rvalue can be given as the stream parameter, for example constructing a new
 * stream inline (`write(ostringstream())`), in which case the stream will be
 * returned.If the generator is not specified, a collector is returned.
 * Collectors remember the operation to be executed.  The operation is executed
 * when a generator is piped `|` to it.
 */
template <typename Stream, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) write(Stream&& stream, Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        collector(detail::wrapIfRef(std::forward<Stream>(stream)),
                  [](auto& stream, auto&& val) { stream << val; }),
        std::forward<Generator>(gen));
}

template <typename Stream, typename Interleave,
          typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) write(Stream&& stream, Interleave i,
                     Generator&& gen = GeneratorHole()) {
    bool first = true;
    return buildOrApply(
        collector(detail::wrapIfRef(std::forward<Stream>(stream)),
                  [first, i = std::move(i)](auto& stream, auto&& val) mutable {
                      if (first) {
                          first = false;
                      } else {
                          stream << i;
                      }
                      stream << val;
                  }),
        std::forward<Generator>(gen));
}

/*
 *  Append each value produced by the given generator to the
 * given container, using container.emplace_back().  For unordered containers,
 * see `insert`.  Note that a rvalue can be given as the container parameter,
 * for example constructing a new container inline (`append(vector<int>())`), in
 * which case the container will be returned. If the generator is not specified,
 * a collector is returned. Collectors remember the operation to be executed.
 * The operation is executed when a generator is piped `|` to it.
 */
template <typename Container, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) append(Container&& container,
                      Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        collector(
            detail::wrapIfRef(std::forward<Container>(container)),
            [](auto& container, auto&& val) { container.emplace_back(val); }),
        std::forward<Generator>(gen));
}

/*
 *  Insert each value produced by the given generator to the
 * given container, using container.emplace().  This is for unordered
 * containers.  For ordered containers, see `append`.  Note that a rvalue can be
 * given as the container parameter, for example constructing a new container
 * inline (`append(set<int>())`), in which case the container will be returned.
 * If the generator is not specified, a collector is returned. Collectors
 * remember the operation to be executed. The operation is executed when a
 * generator is piped `|` to it.
 */
template <typename Container, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) insert(Container&& container,
                      Generator&& gen = GeneratorHole()) {
    return buildOrApply(
        collector(detail::wrapIfRef(std::forward<Container>(container)),
                  [](auto& container, auto&& val) { container.emplace(val); }),
        std::forward<Generator>(gen));
}

}  // namespace LazyCode
#endif /* LAZYCODE_BASICCOLLECTORS_H_*/
