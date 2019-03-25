#ifndef LAZYCODE_LAZYCODE_H_
#define LAZYCODE_LAZYCODE_H_
#ifndef LAZYCODE_BASICCOLLECTORS_H_
#define LAZYCODE_BASICCOLLECTORS_H_
#include <sstream>
#ifndef LAZYCODE_COLLECTOR_H_
#define LAZYCODE_COLLECTOR_H_
#include <iostream>
#include <type_traits>
#include <utility>
#ifndef LAZYCODE_GENERATOR_H_
#define LAZYCODE_GENERATOR_H_
#include <type_traits>
#include <utility>
//
// Copyright (c) 2014-2018 Martin Moene
//
// https://github.com/martinmoene/optional-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NONSTD_OPTIONAL_LITE_HPP
#define NONSTD_OPTIONAL_LITE_HPP

#define optional_lite_MAJOR  3
#define optional_lite_MINOR  1
#define optional_lite_PATCH  1

#define optional_lite_VERSION  optional_STRINGIFY(optional_lite_MAJOR) "." optional_STRINGIFY(optional_lite_MINOR) "." optional_STRINGIFY(optional_lite_PATCH)

#define optional_STRINGIFY(  x )  optional_STRINGIFY_( x )
#define optional_STRINGIFY_( x )  #x

// optional-lite configuration:

#define optional_OPTIONAL_DEFAULT  0
#define optional_OPTIONAL_NONSTD   1
#define optional_OPTIONAL_STD      2

#if !defined( optional_CONFIG_SELECT_OPTIONAL )
# define optional_CONFIG_SELECT_OPTIONAL  ( optional_HAVE_STD_OPTIONAL ? optional_OPTIONAL_STD : optional_OPTIONAL_NONSTD )
#endif

// Control presence of exception handling (try and auto discover):

#ifndef optional_CONFIG_NO_EXCEPTIONS
# if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#  define optional_CONFIG_NO_EXCEPTIONS  0
# else
#  define optional_CONFIG_NO_EXCEPTIONS  1
# endif
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   optional_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define optional_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define optional_CPLUSPLUS  __cplusplus
# endif
#endif

#define optional_CPP98_OR_GREATER  ( optional_CPLUSPLUS >= 199711L )
#define optional_CPP11_OR_GREATER  ( optional_CPLUSPLUS >= 201103L )
#define optional_CPP11_OR_GREATER_ ( optional_CPLUSPLUS >= 201103L )
#define optional_CPP14_OR_GREATER  ( optional_CPLUSPLUS >= 201402L )
#define optional_CPP17_OR_GREATER  ( optional_CPLUSPLUS >= 201703L )
#define optional_CPP20_OR_GREATER  ( optional_CPLUSPLUS >= 202000L )

// C++ language version (represent 98 as 3):

#define optional_CPLUSPLUS_V  ( optional_CPLUSPLUS / 100 - (optional_CPLUSPLUS > 200000 ? 2000 : 1994) )

// Use C++17 std::optional if available and requested:

#if optional_CPP17_OR_GREATER && defined(__has_include )
# if __has_include( <optional> )
#  define optional_HAVE_STD_OPTIONAL  1
# else
#  define optional_HAVE_STD_OPTIONAL  0
# endif
#else
# define  optional_HAVE_STD_OPTIONAL  0
#endif

#define optional_USES_STD_OPTIONAL  ( (optional_CONFIG_SELECT_OPTIONAL == optional_OPTIONAL_STD) || ((optional_CONFIG_SELECT_OPTIONAL == optional_OPTIONAL_DEFAULT) && optional_HAVE_STD_OPTIONAL) )

//
// in_place: code duplicated in any-lite, expected-lite, optional-lite, value-ptr-lite, variant-lite:
//

#ifndef nonstd_lite_HAVE_IN_PLACE_TYPES
#define nonstd_lite_HAVE_IN_PLACE_TYPES  1

// C++17 std::in_place in <utility>:

#if optional_CPP17_OR_GREATER

#include <utility>

namespace nonstd {

using std::in_place;
using std::in_place_type;
using std::in_place_index;
using std::in_place_t;
using std::in_place_type_t;
using std::in_place_index_t;

#define nonstd_lite_in_place_t(      T)  std::in_place_t
#define nonstd_lite_in_place_type_t( T)  std::in_place_type_t<T>
#define nonstd_lite_in_place_index_t(K)  std::in_place_index_t<K>

#define nonstd_lite_in_place(      T)    std::in_place_t{}
#define nonstd_lite_in_place_type( T)    std::in_place_type_t<T>{}
#define nonstd_lite_in_place_index(K)    std::in_place_index_t<K>{}

} // namespace nonstd

#else // optional_CPP17_OR_GREATER

#include <cstddef>

namespace nonstd {
namespace detail {

template< class T >
struct in_place_type_tag {};

template< std::size_t K >
struct in_place_index_tag {};

} // namespace detail

struct in_place_t {};

template< class T >
inline in_place_t in_place( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_t();
}

template< std::size_t K >
inline in_place_t in_place( detail::in_place_index_tag<K> = detail::in_place_index_tag<K>() )
{
    return in_place_t();
}

template< class T >
inline in_place_t in_place_type( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_t();
}

template< std::size_t K >
inline in_place_t in_place_index( detail::in_place_index_tag<K> = detail::in_place_index_tag<K>() )
{
    return in_place_t();
}

// mimic templated typedef:

#define nonstd_lite_in_place_t(      T)  nonstd::in_place_t(&)( nonstd::detail::in_place_type_tag<T>  )
#define nonstd_lite_in_place_type_t( T)  nonstd::in_place_t(&)( nonstd::detail::in_place_type_tag<T>  )
#define nonstd_lite_in_place_index_t(K)  nonstd::in_place_t(&)( nonstd::detail::in_place_index_tag<K> )

#define nonstd_lite_in_place(      T)    nonstd::in_place_type<T>
#define nonstd_lite_in_place_type( T)    nonstd::in_place_type<T>
#define nonstd_lite_in_place_index(K)    nonstd::in_place_index<K>

} // namespace nonstd

#endif // optional_CPP17_OR_GREATER
#endif // nonstd_lite_HAVE_IN_PLACE_TYPES

//
// Using std::optional:
//

#if optional_USES_STD_OPTIONAL

#include <optional>

namespace nonstd {

    using std::optional;
    using std::bad_optional_access;
    using std::hash;

    using std::nullopt;
    using std::nullopt_t;

    using std::operator==;
    using std::operator!=;
    using std::operator<;
    using std::operator<=;
    using std::operator>;
    using std::operator>=;
    using std::make_optional;
    using std::swap;
}

#else // optional_USES_STD_OPTIONAL

#include <cassert>
#include <utility>

// optional-lite alignment configuration:

#ifndef  optional_CONFIG_MAX_ALIGN_HACK
# define optional_CONFIG_MAX_ALIGN_HACK  0
#endif

#ifndef  optional_CONFIG_ALIGN_AS
// no default, used in #if defined()
#endif

#ifndef  optional_CONFIG_ALIGN_AS_FALLBACK
# define optional_CONFIG_ALIGN_AS_FALLBACK  double
#endif

// Compiler warning suppression:

#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wundef"
#elif defined(__GNUC__)
# pragma GCC   diagnostic push
# pragma GCC   diagnostic ignored "-Wundef"
#elif defined(_MSC_VER )
# pragma warning( push )
#endif

// half-open range [lo..hi):
#define optional_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Compiler versions:
//
// MSVC++ 6.0  _MSC_VER == 1200 (Visual Studio 6.0)
// MSVC++ 7.0  _MSC_VER == 1300 (Visual Studio .NET 2002)
// MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio .NET 2003)
// MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
// MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 14.1 _MSC_VER >= 1910 (Visual Studio 2017)

#if defined(_MSC_VER ) && !defined(__clang__)
# define optional_COMPILER_MSVC_VER      (_MSC_VER )
# define optional_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define optional_COMPILER_MSVC_VER      0
# define optional_COMPILER_MSVC_VERSION  0
#endif

#define optional_COMPILER_VERSION( major, minor, patch )  ( 10 * (10 * major + minor ) + patch )

#if defined(__GNUC__) && !defined(__clang__)
# define optional_COMPILER_GNUC_VERSION   optional_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define optional_COMPILER_GNUC_VERSION   0
#endif

#if defined(__clang__)
# define optional_COMPILER_CLANG_VERSION  optional_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
# define optional_COMPILER_CLANG_VERSION  0
#endif

#if optional_BETWEEN(optional_COMPILER_MSVC_VERSION, 70, 140 )
# pragma warning( disable: 4345 )   // initialization behavior changed
#endif

#if optional_BETWEEN(optional_COMPILER_MSVC_VERSION, 70, 150 )
# pragma warning( disable: 4814 )   // in C++14 'constexpr' will not imply 'const'
#endif

// Presence of language and library features:

#define optional_HAVE(FEATURE) ( optional_HAVE_##FEATURE )

#ifdef _HAS_CPP0X
# define optional_HAS_CPP0X  _HAS_CPP0X
#else
# define optional_HAS_CPP0X  0
#endif

// Unless defined otherwise below, consider VC14 as C++11 for optional-lite:

#if optional_COMPILER_MSVC_VER >= 1900
# undef  optional_CPP11_OR_GREATER
# define optional_CPP11_OR_GREATER  1
#endif

#define optional_CPP11_90   (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1500)
#define optional_CPP11_100  (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1600)
#define optional_CPP11_110  (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1700)
#define optional_CPP11_120  (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1800)
#define optional_CPP11_140  (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1900)
#define optional_CPP11_141  (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1910)

#define optional_CPP14_000  (optional_CPP14_OR_GREATER)
#define optional_CPP17_000  (optional_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define optional_HAVE_CONSTEXPR_11      optional_CPP11_140
#define optional_HAVE_NOEXCEPT          optional_CPP11_140
#define optional_HAVE_NULLPTR           optional_CPP11_100
#define optional_HAVE_REF_QUALIFIER     optional_CPP11_140

// Presence of C++14 language features:

#define optional_HAVE_CONSTEXPR_14      optional_CPP14_000

// Presence of C++17 language features:

// no flag

// Presence of C++ library features:

#define optional_HAVE_CONDITIONAL       optional_CPP11_120
#define optional_HAVE_REMOVE_CV         optional_CPP11_120
#define optional_HAVE_TYPE_TRAITS       optional_CPP11_90

#define optional_HAVE_TR1_TYPE_TRAITS   (!! optional_COMPILER_GNUC_VERSION )
#define optional_HAVE_TR1_ADD_POINTER   (!! optional_COMPILER_GNUC_VERSION )

// C++ feature usage:

#if optional_HAVE( CONSTEXPR_11 )
# define optional_constexpr  constexpr
#else
# define optional_constexpr  /*constexpr*/
#endif

#if optional_HAVE( CONSTEXPR_14 )
# define optional_constexpr14  constexpr
#else
# define optional_constexpr14  /*constexpr*/
#endif

#if optional_HAVE( NOEXCEPT )
# define optional_noexcept  noexcept
#else
# define optional_noexcept  /*noexcept*/
#endif

#if optional_HAVE( NULLPTR )
# define optional_nullptr  nullptr
#else
# define optional_nullptr  NULL
#endif

#if optional_HAVE( REF_QUALIFIER )
# define optional_ref_qual  &
# define optional_refref_qual  &&
#else
# define optional_ref_qual  /*&*/
# define optional_refref_qual  /*&&*/
#endif

// additional includes:

#if optional_CONFIG_NO_EXCEPTIONS
// already included: <cassert>
#else
# include <stdexcept>
#endif

#if optional_CPP11_OR_GREATER
# include <functional>
#endif

#if optional_HAVE( INITIALIZER_LIST )
# include <initializer_list>
#endif

#if optional_HAVE( TYPE_TRAITS )
# include <type_traits>
#elif optional_HAVE( TR1_TYPE_TRAITS )
# include <tr1/type_traits>
#endif

// Method enabling

#if optional_CPP11_OR_GREATER

#define optional_REQUIRES_0(...) \
    template< bool B = (__VA_ARGS__), typename std::enable_if<B, int>::type = 0 >

#define optional_REQUIRES_T(...) \
    , typename = typename std::enable_if< (__VA_ARGS__), nonstd::optional_lite::detail::enabler >::type

#define optional_REQUIRES_R(R, ...) \
    typename std::enable_if< (__VA_ARGS__), R>::type

#define optional_REQUIRES_A(...) \
    , typename std::enable_if< (__VA_ARGS__), void*>::type = nullptr

#endif

//
// optional:
//

namespace nonstd { namespace optional_lite {

namespace std11 {

#if optional_CPP11_OR_GREATER
    using std::move;
#else
    template< typename T > T & move( T & t ) { return t; }
#endif

#if optional_HAVE( CONDITIONAL )
    using std::conditional;
#else
    template< bool B, typename T, typename F > struct conditional              { typedef T type; };
    template<         typename T, typename F > struct conditional<false, T, F> { typedef F type; };
#endif // optional_HAVE_CONDITIONAL

} // namespace std11

#if optional_CPP11_OR_GREATER

/// type traits C++17:

namespace std17 {

#if optional_CPP17_OR_GREATER

using std::is_swappable;
using std::is_nothrow_swappable;

#elif optional_CPP11_OR_GREATER

namespace detail {

using std::swap;

struct is_swappable
{
    template< typename T, typename = decltype( swap( std::declval<T&>(), std::declval<T&>() ) ) >
    static std::true_type test( int );

    template< typename >
    static std::false_type test(...);
};

struct is_nothrow_swappable
{
    // wrap noexcept(epr) in separate function as work-around for VC140 (VS2015):

    template< typename T >
    static constexpr bool test()
    {
        return noexcept( swap( std::declval<T&>(), std::declval<T&>() ) );
    }

    template< typename T >
    static auto test( int ) -> std::integral_constant<bool, test<T>()>{}

    template< typename >
    static std::false_type test(...);
};

} // namespace detail

// is [nothow] swappable:

template< typename T >
struct is_swappable : decltype( detail::is_swappable::test<T>(0) ){};

template< typename T >
struct is_nothrow_swappable : decltype( detail::is_nothrow_swappable::test<T>(0) ){};

#endif // optional_CPP17_OR_GREATER

} // namespace std17

/// type traits C++20:

namespace std20 {

template< typename T >
struct remove_cvref
{
    typedef typename std::remove_cv< typename std::remove_reference<T>::type >::type type;
};

} // namespace std20

#endif // optional_CPP11_OR_GREATER

/// class optional

template< typename T >
class optional;

namespace detail {

// for optional_REQUIRES_T

#if optional_CPP11_OR_GREATER
enum class enabler{};
#endif

// C++11 emulation:

struct nulltype{};

template< typename Head, typename Tail >
struct typelist
{
    typedef Head head;
    typedef Tail tail;
};

#if optional_CONFIG_MAX_ALIGN_HACK

// Max align, use most restricted type for alignment:

#define optional_UNIQUE(  name )       optional_UNIQUE2( name, __LINE__ )
#define optional_UNIQUE2( name, line ) optional_UNIQUE3( name, line )
#define optional_UNIQUE3( name, line ) name ## line

#define optional_ALIGN_TYPE( type ) \
    type optional_UNIQUE( _t ); struct_t< type > optional_UNIQUE( _st )

template< typename T >
struct struct_t { T _; };

union max_align_t
{
    optional_ALIGN_TYPE( char );
    optional_ALIGN_TYPE( short int );
    optional_ALIGN_TYPE( int );
    optional_ALIGN_TYPE( long int  );
    optional_ALIGN_TYPE( float  );
    optional_ALIGN_TYPE( double );
    optional_ALIGN_TYPE( long double );
    optional_ALIGN_TYPE( char * );
    optional_ALIGN_TYPE( short int * );
    optional_ALIGN_TYPE( int *  );
    optional_ALIGN_TYPE( long int * );
    optional_ALIGN_TYPE( float * );
    optional_ALIGN_TYPE( double * );
    optional_ALIGN_TYPE( long double * );
    optional_ALIGN_TYPE( void * );

#ifdef HAVE_LONG_LONG
    optional_ALIGN_TYPE( long long );
#endif

    struct Unknown;

    Unknown ( * optional_UNIQUE(_) )( Unknown );
    Unknown * Unknown::* optional_UNIQUE(_);
    Unknown ( Unknown::* optional_UNIQUE(_) )( Unknown );

    struct_t< Unknown ( * )( Unknown)         > optional_UNIQUE(_);
    struct_t< Unknown * Unknown::*            > optional_UNIQUE(_);
    struct_t< Unknown ( Unknown::* )(Unknown) > optional_UNIQUE(_);
};

#undef optional_UNIQUE
#undef optional_UNIQUE2
#undef optional_UNIQUE3

#undef optional_ALIGN_TYPE

#elif defined( optional_CONFIG_ALIGN_AS ) // optional_CONFIG_MAX_ALIGN_HACK

// Use user-specified type for alignment:

#define optional_ALIGN_AS( unused ) \
    optional_CONFIG_ALIGN_AS

#else // optional_CONFIG_MAX_ALIGN_HACK

// Determine POD type to use for alignment:

#define optional_ALIGN_AS( to_align ) \
    typename type_of_size< alignment_types, alignment_of< to_align >::value >::type

template< typename T >
struct alignment_of;

template< typename T >
struct alignment_of_hack
{
    char c;
    T t;
    alignment_of_hack();
};

template< size_t A, size_t S >
struct alignment_logic
{
    enum { value = A < S ? A : S };
};

template< typename T >
struct alignment_of
{
    enum { value = alignment_logic<
        sizeof( alignment_of_hack<T> ) - sizeof(T), sizeof(T) >::value };
};

template< typename List, size_t N >
struct type_of_size
{
    typedef typename std11::conditional<
        N == sizeof( typename List::head ),
            typename List::head,
            typename type_of_size<typename List::tail, N >::type >::type type;
};

template< size_t N >
struct type_of_size< nulltype, N >
{
    typedef optional_CONFIG_ALIGN_AS_FALLBACK type;
};

template< typename T>
struct struct_t { T _; };

#define optional_ALIGN_TYPE( type ) \
    typelist< type , typelist< struct_t< type >

struct Unknown;

typedef
    optional_ALIGN_TYPE( char ),
    optional_ALIGN_TYPE( short ),
    optional_ALIGN_TYPE( int ),
    optional_ALIGN_TYPE( long ),
    optional_ALIGN_TYPE( float ),
    optional_ALIGN_TYPE( double ),
    optional_ALIGN_TYPE( long double ),

    optional_ALIGN_TYPE( char *),
    optional_ALIGN_TYPE( short * ),
    optional_ALIGN_TYPE( int * ),
    optional_ALIGN_TYPE( long * ),
    optional_ALIGN_TYPE( float * ),
    optional_ALIGN_TYPE( double * ),
    optional_ALIGN_TYPE( long double * ),

    optional_ALIGN_TYPE( Unknown ( * )( Unknown ) ),
    optional_ALIGN_TYPE( Unknown * Unknown::*     ),
    optional_ALIGN_TYPE( Unknown ( Unknown::* )( Unknown ) ),

    nulltype
    > > > > > > >    > > > > > > >
    > > > > > > >    > > > > > > >
    > > > > > >
    alignment_types;

#undef optional_ALIGN_TYPE

#endif // optional_CONFIG_MAX_ALIGN_HACK

/// C++03 constructed union to hold value.

template< typename T >
union storage_t
{
//private:
//    template< typename > friend class optional;

    typedef T value_type;

    storage_t() {}

    storage_t( value_type const & v )
    {
        construct_value( v );
    }

    void construct_value( value_type const & v )
    {
        ::new( value_ptr() ) value_type( v );
    }

#if optional_CPP11_OR_GREATER

    storage_t( value_type && v )
    {
        construct_value( std::move( v ) );
    }

    void construct_value( value_type && v )
    {
        ::new( value_ptr() ) value_type( std::move( v ) );
    }

    template< class... Args >
    void emplace( Args&&... args )
    {
        ::new( value_ptr() ) value_type( std::forward<Args>(args)... );
    }

    template< class U, class... Args >
    void emplace( std::initializer_list<U> il, Args&&... args )
    {
        ::new( value_ptr() ) value_type( il, std::forward<Args>(args)... );
    }

#endif

    void destruct_value()
    {
        value_ptr()->~T();
    }

    value_type const * value_ptr() const
    {
        return as<value_type>();
    }

    value_type * value_ptr()
    {
        return as<value_type>();
    }

    value_type const & value() const optional_ref_qual
    {
        return * value_ptr();
    }

    value_type & value() optional_ref_qual
    {
        return * value_ptr();
    }

#if optional_CPP11_OR_GREATER

    value_type const && value() const optional_refref_qual
    {
        return std::move( value() );
    }

    value_type && value() optional_refref_qual
    {
        return std::move( value() );
    }

#endif

#if optional_CPP11_OR_GREATER

    using aligned_storage_t = typename std::aligned_storage< sizeof(value_type), alignof(value_type) >::type;
    aligned_storage_t data;

#elif optional_CONFIG_MAX_ALIGN_HACK

    typedef struct { unsigned char data[ sizeof(value_type) ]; } aligned_storage_t;

    max_align_t hack;
    aligned_storage_t data;

#else
    typedef optional_ALIGN_AS(value_type) align_as_type;

    typedef struct { align_as_type data[ 1 + ( sizeof(value_type) - 1 ) / sizeof(align_as_type) ]; } aligned_storage_t;
    aligned_storage_t data;

#   undef optional_ALIGN_AS

#endif // optional_CONFIG_MAX_ALIGN_HACK

    void * ptr() optional_noexcept
    {
        return &data;
    }

    void const * ptr() const optional_noexcept
    {
        return &data;
    }

    template <typename U>
    U * as()
    {
        return reinterpret_cast<U*>( ptr() );
    }

    template <typename U>
    U const * as() const
    {
        return reinterpret_cast<U const *>( ptr() );
    }
};

} // namespace detail

/// disengaged state tag

struct nullopt_t
{
    struct init{};
    optional_constexpr nullopt_t( init ) {}
};

#if optional_HAVE( CONSTEXPR_11 )
constexpr nullopt_t nullopt{ nullopt_t::init{} };
#else
// extra parenthesis to prevent the most vexing parse:
const nullopt_t nullopt(( nullopt_t::init() ));
#endif

/// optional access error

#if ! optional_CONFIG_NO_EXCEPTIONS

class bad_optional_access : public std::logic_error
{
public:
  explicit bad_optional_access()
  : logic_error( "bad optional access" ) {}
};

#endif //optional_CONFIG_NO_EXCEPTIONS

/// optional

template< typename T>
class optional
{
private:
    template< typename > friend class optional;

    typedef void (optional::*safe_bool)() const;

public:
    typedef T value_type;

    // x.x.3.1, constructors

    // 1a - default construct
    optional_constexpr optional() optional_noexcept
    : has_value_( false )
    , contained()
    {}

    // 1b - construct explicitly empty
    optional_constexpr optional( nullopt_t ) optional_noexcept
    : has_value_( false )
    , contained()
    {}

    // 2 - copy-construct
    optional_constexpr14 optional( optional const & other
#if optional_CPP11_OR_GREATER
        optional_REQUIRES_A(
            true || std::is_copy_constructible<T>::value
        )
#endif
    )
    : has_value_( other.has_value() )
    {
        if ( other.has_value() )
            contained.construct_value( other.contained.value() );
    }

#if optional_CPP11_OR_GREATER

    // 3 (C++11) - move-construct from optional
    optional_constexpr14 optional( optional && other
        optional_REQUIRES_A(
            true || std::is_move_constructible<T>::value
        )
    ) noexcept( std::is_nothrow_move_constructible<T>::value )
    : has_value_( other.has_value() )
    {
        if ( other.has_value() )
            contained.construct_value( std::move( other.contained.value() ) );
    }

    // 4a (C++11) - explicit converting copy-construct from optional
    template< typename U >
    explicit optional( optional<U> const & other
        optional_REQUIRES_A(
            std::is_constructible<T, U const &>::value
            && !std::is_constructible<T, optional<U> &          >::value
            && !std::is_constructible<T, optional<U> &&         >::value
            && !std::is_constructible<T, optional<U> const &    >::value
            && !std::is_constructible<T, optional<U> const &&   >::value
            && !std::is_convertible<     optional<U> &       , T>::value
            && !std::is_convertible<     optional<U> &&      , T>::value
            && !std::is_convertible<     optional<U> const & , T>::value
            && !std::is_convertible<     optional<U> const &&, T>::value
            && !std::is_convertible<               U const & , T>::value /*=> explicit */
        )
    )
    : has_value_( other.has_value() )
    {
        if ( other.has_value() )
            contained.construct_value( T{ other.contained.value() } );
    }
#endif // optional_CPP11_OR_GREATER

    // 4b (C++98 and later) - non-explicit converting copy-construct from optional
    template< typename U >
    optional( optional<U> const & other
#if optional_CPP11_OR_GREATER
        optional_REQUIRES_A(
            std::is_constructible<T, U const &>::value
            && !std::is_constructible<T, optional<U> &          >::value
            && !std::is_constructible<T, optional<U> &&         >::value
            && !std::is_constructible<T, optional<U> const &    >::value
            && !std::is_constructible<T, optional<U> const &&   >::value
            && !std::is_convertible<     optional<U> &       , T>::value
            && !std::is_convertible<     optional<U> &&      , T>::value
            && !std::is_convertible<     optional<U> const & , T>::value
            && !std::is_convertible<     optional<U> const &&, T>::value
            &&  std::is_convertible<               U const & , T>::value /*=> non-explicit */
        )
#endif // optional_CPP11_OR_GREATER
    )
    : has_value_( other.has_value() )
    {
        if ( other.has_value() )
            contained.construct_value( other.contained.value() );
    }

#if optional_CPP11_OR_GREATER

    // 5a (C++11) - explicit converting move-construct from optional
    template< typename U >
    optional( optional<U> && other
        optional_REQUIRES_A(
            std::is_constructible<T, U &&>::value
            && !std::is_constructible<T, optional<U> &          >::value
            && !std::is_constructible<T, optional<U> &&         >::value
            && !std::is_constructible<T, optional<U> const &    >::value
            && !std::is_constructible<T, optional<U> const &&   >::value
            && !std::is_convertible<     optional<U> &       , T>::value
            && !std::is_convertible<     optional<U> &&      , T>::value
            && !std::is_convertible<     optional<U> const & , T>::value
            && !std::is_convertible<     optional<U> const &&, T>::value
            && !std::is_convertible<                     U &&, T>::value /*=> explicit */
        )
    )
    : has_value_( other.has_value() )
    {
        if ( other.has_value() )
            contained.construct_value( T{ std::move( other.contained.value() ) } );
    }

    // 5a (C++11) - non-explicit converting move-construct from optional
    template< typename U >
    optional( optional<U> && other
        optional_REQUIRES_A(
            std::is_constructible<T, U &&>::value
            && !std::is_constructible<T, optional<U> &          >::value
            && !std::is_constructible<T, optional<U> &&         >::value
            && !std::is_constructible<T, optional<U> const &    >::value
            && !std::is_constructible<T, optional<U> const &&   >::value
            && !std::is_convertible<     optional<U> &       , T>::value
            && !std::is_convertible<     optional<U> &&      , T>::value
            && !std::is_convertible<     optional<U> const & , T>::value
            && !std::is_convertible<     optional<U> const &&, T>::value
            &&  std::is_convertible<                     U &&, T>::value /*=> non-explicit */
        )
    )
    : has_value_( other.has_value() )
    {
        if ( other.has_value() )
            contained.construct_value( std::move( other.contained.value() ) );
    }

    // 6 (C++11) - in-place construct
    template< typename... Args
        optional_REQUIRES_T(
            std::is_constructible<T, Args&&...>::value
        )
    >
    optional_constexpr explicit optional( nonstd_lite_in_place_t(T), Args&&... args )
    : has_value_( true )
    , contained( T( std::forward<Args>(args)...) )
    {}

    // 7 (C++11) - in-place construct,  initializer-list
    template< typename U, typename... Args
        optional_REQUIRES_T(
            std::is_constructible<T, std::initializer_list<U>&, Args&&...>::value
        )
    >
    optional_constexpr explicit optional( nonstd_lite_in_place_t(T), std::initializer_list<U> il, Args&&... args )
    : has_value_( true )
    , contained( T( il, std::forward<Args>(args)...) )
    {}

    // 8a (C++11) - explicit move construct from value
    template< typename U = value_type >
    optional_constexpr explicit optional( U && value
        optional_REQUIRES_A(
            std::is_constructible<T, U&&>::value
            && !std::is_same<typename std20::remove_cvref<U>::type, nonstd_lite_in_place_t(U)>::value
            && !std::is_same<typename std20::remove_cvref<U>::type, optional<T>>::value
            && !std::is_convertible<U&&, T>::value /*=> explicit */
        )
    )
    : has_value_( true )
    , contained( T{ std::forward<U>( value ) } )
    {}

    // 8b (C++11) - non-explicit move construct from value
    template< typename U = value_type >
    optional_constexpr optional( U && value
        optional_REQUIRES_A(
            std::is_constructible<T, U&&>::value
            && !std::is_same<typename std20::remove_cvref<U>::type, nonstd_lite_in_place_t(U)>::value
            && !std::is_same<typename std20::remove_cvref<U>::type, optional<T>>::value
            && std::is_convertible<U&&, T>::value /*=> non-explicit */
        )
    )
    : has_value_( true )
    , contained( std::forward<U>( value ) )
    {}

#else // optional_CPP11_OR_GREATER

    // 8 (C++98)
    optional( value_type const & value )
    : has_value_( true )
    , contained( value )
    {}

#endif // optional_CPP11_OR_GREATER

    // x.x.3.2, destructor

    ~optional()
    {
        if ( has_value() )
            contained.destruct_value();
    }

    // x.x.3.3, assignment

    // 1 (C++98and later) -  assign explicitly empty
    optional & operator=( nullopt_t ) optional_noexcept
    {
        reset();
        return *this;
    }

    // 2 (C++98and later) - copy-assign from optional
#if optional_CPP11_OR_GREATER
    optional_REQUIRES_R(
        optional &,
        true
//      std::is_copy_constructible<T>::value
//      && std::is_copy_assignable<T>::value
    )
    operator=( optional const & other )
        noexcept(
            std::is_nothrow_move_assignable<T>::value
            && std::is_nothrow_move_constructible<T>::value
        )
#else
    optional & operator=( optional const & other )
#endif
    {
        if      ( has_value() == true  && other.has_value() == false ) reset();
        else if ( has_value() == false && other.has_value() == true  ) initialize( *other );
        else if ( has_value() == true  && other.has_value() == true  ) contained.value() = *other;
        return *this;
    }

#if optional_CPP11_OR_GREATER

    // 3 (C++11) - move-assign from optional
    optional_REQUIRES_R(
        optional &,
        true
//      std::is_move_constructible<T>::value
//      && std::is_move_assignable<T>::value
    )
    operator=( optional && other ) noexcept
    {
        if      ( has_value() == true  && other.has_value() == false ) reset();
        else if ( has_value() == false && other.has_value() == true  ) initialize( std::move( *other ) );
        else if ( has_value() == true  && other.has_value() == true  ) contained.value() = std::move( *other );
        return *this;
    }

    // 4 (C++11) - move-assign from value
    template< typename U = T >
        optional_REQUIRES_R(
            optional &,
            std::is_constructible<T , U>::value
            && std::is_assignable<T&, U>::value
            && !std::is_same<typename std20::remove_cvref<U>::type, nonstd_lite_in_place_t(U)>::value
            && !std::is_same<typename std20::remove_cvref<U>::type, optional<T>>::value
            && !(std::is_scalar<T>::value && std::is_same<T, typename std::decay<U>::type>::value)
        )
    operator=( U && value )
    {
        if ( has_value() ) contained.value() = std::forward<U>( value );
        else               initialize( T( std::forward<U>( value ) ) );
        return *this;
    }

#else // optional_CPP11_OR_GREATER

    // 4 (C++98) - copy-assign from value
    template< typename U /*= T*/ >
    optional & operator=( U const & value )
    {
        if ( has_value() ) contained.value() = value;
        else               initialize( T( value ) );
        return *this;
    }

#endif // optional_CPP11_OR_GREATER

    // 5 (C++98 and later) - converting copy-assign from optional
    template< typename U >
#if optional_CPP11_OR_GREATER
        optional_REQUIRES_R(
            optional&,
            std::is_constructible<  T , U const &>::value
            &&  std::is_assignable< T&, U const &>::value
            && !std::is_constructible<T, optional<U> &          >::value
            && !std::is_constructible<T, optional<U> &&         >::value
            && !std::is_constructible<T, optional<U> const &    >::value
            && !std::is_constructible<T, optional<U> const &&   >::value
            && !std::is_convertible<     optional<U> &       , T>::value
            && !std::is_convertible<     optional<U> &&      , T>::value
            && !std::is_convertible<     optional<U> const & , T>::value
            && !std::is_convertible<     optional<U> const &&, T>::value
            && !std::is_assignable<  T&, optional<U> &          >::value
            && !std::is_assignable<  T&, optional<U> &&         >::value
            && !std::is_assignable<  T&, optional<U> const &    >::value
            && !std::is_assignable<  T&, optional<U> const &&   >::value
        )
#else
    optional&
#endif // optional_CPP11_OR_GREATER
    operator=( optional<U> const & other )
    {
        return *this = optional( other );
    }

#if optional_CPP11_OR_GREATER

    // 6 (C++11) -  converting move-assign from optional
    template< typename U >
        optional_REQUIRES_R(
            optional&,
            std::is_constructible<  T , U>::value
            &&  std::is_assignable< T&, U>::value
            && !std::is_constructible<T, optional<U> &          >::value
            && !std::is_constructible<T, optional<U> &&         >::value
            && !std::is_constructible<T, optional<U> const &    >::value
            && !std::is_constructible<T, optional<U> const &&   >::value
            && !std::is_convertible<     optional<U> &       , T>::value
            && !std::is_convertible<     optional<U> &&      , T>::value
            && !std::is_convertible<     optional<U> const & , T>::value
            && !std::is_convertible<     optional<U> const &&, T>::value
            && !std::is_assignable<  T&, optional<U> &          >::value
            && !std::is_assignable<  T&, optional<U> &&         >::value
            && !std::is_assignable<  T&, optional<U> const &    >::value
            && !std::is_assignable<  T&, optional<U> const &&   >::value
        )
    operator=( optional<U> && other )
    {
        return *this = optional( std::move( other ) );
    }

    // 7 (C++11) - emplace
    template< typename... Args
        optional_REQUIRES_T(
            std::is_constructible<T, Args&&...>::value
        )
    >
    T& emplace( Args&&... args )
    {
        *this = nullopt;
        contained.emplace( std::forward<Args>(args)...  );
        has_value_ = true;
        return contained.value();
    }

    // 8 (C++11) - emplace, initializer-list
    template< typename U, typename... Args
        optional_REQUIRES_T(
            std::is_constructible<T, std::initializer_list<U>&, Args&&...>::value
        )
    >
    T& emplace( std::initializer_list<U> il, Args&&... args )
    {
        *this = nullopt;
        contained.emplace( il, std::forward<Args>(args)...  );
        has_value_ = true;
        return contained.value();
    }

#endif // optional_CPP11_OR_GREATER

    // x.x.3.4, swap

    void swap( optional & other )
#if optional_CPP11_OR_GREATER
        noexcept(
            std::is_nothrow_move_constructible<T>::value
            && std17::is_nothrow_swappable<T>::value
        )
#endif
    {
        using std::swap;
        if      ( has_value() == true  && other.has_value() == true  ) { swap( **this, *other ); }
        else if ( has_value() == false && other.has_value() == true  ) { initialize( std11::move(*other) ); other.reset(); }
        else if ( has_value() == true  && other.has_value() == false ) { other.initialize( std11::move(**this) ); reset(); }
    }

    // x.x.3.5, observers

    optional_constexpr value_type const * operator ->() const
    {
        return assert( has_value() ),
            contained.value_ptr();
    }

    optional_constexpr14 value_type * operator ->()
    {
        return assert( has_value() ),
            contained.value_ptr();
    }

    optional_constexpr value_type const & operator *() const optional_ref_qual
    {
        return assert( has_value() ),
            contained.value();
    }

    optional_constexpr14 value_type & operator *() optional_ref_qual
    {
        return assert( has_value() ),
            contained.value();
    }

#if optional_HAVE( REF_QUALIFIER )  &&  ( !optional_COMPILER_GNUC_VERSION || optional_COMPILER_GNUC_VERSION >= 490 )

    optional_constexpr value_type const && operator *() const optional_refref_qual
    {
        return std::move( **this );
    }

    optional_constexpr14 value_type && operator *() optional_refref_qual
    {
        return std::move( **this );
    }

#endif

#if optional_CPP11_OR_GREATER
    optional_constexpr explicit operator bool() const optional_noexcept
    {
        return has_value();
    }
#else
    optional_constexpr operator safe_bool() const optional_noexcept
    {
        return has_value() ? &optional::this_type_does_not_support_comparisons : 0;
    }
#endif

    optional_constexpr bool has_value() const optional_noexcept
    {
        return has_value_;
    }

    optional_constexpr14 value_type const & value() const optional_ref_qual
    {
#if optional_CONFIG_NO_EXCEPTIONS
        assert( has_value() );
#else
        if ( ! has_value() )
            throw bad_optional_access();
#endif
        return contained.value();
    }

    optional_constexpr14 value_type & value() optional_ref_qual
    {
#if optional_CONFIG_NO_EXCEPTIONS
        assert( has_value() );
#else
        if ( ! has_value() )
            throw bad_optional_access();
#endif
        return contained.value();
    }

#if optional_HAVE( REF_QUALIFIER )  &&  ( !optional_COMPILER_GNUC_VERSION || optional_COMPILER_GNUC_VERSION >= 490 )

    optional_constexpr value_type const && value() const optional_refref_qual
    {
        return std::move( value() );
    }

    optional_constexpr14 value_type && value() optional_refref_qual
    {
        return std::move( value() );
    }

#endif

#if optional_CPP11_OR_GREATER

    template< typename U >
    optional_constexpr value_type value_or( U && v ) const optional_ref_qual
    {
        return has_value() ? contained.value() : static_cast<T>(std::forward<U>( v ) );
    }

    template< typename U >
    optional_constexpr14 value_type value_or( U && v ) optional_refref_qual
    {
        return has_value() ? std::move( contained.value() ) : static_cast<T>(std::forward<U>( v ) );
    }

#else

    template< typename U >
    optional_constexpr value_type value_or( U const & v ) const
    {
        return has_value() ? contained.value() : static_cast<value_type>( v );
    }

#endif // optional_CPP11_OR_GREATER

    // x.x.3.6, modifiers

    void reset() optional_noexcept
    {
        if ( has_value() )
            contained.destruct_value();

        has_value_ = false;
    }

private:
    void this_type_does_not_support_comparisons() const {}

    template< typename V >
    void initialize( V const & value )
    {
        assert( ! has_value()  );
        contained.construct_value( value );
        has_value_ = true;
    }

#if optional_CPP11_OR_GREATER
    template< typename V >
    void initialize( V && value )
    {
        assert( ! has_value()  );
        contained.construct_value( std::move( value ) );
        has_value_ = true;
    }

#endif

private:
    bool has_value_;
    detail::storage_t< value_type > contained;

};

// Relational operators

template< typename T, typename U >
inline optional_constexpr bool operator==( optional<T> const & x, optional<U> const & y )
{
    return bool(x) != bool(y) ? false : !bool( x ) ? true : *x == *y;
}

template< typename T, typename U >
inline optional_constexpr bool operator!=( optional<T> const & x, optional<U> const & y )
{
    return !(x == y);
}

template< typename T, typename U >
inline optional_constexpr bool operator<( optional<T> const & x, optional<U> const & y )
{
    return (!y) ? false : (!x) ? true : *x < *y;
}

template< typename T, typename U >
inline optional_constexpr bool operator>( optional<T> const & x, optional<U> const & y )
{
    return (y < x);
}

template< typename T, typename U >
inline optional_constexpr bool operator<=( optional<T> const & x, optional<U> const & y )
{
    return !(y < x);
}

template< typename T, typename U >
inline optional_constexpr bool operator>=( optional<T> const & x, optional<U> const & y )
{
    return !(x < y);
}

// Comparison with nullopt

template< typename T >
inline optional_constexpr bool operator==( optional<T> const & x, nullopt_t ) optional_noexcept
{
    return (!x);
}

template< typename T >
inline optional_constexpr bool operator==( nullopt_t, optional<T> const & x ) optional_noexcept
{
    return (!x);
}

template< typename T >
inline optional_constexpr bool operator!=( optional<T> const & x, nullopt_t ) optional_noexcept
{
    return bool(x);
}

template< typename T >
inline optional_constexpr bool operator!=( nullopt_t, optional<T> const & x ) optional_noexcept
{
    return bool(x);
}

template< typename T >
inline optional_constexpr bool operator<( optional<T> const &, nullopt_t ) optional_noexcept
{
    return false;
}

template< typename T >
inline optional_constexpr bool operator<( nullopt_t, optional<T> const & x ) optional_noexcept
{
    return bool(x);
}

template< typename T >
inline optional_constexpr bool operator<=( optional<T> const & x, nullopt_t ) optional_noexcept
{
    return (!x);
}

template< typename T >
inline optional_constexpr bool operator<=( nullopt_t, optional<T> const & ) optional_noexcept
{
    return true;
}

template< typename T >
inline optional_constexpr bool operator>( optional<T> const & x, nullopt_t ) optional_noexcept
{
    return bool(x);
}

template< typename T >
inline optional_constexpr bool operator>( nullopt_t, optional<T> const & ) optional_noexcept
{
    return false;
}

template< typename T >
inline optional_constexpr bool operator>=( optional<T> const &, nullopt_t ) optional_noexcept
{
    return true;
}

template< typename T >
inline optional_constexpr bool operator>=( nullopt_t, optional<T> const & x ) optional_noexcept
{
    return (!x);
}

// Comparison with T

template< typename T, typename U >
inline optional_constexpr bool operator==( optional<T> const & x, U const & v )
{
    return bool(x) ? *x == v : false;
}

template< typename T, typename U >
inline optional_constexpr bool operator==( U const & v, optional<T> const & x )
{
    return bool(x) ? v == *x : false;
}

template< typename T, typename U >
inline optional_constexpr bool operator!=( optional<T> const & x, U const & v )
{
    return bool(x) ? *x != v : true;
}

template< typename T, typename U >
inline optional_constexpr bool operator!=( U const & v, optional<T> const & x )
{
    return bool(x) ? v != *x : true;
}

template< typename T, typename U >
inline optional_constexpr bool operator<( optional<T> const & x, U const & v )
{
    return bool(x) ? *x < v : true;
}

template< typename T, typename U >
inline optional_constexpr bool operator<( U const & v, optional<T> const & x )
{
    return bool(x) ? v < *x : false;
}

template< typename T, typename U >
inline optional_constexpr bool operator<=( optional<T> const & x, U const & v )
{
    return bool(x) ? *x <= v : true;
}

template< typename T, typename U >
inline optional_constexpr bool operator<=( U const & v, optional<T> const & x )
{
    return bool(x) ? v <= *x : false;
}

template< typename T, typename U >
inline optional_constexpr bool operator>( optional<T> const & x, U const & v )
{
    return bool(x) ? *x > v : false;
}

template< typename T, typename U >
inline optional_constexpr bool operator>( U const & v, optional<T> const & x )
{
    return bool(x) ? v > *x : true;
}

template< typename T, typename U >
inline optional_constexpr bool operator>=( optional<T> const & x, U const & v )
{
    return bool(x) ? *x >= v : false;
}

template< typename T, typename U >
inline optional_constexpr bool operator>=( U const & v, optional<T> const & x )
{
    return bool(x) ? v >= *x : true;
}

// Specialized algorithms

template< typename T
#if optional_CPP11_OR_GREATER
    optional_REQUIRES_T(
        std::is_move_constructible<T>::value
        && std17::is_swappable<T>::value )
#endif
>
void swap( optional<T> & x, optional<T> & y )
#if optional_CPP11_OR_GREATER
    noexcept( noexcept( x.swap(y) ) )
#endif
{
    x.swap( y );
}

#if optional_CPP11_OR_GREATER

template< typename T >
optional_constexpr optional< typename std::decay<T>::type > make_optional( T && value )
{
    return optional< typename std::decay<T>::type >( std::forward<T>( value ) );
}

template< typename T, typename...Args >
optional_constexpr optional<T> make_optional( Args&&... args )
{
    return optional<T>( nonstd_lite_in_place(T), std::forward<Args>(args)...);
}

template< typename T, typename U, typename... Args >
optional_constexpr optional<T> make_optional( std::initializer_list<U> il, Args&&... args )
{
    return optional<T>( nonstd_lite_in_place(T), il, std::forward<Args>(args)...);
}

#else

template< typename T >
optional<T> make_optional( T const & value )
{
    return optional<T>( value );
}

#endif // optional_CPP11_OR_GREATER

} // namespace optional_lite

using namespace optional_lite;

} // namespace nonstd

#if optional_CPP11_OR_GREATER

// specialize the std::hash algorithm:

namespace std {

template< class T >
struct hash< nonstd::optional<T> >
{
public:
    std::size_t operator()( nonstd::optional<T> const & v ) const optional_noexcept
    {
        return bool( v ) ? std::hash<T>{}( *v ) : 0;
    }
};

} //namespace std

#endif // optional_CPP11_OR_GREATER

#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined(__GNUC__)
# pragma GCC   diagnostic pop
#elif defined(_MSC_VER )
# pragma warning( pop )
#endif

#endif // optional_USES_STD_OPTIONAL

#endif // NONSTD_OPTIONAL_LITE_HPP


#ifndef INCLUDE_LAZYCODE_UTILS_H_
#define INCLUDE_LAZYCODE_UTILS_H_
#include <functional>
#include <sstream>
#include <tuple>
#include <utility>

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
 * which case the collector stores the container and returns it after appending
 * the values. Otherwise, only a reference to the container is held by the
 * collector.  If the generator is not specified, a collector is returned.
 * Collectors remember the operation to be executed. The operation is executed
 * when a generator is piped `|` to it.
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
 * inline (`append(set<int>())`), in
 * which case the collector stores the container and returns it after inserting
 * the values. Otherwise, only a reference to the container is held by the
 * collector. If the generator is not specified, a collector is returned.
 * Collectors remember the operation to be executed. The operation is executed
 * when a generator is piped `|` to it.
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

#ifndef LAZYCODE_BASICGENERATORS_H_
#define LAZYCODE_BASICGENERATORS_H_
#include <iostream>


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

#endif /* LAZYCODE_LAZYCODE_H_*/

