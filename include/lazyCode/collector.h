#ifndef LAZYCODE_COLLECTOR_H_
#define LAZYCODE_COLLECTOR_H_
#include <iostream>
#include <type_traits>
#include <utility>
#include "generator.h"
#include "nonstd/optional.hpp"
#include "utils.h"
namespace LazyCode {
namespace detail {
struct CollectorBase {};
struct GeneratorBase;

}  // namespace detail

template <typename Member, typename CollectorFunc>
class Collector : public detail::CollectorBase {
    Member member;
    CollectorFunc collector;

   public:
    Collector(Member&& member, CollectorFunc&& collector)
        : member(std::forward<Member>(member)),
          collector(std::forward<CollectorFunc>(collector)) {}

    template <typename V>
    void push(V& v) {
        collector(member, v);
    }

    template <typename V>
    void push(std::reference_wrapper<V>& v) {
        collector(member, v.get());
    }

    template <typename Generator>
    Member apply(Generator&& gen) {
        while (true) {
            auto val = next(gen);
            if (!val) {
                return std::forward<Member>(member);
            }
            push(*val);
        }
    }
};

template <typename CollectorFunc>
class Collector<void, CollectorFunc> : public detail::CollectorBase {
    CollectorFunc collector;

   public:
    Collector(CollectorFunc&& collector)
        : collector(std::forward<CollectorFunc>(collector)) {}

    template <typename V>
    void push(V& v) {
        collector(v);
    }

    template <typename V>
    void push(std::reference_wrapper<V>& v) {
        collector(v.get());
    }
    template <typename Generator>
    void apply(Generator&& gen) {
        while (true) {
            auto val = next(gen);
            if (!val) {
                return;
            }
            push(*val);
        }
    }
};

template <typename MemberIn, typename CollectorFuncIn>
auto collector(MemberIn member, CollectorFuncIn collector) {
    typedef typename detail::StdRefToRef<MemberIn>::type Member;
    typedef typename detail::StdRefToRef<CollectorFuncIn>::type CollectorFunc;
    return Collector<Member, CollectorFunc>(
        std::forward<MemberIn>(member),
        std::forward<CollectorFuncIn>(collector));
}

template <typename CollectorFuncIn>
auto collector(CollectorFuncIn collector) {
    typedef typename detail::StdRefToRef<CollectorFuncIn>::type CollectorFunc;
    return Collector<void, CollectorFunc>(
        std::forward<CollectorFuncIn>(collector));
}

template <typename CollectorBuilder, typename Generator,
          detail::EnableIfNotType<detail::CollectorBase, CollectorBuilder> = 0,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) buildOrApply(CollectorBuilder&& builder, Generator&& gen) {
    return builder(gen).apply(std::forward<Generator>(gen));
}

template <typename Collector, typename Generator,
          detail::EnableIfType<detail::CollectorBase, Collector> = 0,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) buildOrApply(Collector&& collector, Generator&& gen) {
    return collector.apply(std::forward<Generator>(gen));
}

template <typename T>
T buildOrApply(T v, GeneratorHole) {
    return std::move(v);
}

template <typename Generator, typename CollectorBuilder,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0,
          detail::EnableIfNotType<detail::CollectorBase, CollectorBuilder> = 0>
decltype(auto) operator|(Generator&& gen, CollectorBuilder&& builder) {
    return buildOrApply(std::forward<CollectorBuilder>(builder),
                        std::forward<Generator>(gen));
}

template <typename Generator, typename Member, typename Func,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) operator|(Generator&& gen, Collector<Member, Func>& collector) {
    return collector.apply(std::forward<Generator>(gen));
}

template <typename Generator, typename Member, typename Func,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) operator|(Generator&& gen, Collector<Member, Func> collector) {
    return collector.apply(std::forward<Generator>(gen));
}

}  // namespace LazyCode
#endif /*LAZYCODE_COLLECTOR_H_*/
