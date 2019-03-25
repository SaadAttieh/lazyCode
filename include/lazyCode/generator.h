#ifndef LAZYCODE_GENERATOR_H_
#define LAZYCODE_GENERATOR_H_
#include <type_traits>
#include <utility>
#include "nonstd/optional.hpp"
#include "utils.h"
namespace LazyCode {
namespace detail {
struct GeneratorBase {};
struct GeneratorBuilderBase {};
struct CollectorBase;

}  // namespace detail
template <typename Generator>
class GeneratorIterator;
template <typename Member, typename ProducerFunc>
class Generator;

template <typename Member, typename ProducerFunc>
class Generator : public detail::GeneratorBase {
    Member member;
    ProducerFunc producer;

   public:
    typedef typename detail::YieldType<decltype(
        std::declval<ProducerFunc&>()(std::declval<Member&>()))>
        YieldType;
    typedef detail::RmRef<YieldType> NoRefYieldType;

    Generator(Member&& member, ProducerFunc&& producer)
        : member(std::forward<Member>(member)),
          producer(std::forward<ProducerFunc>(producer)) {}

    friend inline decltype(auto) next(Generator<Member, ProducerFunc>& gen) {
        return gen.producer(gen.member);
    }

    inline auto begin() {
        return GeneratorIterator<Generator<Member, ProducerFunc>>(*this);
    }

    inline auto end() {
        return GeneratorIterator<Generator<Member, ProducerFunc>>();
    }
};

struct GeneratorHole : public detail::GeneratorBase {};

template <typename MemberIn, typename ProducerFuncIn>
auto generator(MemberIn member, ProducerFuncIn producer) {
    typedef typename detail::StdRefToRef<MemberIn>::type Member;
    typedef typename detail::StdRefToRef<ProducerFuncIn>::type ProducerFunc;
    return Generator<Member, ProducerFunc>(
        std::forward<MemberIn>(member), std::forward<ProducerFuncIn>(producer));
}

template <typename BuilderFunc>
struct GeneratorBuilder {
    BuilderFunc build;
    GeneratorBuilder(BuilderFunc&& build)
        : build(std::forward<BuilderFunc>(build)) {}
};

template <typename BuilderFunc>
auto generatorBuilder(BuilderFunc&& builder) {
    return GeneratorBuilder<BuilderFunc>(std::forward<BuilderFunc>(builder));
}

template <typename BuilderFunc, typename Generator,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
auto buildOrDelay(GeneratorBuilder<BuilderFunc> builder, Generator&& gen) {
    return builder.build(std::forward<Generator>(gen));
}

template <typename BuilderFunc>
auto buildOrDelay(GeneratorBuilder<BuilderFunc> builder, GeneratorHole) {
    return std::move(builder);
}

template <typename Gen, typename Func>
auto operator|(Gen&& gen, GeneratorBuilder<Func>&& builder) {
    return builder.build(std::forward<Gen>(gen));
}

template <typename Gen, typename Func>
auto operator|(Gen&& gen, GeneratorBuilder<Func>& builder) {
    return builder.build(std::forward<Gen>(gen));
}

template <typename Generator>
class GeneratorIterator {
    Generator* gen = NULL;
    typedef OptionOrRef<typename Generator::YieldType> OptionType;
    OptionType value;

   public:
    typedef typename Generator::NoRefYieldType value_type;
    typedef value_type& reference;
    typedef value_type* pointer;

    GeneratorIterator() : value(nullopt) {}
    GeneratorIterator(Generator& gen) : gen(&gen), value(next(gen)) {}

    decltype(auto) operator*() { return *value; }
    GeneratorIterator<Generator>& operator++() {
        value = next(*gen);
        return *this;
    }
    auto operator++(int) {
        auto temp = *value;
        value = next(*gen);
        return temp;
    }
    inline bool operator!=(const GeneratorIterator<Generator>&) {
        return value.has_value();
    }
    inline bool operator==(const GeneratorIterator<Generator>&) {
        return !value.has_value();
    }
};

}  // namespace LazyCode
#endif /*LAZYCODE_GENERATOR_H_*/
