#ifndef LAZYCODE_LAZYCODE_H_
#define LAZYCODE_LAZYCODE_H_
#ifndef LAZYCODE_BASICCOLLECTORS_H_
#define LAZYCODE_BASICCOLLECTORS_H_
#ifndef LAZYCODE_COLLECTOR_H_
#define LAZYCODE_COLLECTOR_H_
#include <iostream>
#include <type_traits>
#include <utility>
#ifndef LAZYCODE_GENERATOR_H_
#define LAZYCODE_GENERATOR_H_
#include <type_traits>
#include <utility>
// Copyright (C) 2011 - 2012 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea and interface is based on Boost.Optional library
// authored by Fernando Luis Cacciola Carballal

# ifndef ___OPTIONAL_HPP___
# define ___OPTIONAL_HPP___

# include <utility>
# include <type_traits>
# include <initializer_list>
# include <cassert>
# include <functional>
# include <string>
# include <stdexcept>

# define TR2_OPTIONAL_REQUIRES(...) typename enable_if<__VA_ARGS__::value, bool>::type = false

# if defined __GNUC__ // NOTE: GNUC is also defined for Clang
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ >= 8)
#     define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#   endif
#
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)
#     define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#   endif
#
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ == 8) && (__GNUC_PATCHLEVEL__ >= 1)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   elif (__GNUC__ == 4) && (__GNUC_MINOR__ >= 9)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   endif
# endif
#
# if defined __clang_major__
#   if (__clang_major__ == 3 && __clang_minor__ >= 5)
#     define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#   elif (__clang_major__ > 3)
#     define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#   endif
#   if defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#     define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#   elif (__clang_major__ == 3 && __clang_minor__ == 4 && __clang_patchlevel__ >= 2)
#     define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#   endif
# endif
#
# if defined _MSC_VER
#   if (_MSC_VER >= 1900)
#     define TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#   endif
# endif

# if defined __clang__
#   if (__clang_major__ > 2) || (__clang_major__ == 2) && (__clang_minor__ >= 9)
#     define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#   else
#     define OPTIONAL_HAS_THIS_RVALUE_REFS 0
#   endif
# elif defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 1
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 1
# else
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 0
# endif


# if defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 1
#   define OPTIONAL_CONSTEXPR_INIT_LIST constexpr
# else
#   define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 0
#   define OPTIONAL_CONSTEXPR_INIT_LIST
# endif

# if defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_ && (defined __cplusplus) && (__cplusplus != 201103L)
#   define OPTIONAL_HAS_MOVE_ACCESSORS 1
# else
#   define OPTIONAL_HAS_MOVE_ACCESSORS 0
# endif

# // In C++11 constexpr implies const, so we need to make non-const members also non-constexpr
# if (defined __cplusplus) && (__cplusplus == 201103L)
#   define OPTIONAL_MUTABLE_CONSTEXPR
# else
#   define OPTIONAL_MUTABLE_CONSTEXPR constexpr
# endif

namespace std{

namespace experimental{

// BEGIN workaround for missing is_trivially_destructible
# if defined TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
    // leave it: it is already there
# elif defined TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
    // leave it: it is already there
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
    // leave it: it is already there
# elif defined TR2_OPTIONAL_DISABLE_EMULATION_OF_TYPE_TRAITS
    // leave it: the user doesn't want it
# else
	template <typename T>
	using is_trivially_destructible = std::has_trivial_destructor<T>;
# endif
// END workaround for missing is_trivially_destructible

# if (defined TR2_OPTIONAL_GCC_4_7_AND_HIGHER___)
    // leave it; our metafunctions are already defined.
# elif defined TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
    // leave it; our metafunctions are already defined.
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
    // leave it: it is already there
# elif defined TR2_OPTIONAL_DISABLE_EMULATION_OF_TYPE_TRAITS
    // leave it: the user doesn't want it
# else


// workaround for missing traits in GCC and CLANG
template <class T>
struct is_nothrow_move_constructible
{
  constexpr static bool value = std::is_nothrow_constructible<T, T&&>::value;
};


template <class T, class U>
struct is_assignable
{
  template <class X, class Y>
  constexpr static bool has_assign(...) { return false; }

  template <class X, class Y, size_t S = sizeof((std::declval<X>() = std::declval<Y>(), true)) >
  // the comma operator is necessary for the cases where operator= returns void
  constexpr static bool has_assign(bool) { return true; }

  constexpr static bool value = has_assign<T, U>(true);
};


template <class T>
struct is_nothrow_move_assignable
{
  template <class X, bool has_any_move_assign>
  struct has_nothrow_move_assign {
    constexpr static bool value = false;
  };

  template <class X>
  struct has_nothrow_move_assign<X, true> {
    constexpr static bool value = noexcept( std::declval<X&>() = std::declval<X&&>() );
  };

  constexpr static bool value = has_nothrow_move_assign<T, is_assignable<T&, T&&>::value>::value;
};
// end workaround


# endif



// 20.5.4, optional for object types
template <class T> class optional;

// 20.5.5, optional for lvalue reference types
template <class T> class optional<T&>;


// workaround: std utility functions aren't constexpr yet
template <class T> inline constexpr T&& constexpr_forward(typename std::remove_reference<T>::type& t) noexcept
{
  return static_cast<T&&>(t);
}

template <class T> inline constexpr T&& constexpr_forward(typename std::remove_reference<T>::type&& t) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "!!");
    return static_cast<T&&>(t);
}

template <class T> inline constexpr typename std::remove_reference<T>::type&& constexpr_move(T&& t) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}


#if defined NDEBUG
# define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) (EXPR)
#else
# define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) ((CHECK) ? (EXPR) : ([]{assert(!#CHECK);}(), (EXPR)))
#endif


namespace detail_
{

// static_addressof: a constexpr version of addressof
template <typename T>
struct has_overloaded_addressof
{
  template <class X>
  constexpr static bool has_overload(...) { return false; }
  
  template <class X, size_t S = sizeof(std::declval<X&>().operator&()) >
  constexpr static bool has_overload(bool) { return true; }

  constexpr static bool value = has_overload<T>(true);
};

template <typename T, TR2_OPTIONAL_REQUIRES(!has_overloaded_addressof<T>)>
constexpr T* static_addressof(T& ref)
{
  return &ref;
}

template <typename T, TR2_OPTIONAL_REQUIRES(has_overloaded_addressof<T>)>
T* static_addressof(T& ref)
{
  return std::addressof(ref);
}


// the call to convert<A>(b) has return type A and converts b to type A iff b decltype(b) is implicitly convertible to A  
template <class U>
constexpr U convert(U v) { return v; }
  

namespace swap_ns
{
  using std::swap;
    
  template <class T>
  void adl_swap(T& t, T& u) noexcept(noexcept(swap(t, u)))
  {
    swap(t, u);
  }

} // namespace swap_ns

} // namespace detail


constexpr struct trivial_init_t{} trivial_init{};


// 20.5.6, In-place construction
constexpr struct in_place_t{} in_place{};


// 20.5.7, Disengaged state indicator
struct nullopt_t
{
  struct init{};
  constexpr explicit nullopt_t(init){}
};
constexpr nullopt_t nullopt{nullopt_t::init()};


// 20.5.8, class bad_optional_access
class bad_optional_access : public logic_error {
public:
  explicit bad_optional_access(const string& what_arg) : logic_error{what_arg} {}
  explicit bad_optional_access(const char* what_arg) : logic_error{what_arg} {}
};


template <class T>
union storage_t
{
  unsigned char dummy_;
  T value_;

  constexpr storage_t( trivial_init_t ) noexcept : dummy_() {};

  template <class... Args>
  constexpr storage_t( Args&&... args ) : value_(constexpr_forward<Args>(args)...) {}

  ~storage_t(){}
};


template <class T>
union constexpr_storage_t
{
    unsigned char dummy_;
    T value_;

    constexpr constexpr_storage_t( trivial_init_t ) noexcept : dummy_() {};

    template <class... Args>
    constexpr constexpr_storage_t( Args&&... args ) : value_(constexpr_forward<Args>(args)...) {}

    ~constexpr_storage_t() = default;
};


template <class T>
struct optional_base
{
    bool init_;
    storage_t<T> storage_;

    constexpr optional_base() noexcept : init_(false), storage_(trivial_init) {};

    explicit constexpr optional_base(const T& v) : init_(true), storage_(v) {}

    explicit constexpr optional_base(T&& v) : init_(true), storage_(constexpr_move(v)) {}

    template <class... Args> explicit optional_base(in_place_t, Args&&... args)
        : init_(true), storage_(constexpr_forward<Args>(args)...) {}

    template <class U, class... Args, TR2_OPTIONAL_REQUIRES(is_constructible<T, std::initializer_list<U>>)>
    explicit optional_base(in_place_t, std::initializer_list<U> il, Args&&... args)
        : init_(true), storage_(il, std::forward<Args>(args)...) {}

    ~optional_base() { if (init_) storage_.value_.T::~T(); }
};


template <class T>
struct constexpr_optional_base
{
    bool init_;
    constexpr_storage_t<T> storage_;

    constexpr constexpr_optional_base() noexcept : init_(false), storage_(trivial_init) {};

    explicit constexpr constexpr_optional_base(const T& v) : init_(true), storage_(v) {}

    explicit constexpr constexpr_optional_base(T&& v) : init_(true), storage_(constexpr_move(v)) {}

    template <class... Args> explicit constexpr constexpr_optional_base(in_place_t, Args&&... args)
      : init_(true), storage_(constexpr_forward<Args>(args)...) {}

    template <class U, class... Args, TR2_OPTIONAL_REQUIRES(is_constructible<T, std::initializer_list<U>>)>
    OPTIONAL_CONSTEXPR_INIT_LIST explicit constexpr_optional_base(in_place_t, std::initializer_list<U> il, Args&&... args)
      : init_(true), storage_(il, std::forward<Args>(args)...) {}

    ~constexpr_optional_base() = default;
};

template <class T>
using OptionalBase = typename std::conditional<
    is_trivially_destructible<T>::value,                          // if possible
    constexpr_optional_base<typename std::remove_const<T>::type>, // use base with trivial destructor
    optional_base<typename std::remove_const<T>::type>
>::type;



template <class T>
class optional : private OptionalBase<T>
{
  static_assert( !std::is_same<typename std::decay<T>::type, nullopt_t>::value, "bad T" );
  static_assert( !std::is_same<typename std::decay<T>::type, in_place_t>::value, "bad T" );
  

  constexpr bool initialized() const noexcept { return OptionalBase<T>::init_; }
  typename std::remove_const<T>::type* dataptr() {  return std::addressof(OptionalBase<T>::storage_.value_); }
  constexpr const T* dataptr() const { return detail_::static_addressof(OptionalBase<T>::storage_.value_); }
  
# if OPTIONAL_HAS_THIS_RVALUE_REFS == 1
  constexpr const T& contained_val() const& { return OptionalBase<T>::storage_.value_; }
#   if OPTIONAL_HAS_MOVE_ACCESSORS == 1
  OPTIONAL_MUTABLE_CONSTEXPR T&& contained_val() && { return std::move(OptionalBase<T>::storage_.value_); }
  OPTIONAL_MUTABLE_CONSTEXPR T& contained_val() & { return OptionalBase<T>::storage_.value_; }
#   else
  T& contained_val() & { return OptionalBase<T>::storage_.value_; }
  T&& contained_val() && { return std::move(OptionalBase<T>::storage_.value_); }
#   endif
# else
  constexpr const T& contained_val() const { return OptionalBase<T>::storage_.value_; }
  T& contained_val() { return OptionalBase<T>::storage_.value_; }
# endif

  void clear() noexcept {
    if (initialized()) dataptr()->T::~T();
    OptionalBase<T>::init_ = false;
  }
  
  template <class... Args>
  void initialize(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
  {
    assert(!OptionalBase<T>::init_);
    ::new (static_cast<void*>(dataptr())) T(std::forward<Args>(args)...);
    OptionalBase<T>::init_ = true;
  }

  template <class U, class... Args>
  void initialize(std::initializer_list<U> il, Args&&... args) noexcept(noexcept(T(il, std::forward<Args>(args)...)))
  {
    assert(!OptionalBase<T>::init_);
    ::new (static_cast<void*>(dataptr())) T(il, std::forward<Args>(args)...);
    OptionalBase<T>::init_ = true;
  }

public:
  typedef T value_type;

  // 20.5.5.1, constructors
  constexpr optional() noexcept : OptionalBase<T>()  {};
  constexpr optional(nullopt_t) noexcept : OptionalBase<T>() {};

  optional(const optional& rhs)
  : OptionalBase<T>()
  {
    if (rhs.initialized()) {
        ::new (static_cast<void*>(dataptr())) T(*rhs);
        OptionalBase<T>::init_ = true;
    }
  }

  optional(optional&& rhs) noexcept(is_nothrow_move_constructible<T>::value)
  : OptionalBase<T>()
  {
    if (rhs.initialized()) {
        ::new (static_cast<void*>(dataptr())) T(std::move(*rhs));
        OptionalBase<T>::init_ = true;
    }
  }

  constexpr optional(const T& v) : OptionalBase<T>(v) {}

  constexpr optional(T&& v) : OptionalBase<T>(constexpr_move(v)) {}

  template <class... Args>
  explicit constexpr optional(in_place_t, Args&&... args)
  : OptionalBase<T>(in_place_t{}, constexpr_forward<Args>(args)...) {}

  template <class U, class... Args, TR2_OPTIONAL_REQUIRES(is_constructible<T, std::initializer_list<U>>)>
  OPTIONAL_CONSTEXPR_INIT_LIST explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args)
  : OptionalBase<T>(in_place_t{}, il, constexpr_forward<Args>(args)...) {}

  // 20.5.4.2, Destructor
  ~optional() = default;

  // 20.5.4.3, assignment
  optional& operator=(nullopt_t) noexcept
  {
    clear();
    return *this;
  }
  
  optional& operator=(const optional& rhs)
  {
    if      (initialized() == true  && rhs.initialized() == false) clear();
    else if (initialized() == false && rhs.initialized() == true)  initialize(*rhs);
    else if (initialized() == true  && rhs.initialized() == true)  contained_val() = *rhs;
    return *this;
  }
  
  optional& operator=(optional&& rhs)
  noexcept(is_nothrow_move_assignable<T>::value && is_nothrow_move_constructible<T>::value)
  {
    if      (initialized() == true  && rhs.initialized() == false) clear();
    else if (initialized() == false && rhs.initialized() == true)  initialize(std::move(*rhs));
    else if (initialized() == true  && rhs.initialized() == true)  contained_val() = std::move(*rhs);
    return *this;
  }

  template <class U>
  auto operator=(U&& v)
  -> typename enable_if
  <
    is_same<typename decay<U>::type, T>::value,
    optional&
  >::type
  {
    if (initialized()) { contained_val() = std::forward<U>(v); }
    else               { initialize(std::forward<U>(v));  }
    return *this;
  }
  
  
  template <class... Args>
  void emplace(Args&&... args)
  {
    clear();
    initialize(std::forward<Args>(args)...);
  }
  
  template <class U, class... Args>
  void emplace(initializer_list<U> il, Args&&... args)
  {
    clear();
    initialize<U, Args...>(il, std::forward<Args>(args)...);
  }
  
  // 20.5.4.4, Swap
  void swap(optional<T>& rhs) noexcept(is_nothrow_move_constructible<T>::value
                                       && noexcept(detail_::swap_ns::adl_swap(declval<T&>(), declval<T&>())))
  {
    if      (initialized() == true  && rhs.initialized() == false) { rhs.initialize(std::move(**this)); clear(); }
    else if (initialized() == false && rhs.initialized() == true)  { initialize(std::move(*rhs)); rhs.clear(); }
    else if (initialized() == true  && rhs.initialized() == true)  { using std::swap; swap(**this, *rhs); }
  }

  // 20.5.4.5, Observers
  
  explicit constexpr operator bool() const noexcept { return initialized(); }
  constexpr bool has_value() const noexcept { return initialized(); }
  
  constexpr T const* operator ->() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), dataptr());
  }
  
# if OPTIONAL_HAS_MOVE_ACCESSORS == 1

  OPTIONAL_MUTABLE_CONSTEXPR T* operator ->() {
    assert (initialized());
    return dataptr();
  }
  
  constexpr T const& operator *() const& {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T& operator *() & {
    assert (initialized());
    return contained_val();
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T&& operator *() && {
    assert (initialized());
    return constexpr_move(contained_val());
  }

  constexpr T const& value() const& {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T& value() & {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T&& value() && {
    if (!initialized()) throw bad_optional_access("bad optional access");
	return std::move(contained_val());
  }
  
# else

  T* operator ->() {
    assert (initialized());
    return dataptr();
  }
  
  constexpr T const& operator *() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
  }
  
  T& operator *() {
    assert (initialized());
    return contained_val();
  }
  
  constexpr T const& value() const {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  T& value() {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
# endif
  
# if OPTIONAL_HAS_THIS_RVALUE_REFS == 1

  template <class V>
  constexpr T value_or(V&& v) const&
  {
    return *this ? **this : detail_::convert<T>(constexpr_forward<V>(v));
  }
  
#   if OPTIONAL_HAS_MOVE_ACCESSORS == 1

  template <class V>
  OPTIONAL_MUTABLE_CONSTEXPR T value_or(V&& v) &&
  {
    return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail_::convert<T>(constexpr_forward<V>(v));
  }

#   else
 
  template <class V>
  T value_or(V&& v) &&
  {
    return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail_::convert<T>(constexpr_forward<V>(v));
  }
  
#   endif
  
# else
  
  template <class V>
  constexpr T value_or(V&& v) const
  {
    return *this ? **this : detail_::convert<T>(constexpr_forward<V>(v));
  }

# endif

  // 20.6.3.6, modifiers
  void reset() noexcept { clear(); }
};


template <class T>
class optional<T&>
{
  static_assert( !std::is_same<T, nullopt_t>::value, "bad T" );
  static_assert( !std::is_same<T, in_place_t>::value, "bad T" );
  T* ref;
  
public:

  // 20.5.5.1, construction/destruction
  constexpr optional() noexcept : ref(nullptr) {}
  
  constexpr optional(nullopt_t) noexcept : ref(nullptr) {}
   
  constexpr optional(T& v) noexcept : ref(detail_::static_addressof(v)) {}
  
  optional(T&&) = delete;
  
  constexpr optional(const optional& rhs) noexcept : ref(rhs.ref) {}
  
  explicit constexpr optional(in_place_t, T& v) noexcept : ref(detail_::static_addressof(v)) {}
  
  explicit optional(in_place_t, T&&) = delete;
  
  ~optional() = default;
  
  // 20.5.5.2, mutation
  optional& operator=(nullopt_t) noexcept {
    ref = nullptr;
    return *this;
  }
  
  // optional& operator=(const optional& rhs) noexcept {
    // ref = rhs.ref;
    // return *this;
  // }
  
  // optional& operator=(optional&& rhs) noexcept {
    // ref = rhs.ref;
    // return *this;
  // }
  
  template <typename U>
  auto operator=(U&& rhs) noexcept
  -> typename enable_if
  <
    is_same<typename decay<U>::type, optional<T&>>::value,
    optional&
  >::type
  {
    ref = rhs.ref;
    return *this;
  }
  
  template <typename U>
  auto operator=(U&& rhs) noexcept
  -> typename enable_if
  <
    !is_same<typename decay<U>::type, optional<T&>>::value,
    optional&
  >::type
  = delete;
  
  void emplace(T& v) noexcept {
    ref = detail_::static_addressof(v);
  }
  
  void emplace(T&&) = delete;
  
  
  void swap(optional<T&>& rhs) noexcept
  {
    std::swap(ref, rhs.ref);
  }
    
  // 20.5.5.3, observers
  constexpr T* operator->() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, ref);
  }
  
  constexpr T& operator*() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, *ref);
  }
  
  constexpr T& value() const {
    return ref ? *ref : (throw bad_optional_access("bad optional access"), *ref);
  }
  
  explicit constexpr operator bool() const noexcept {
    return ref != nullptr;
  }
 
  constexpr bool has_value() const noexcept {
    return ref != nullptr;
  }
  
  template <class V>
  constexpr typename decay<T>::type value_or(V&& v) const
  {
    return *this ? **this : detail_::convert<typename decay<T>::type>(constexpr_forward<V>(v));
  }

  // x.x.x.x, modifiers
  void reset() noexcept { ref = nullptr; }
};


template <class T>
class optional<T&&>
{
  static_assert( sizeof(T) == 0, "optional rvalue references disallowed" );
};


// 20.5.8, Relational operators
template <class T> constexpr bool operator==(const optional<T>& x, const optional<T>& y)
{
  return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
}

template <class T> constexpr bool operator!=(const optional<T>& x, const optional<T>& y)
{
  return !(x == y);
}

template <class T> constexpr bool operator<(const optional<T>& x, const optional<T>& y)
{
  return (!y) ? false : (!x) ? true : *x < *y;
}

template <class T> constexpr bool operator>(const optional<T>& x, const optional<T>& y)
{
  return (y < x);
}

template <class T> constexpr bool operator<=(const optional<T>& x, const optional<T>& y)
{
  return !(y < x);
}

template <class T> constexpr bool operator>=(const optional<T>& x, const optional<T>& y)
{
  return !(x < y);
}


// 20.5.9, Comparison with nullopt
template <class T> constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator==(nullopt_t, const optional<T>& x) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator!=(const optional<T>& x, nullopt_t) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator!=(nullopt_t, const optional<T>& x) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator<(const optional<T>&, nullopt_t) noexcept
{
  return false;
}

template <class T> constexpr bool operator<(nullopt_t, const optional<T>& x) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator<=(const optional<T>& x, nullopt_t) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept
{
  return true;
}

template <class T> constexpr bool operator>(const optional<T>& x, nullopt_t) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator>(nullopt_t, const optional<T>&) noexcept
{
  return false;
}

template <class T> constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept
{
  return true;
}

template <class T> constexpr bool operator>=(nullopt_t, const optional<T>& x) noexcept
{
  return (!x);
}



// 20.5.10, Comparison with T
template <class T> constexpr bool operator==(const optional<T>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<T>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<T>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<T>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<T>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<T>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<T>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<T>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<T>& x)
{
  return bool(x) ? v >= *x : true;
}


// Comparison of optional<T&> with T
template <class T> constexpr bool operator==(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<T&>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<T&>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<T&>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v >= *x : true;
}

// Comparison of optional<T const&> with T
template <class T> constexpr bool operator==(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v >= *x : true;
}


// 20.5.12, Specialized algorithms
template <class T>
void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)))
{
  x.swap(y);
}


template <class T>
constexpr optional<typename decay<T>::type> make_optional(T&& v)
{
  return optional<typename decay<T>::type>(constexpr_forward<T>(v));
}

template <class X>
constexpr optional<X&> make_optional(reference_wrapper<X> v)
{
  return optional<X&>(v.get());
}


} // namespace experimental
} // namespace std

namespace std
{
  template <typename T>
  struct hash<std::experimental::optional<T>>
  {
    typedef typename hash<T>::result_type result_type;
    typedef std::experimental::optional<T> argument_type;
    
    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };
  
  template <typename T>
  struct hash<std::experimental::optional<T&>>
  {
    typedef typename hash<T>::result_type result_type;
    typedef std::experimental::optional<T&> argument_type;
    
    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };
}

# undef TR2_OPTIONAL_REQUIRES
# undef TR2_OPTIONAL_ASSERTED_EXPRESSION

# endif //___OPTIONAL_HPP___


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

using std::experimental::make_optional;
using std::experimental::nullopt;
using std::experimental::optional;

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
 * produced by another generator paired with the number of elements that have
 * been yielded before.  Each yielded item will be pair(count,value).  If the
 * generator is not specified, a GeneratorBuilder is returned.
 * GeneratorBuilders are converted to generators by piping `|` a generator to
 * them.
 */
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) enumerate(Generator&& gen = GeneratorHole()) {
    return buildOrDelay(generatorBuilder([](auto&& gen) mutable {
                            typedef decltype(gen) Gen;
                            return zip(infRange(0, 1), std::forward<Gen>(gen));
                        }),
                        std::forward<Generator>(gen));
}

/*
 * return a generator that reads from the given stream.  The generated type (the
 * type of values pulled from the stream) must be specified as the first
 * template parameter.  For example, to read integers from the stream, use
 * `read<int>`.  */
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
 * yields a new string for each line.*/
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

