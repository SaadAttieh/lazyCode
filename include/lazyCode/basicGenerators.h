#ifndef LAZYCODE_BASICGENERATORS_H_
#define LAZYCODE_BASICGENERATORS_H_
#include <iostream>
#include "generator.h"

#include <iterator>
namespace LazyCode {

/*
 * Create a never ending generator of sequence of values.  Sequence begins at
 * specified start point (inclusive) and is followed by values
 * increasing/decreasing by the specified increment.
 */
template <typename Number1, typename Number2>
auto infRange(Number1 start, Number2 increment) {
    return generator(start, [increment](Number1& val) -> optional<Number1> {
        Number1 preVal = val;
        val += increment;
        return preVal;
    });
}

/*
 * Create a generator of sequence of values.  Sequence begins at
 * specified start point (inclusive) , is followed by values
 * increasing/decreasing by the specified increment and stops at the specified
 * last point (exclusive).
 */
template <typename Number1, typename Number2>
auto range(Number1 start, Number1 end, Number2 increment) {
    auto comparator = (increment >= 0)
                          ? [](Number1 a, Number1 b) { return a < b; }
                          : [](Number1 a, Number1 b) { return b < a; };
    return generator(
        start, [end, increment, comparator](Number1& val) -> optional<Number1> {
            if (!comparator(val, end)) {
                return nullopt;
            }
            Number1 preVal = val;
            val += increment;
            return preVal;
        });
}

/*
 * Create a generator of sequence of integral values.  Sequence begins at
 * specified start point (inclusive) , is followed by values increasing by 1 and
 * stops at the specified last point (exclusive).
 */
template <typename Number, typename std::enable_if<
                               std::is_integral<Number>::value, int>::type = 0>
auto range(Number start, Number end) {
    return range(start, end, 1);
}

/*
 * Create a generator of sequence of integral values.  Sequence begins at 0
 * (inclusive) , is followed by values increasing by 1 and stops at the
 * specified last point (exclusive).
 */
template <typename Number, typename std::enable_if<
                               std::is_integral<Number>::value, int>::type = 0>
auto range(Number end) {
    return range(((Number)0), end, 1);
}

/*
 * Create a generator from a container.  The generator uses the containers begin
 * and end iterators via `std::begin, std::end`.  If an rvalue is given, the
 * generator will take ownership of the container and move it into the generator
 * object, otherwise the generator will only hold a reference to the container.
 */
template <typename Container,
          detail::EnableIfNotType<detail::GeneratorBase, Container> = 0>
decltype(auto) generator(Container&& container) {
    typedef detail::RmRef<decltype(std::begin(container))> Iter;
    typedef decltype(*std::begin(container)) IterValueType;
    struct Iterable {
        Container container;
        Iter first;
        Iter last;
        Iterable(Container&& container)
            : container(std::forward<Container>(container)),
              first(std::begin(this->container)),
              last(std::end(this->container)) {}
    };
    return generator(Iterable(std::forward<Container>(container)),
                     [](auto&& iter) mutable -> OptionOrRef<IterValueType> {
                         if (iter.first == iter.last) {
                             return nullopt;
                         }
                         decltype(auto) val = *(iter.first);
                         ++iter.first;
                         return val;
                     });
}

/*
 * Create a generator from a pair of iterators first and last.  The generator
 * container yields values from first (inclusive) to last (exclusive).
 */
template <typename Iter>
decltype(auto) slice(Iter first, Iter last) {
    typedef decltype(*first) IterValueType;
    struct Iterable {
        Iter first;
        Iter last;
        Iterable(Iter first, Iter last)
            : first(std::move(first)), last(std::move(last)) {}
    };
    return generator(Iterable(std::move(first), std::move(last)),
                     [](auto&& iter) mutable -> OptionOrRef<IterValueType> {
                         if (iter.first == iter.last) {
                             return nullopt;
                         }
                         decltype(auto) val = *(iter.first);
                         ++iter.first;
                         return val;
                     });
}

/*
 * return a generator that iterates through a container from position start
 * (inclusive) to position end (exclusive).
  If an rvalue is given, the generator will take ownership of the container and
 move it into the generator object, otherwise the generator will only hold a
 reference to the container.
*/
template <typename Container,
          detail::EnableIfNotType<detail::GeneratorBase, Container> = 0>
decltype(auto) slice(Container&& container, size_t start, size_t last) {
    typedef detail::RmRef<decltype(std::begin(container))> Iter;
    typedef decltype(*std::begin(container)) IterValueType;
    struct Iterable {
        Container container;
        Iter first;
        Iter last;
        Iterable(Container&& container, size_t start, size_t last)
            : container(std::forward<Container>(container)),
              first(std::begin(this->container) + start),
              last(std::begin(this->container) + last) {}
    };
    return generator(Iterable(std::forward<Container>(container), start, last),
                     [](auto&& iter) mutable -> OptionOrRef<IterValueType> {
                         if (iter.first == iter.last) {
                             return nullopt;
                         }
                         decltype(auto) val = *(iter.first);
                         ++iter.first;
                         return val;
                     });
}
/*
 * Map one generator to another.  Produce a generator that returns the values
 * produced by another generator applied to the given function.  The given
 * function is invoked lazily to each value as requested. If the generator is
 * not specified, a GeneratorBuilder is returned.  GeneratorBuilders are
 * converted to generators by piping `|` a generator to them.
 */
template <typename MapperFunc, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) map(MapperFunc&& mapperIn, Generator&& gen = GeneratorHole()) {
    return buildOrDelay(
        generatorBuilder([mapperIn1 = std::forward<MapperFunc>(mapperIn)](
                             auto&& gen) mutable {
            typedef decltype(gen) Gen;
            return generator(
                wrapIfRef(std::forward<Gen>(gen)),
                [mapper = std::forward<MapperFunc>(mapperIn1)](
                    auto&& gen) -> OptionOrRef<decltype(mapperIn(*next(gen)))> {
                    decltype(auto) val = next(gen);
                    if (!val) {
                        return nullopt;
                    } else {
                        return mapper(*val);
                    }
                });
        }),
        std::forward<Generator>(gen));
}

/*
 * Produce a generator that filters the output of another generator according to
 * the given function.  The function should accept each value produced by the
 * given generator and return true if that value is to be forwarded, false
 * otherwise. The given function is invoked lazily to each value as requested.
 * If the generator is not specified, a GeneratorBuilder is returned.
 * GeneratorBuilders are converted to generators by piping `|` a generator to
 * them.
 */
template <typename FilterFunc, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) filter(FilterFunc&& filterIn,
                      Generator&& gen = GeneratorHole()) {
    return buildOrDelay(
        generatorBuilder([filterIn1 = std::forward<FilterFunc>(filterIn)](
                             auto&& gen) mutable {
            typedef decltype(gen) Gen;
            typedef typename detail::RmRef<Gen>::YieldType YieldType;
            return generator(wrapIfRef(std::forward<Gen>(gen)),
                             [filter = std::forward<FilterFunc>(filterIn1)](

                                 auto&& gen) -> OptionOrRef<YieldType> {
                                 while (true) {
                                     decltype(auto) val = next(gen);
                                     if (!val || filter(*val)) {
                                         return val;
                                     }
                                 }
                             });
        }),
        std::forward<Generator>(gen));
}

/*
 * Produce a generator that takes the first n values produced by another
 * generator.If the generator is not specified, a GeneratorBuilder is returned.
 * GeneratorBuilders are converted to generators by piping `|` a generator to
 * them.
 */
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) limit(size_t n, Generator&& gen = GeneratorHole()) {
    return buildOrDelay(
        generatorBuilder([n](auto&& gen) mutable {
            typedef decltype(gen) Gen;
            typedef typename detail::RmRef<Gen>::YieldType YieldType;
            size_t count = 0;
            return generator(
                wrapIfRef(std::forward<Gen>(gen)),
                [count, n](auto&& gen) mutable -> OptionOrRef<YieldType> {
                    if (count++ < n) {
                        return next(gen);
                    } else {
                        return nullopt;
                    }
                });
        }),
        std::forward<Generator>(gen));
}

/*
 * Combine two generators x and y into one.  Produce a generator that yields
 * tuples where the first element of each tuple is a value pulled from x and the
 * second element is pulled from y.  The generator ends when either x or y
 * end.*/
template <typename Gen1, typename Gen2,
          detail::EnableIfType<detail::GeneratorBase, Gen1> = 0,
          detail::EnableIfType<detail::GeneratorBase, Gen2> = 0>
decltype(auto) zip(Gen1&& gen1, Gen2&& gen2) {
    typedef typename Gen1::YieldType Gen1YieldType;
    typedef typename Gen2::YieldType Gen2YieldType;
    typedef std::pair<Gen1YieldType, Gen2YieldType> YieldType;
    return generator(wrapIfRef(std::forward<Gen1>(gen1)),
                     [gen2 = std::forward<Gen2>(gen2)](
                         auto&& gen1) mutable -> optional<YieldType> {
                         auto o1 = next(gen1);
                         auto o2 = next(gen2);
                         if (!o1 || !o2) {
                             return nullopt;
                         } else {
                             return YieldType(std::forward<Gen1YieldType>(*o1),
                                              std::forward<Gen2YieldType>(*o2));
                         }
                     });
}

/*
 * Enumerate a generator.  Produce a generator that returns the values
 * produced by another generator paired with an increasing count.  The default
 * initial value of the count is 0.  Each yielded item will be
 * pair(count,value).  If the generator is not specified, a GeneratorBuilder is
 * returned. GeneratorBuilders are converted to generators by piping `|` a
 * generator to them.
 */
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) enumerate(size_t count = 0, Generator&& gen = GeneratorHole()) {
    return buildOrDelay(generatorBuilder([count](auto&& gen) mutable {
                            typedef decltype(gen) Gen;
                            return zip(infRange(count, 1),
                                       std::forward<Gen>(gen));
                        }),
                        std::forward<Generator>(gen));
}

/*
 * return a generator that reads from the given stream.  The generated type (the
 * type of values pulled from the stream) must be specified as the first
 * template parameter.  For example, to read integers from the stream, use
 * `read<int>`.  If an lvalue is given, only a reference to the stream is held.
 * If a rvalue is given, the generator takes ownership, the stream is moved into
 * the generator.*/
template <typename ReadType, typename Stream>
auto read(Stream&& stream) {
    struct Reader {
        Stream stream;
        std::istream_iterator<ReadType> first;
        std::istream_iterator<ReadType> last;
        Reader(Stream&& stream)
            : stream(std::forward<Stream>(stream)), first(this->stream) {}
    };
    return generator(Reader(std::forward<Stream>(stream)),
                     [](auto&& reader) mutable -> optional<ReadType> {
                         if (reader.first == reader.last) {
                             return nullopt;
                         }
                         decltype(auto) val = *reader.first;
                         ++reader.first;
                         return val;
                     });
}

/*
 * return a generator that reads lines from the given stream.  The generator
 * yields a new string for each line.  If an lvalue is given, only a reference
 * to the stream is held.  If a rvalue is given, the generator takes ownership,
 * the stream is moved into the generator.*/
template <typename Stream>
auto readLines(Stream&& stream) {
    return generator(detail::wrapIfRef(std::forward<Stream>(stream)),
                     [](auto&& stream) mutable -> optional<std::string> {
                         if (static_cast<bool>(stream)) {
                             std::string s;
                             std::getline(stream, s);
                             return s;
                         } else {
                             return nullopt;
                         }
                     });
}

}  // namespace LazyCode
#endif /* LAZYCODE_BASICGENERATORS_H_*/
