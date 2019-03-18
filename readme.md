# Lazy Code
# Cute and Efficient Is Definitely Possible 

*  Compossible, lazily evaluated generators such as map, filter, fold, enumerated ranges and more.
*  Easy handling of input and output streams, reading line by line, parsing objects, etc.
*  Compact syntax:  Choice between
    *  Functional `fold(...,map(...,filter(...)))`
    *  Piped `filter(...) | map(...) | fold(...)`
*  Zero cost abstractions leveraging C++ templates, no macros involved.
*  Easily create new generators and integrate with existing ones.
*  _Optionally_ enabled  macros to make the syntax even more cute.


# Quick examples:

This is just some demonstrations of the coding style enabled with this library.  A full listing comes after the examples.


```c++
namespace lz = LazyCode;
using namespace std;
```

## Example 1:

*  Read in lines of text into a vector until EOF is reached, one line per vector element.
*  Sort the vector and then reprint the lines.

```c++
    auto lines = lz::readLines(cin) | lz::append(vector<string>());
    sort(lines.begin(), lines.end());
    lz::generator(lines) | lz::write(cout, "\n");
```

Yup, that's it.  And it gets better...

## Example 2:

*  Read in up to 10 integers from a file "test.txt".  
*  filter for the even numbers, square them and sum their values.
*  Do not store the numbers in a container, after all, it could easily be more than 10 numbers.

```c++
    int total = lz::read<int>(ifstream("test.txt")) | lz::limit(10) |
                lz::filter([](int i) { return i % 2 == 0; }) |
                lz::map([](int i) { return i *  i; }) | lz::sum();
```

__Wow__, that's compact.  Maybe too compact? If you are concerned, you can split that line up into multiple expressions.  Take a look:

```c++
    auto numbers = lz::read<int>(ifstream("test.txt")) | lz::limit(10);
    auto evenFilter = numbers | lz::filter([](int i) { return i % 2 == 0; });
    auto squares = evenFilter | lz::map([](int i) { return i *  i; });
    int total = squares | lz::sum();
```

*  Even though this expression is split over multiple variable assignments, it is not any less efficient.
*  Each intermediate variable simply 
describes a unit of code to be executed.  All held in stack.  Nothing is executed until the final pipe into the `sum()`.
*  The final value (the sum of the squares of only even numbers in a file) is calculated in a single pass with no intermediate container or memory allocations required.
*  for each number in `test.txt`, evaluate `filter condition`, and add to `total`.


## Use a functional style instead:

Piping does not work with you?  Simply use the functional interface:

```c++
    auto numbers = lz::limit(10, lz::read<int>(ifstream("test.txt")));
    auto evenFilter = lz::filter([](int i) { return i % 2 == 0; }, numbers);
    auto squares = lz::map([](int i) { return i *  i; }, evenFilter);
    int total = lz::sum(squares);
```

## Even more cute:

Those long lambdas, what can we do?  You could use a macro * don't panic* , macros are optional, I only offer a single one for convenience.

```c++
    int total = lz::read<int>(cin) | lz::limit(10) |
                lz::filter(lambda(i, i % 2 == 0)) | lz::map(lambda(i, i *  i)) |
                lz::sum();
```

The lambda macro (* if you want it* ) is there to build the standard lambda, one that captures the surrounding context and that can take both lvalue and references.  It simply uses all but the last argument as parameter names and the last argument as the return expression to evaluate.  `lambda(a,b,c,expression)` maps to `[&] (auto&& a, auto&& b, auto && c) { return expression; }`.

__It can be disabled___ by defining `#define LAZY_CODE_NO_MACROS` before including the `lazyCode` header.


## A case for safety:

Writing the equivalent of the above in plain old c++ is more cumbersome and can be argued to be less safe.  For example, this almost equivalent snippet has a bug.  Can you spot it?

```c++
    int total = 0;
    for (int i = 0; i < 10; i++) {
        int value;
        cin >> value;
        if (value % 2 == 0) {
            total += value *  value;
        }
    }
```

__The bug?__  What if the user enters less than 10 numbers.  You'll be reading `EOF` symbols into your `total`.  Silly example, but the idea is there.  This is not only more compact and readable, it can enable safer code.


# installation:

*  The project is built as a standard Cmake header only interface and can be included via cmake's external_project_add or as a sub directory.  Exact instructions for this are coming soon.
*  __For your convenience__, a single header is also maintained in the repo's top level directory `single_header`.  For contributers, notes on how to recreate the single header after a source change is described at the end of this readme.
*  Download the header file here: [lazyCode.h](single_header/lazyCode.h)
*  A __c++14__ compiler is required.


# The docs:

## The Basics:

There are two types of objects that can be created, `generators` and `collectors`.

A generator is simply a state object paired with a function that can be called to produce values based off the state.  This is discussed later in the section *creating your own generators*.  A generator by itself does not execute any instructions.  This is why we call it lazy evaluation; it merely describes how values may be generated.  Values may be pulled from a generator via the `next()` function;, by using a for loop `for (auto i: generator)`, or by passing it to a collector.  Collectors pull values from a generator and perform a given action, see the section below on collectors. 



## Basic generators:

### range:

```c++
template <typename Number, typename std::enable_if<
                               std::is_integral<Number>::value, int>::type = 0>
auto range(Number end);
```
*  Create a generator of sequence of integral values.  Sequence begins at 0 (inclusive) , is followed by values increasing by 1 and stops at the specified last point (exclusive).
*  `range(5)` generates `0,1,2,3,4,`
<!-- -->

```c++
template <typename Number, typename std::enable_if<
                               std::is_integral<Number>::value, int>::type = 0>
auto range(Number start, Number end); 
```
*  Create a generator of sequence of integral values.  Sequence begins at specified start point (inclusive) , is followed by values increasing by 1 and stops at the specified last point (exclusive).
*  `range(2,5)` generates `2,3,4`
<!-- -->


```c++
template <typename Number1, typename Number2>
auto range(Number1 start, Number1 end, Number2 increment); 
```
*  Create a generator of sequence of values.  Sequence begins at specified start point (inclusive) , is followed by values increasing/decreasing by the specified increment and stops at the specified last point (exclusive).
*  `range(0.1,1.0,0.2)` generates `0.1,0.3,0.5,0.7,0.9`
<!-- -->


### infRange


```c++
template <typename Number1, typename Number2>
auto infRange(Number1 start, Number2 increment)
```
* Create a never ending generator of sequence of values.  Sequence begins at specified start point (inclusive) and is followed by values increasing/decreasing by the specified increment.
*  `infRange(0,2)` infinite range, generates `0,2,4,6,8,...`
<!-- -->


### readLines

```c++
template <typename Stream>
auto readLines(Stream&& stream)
```
* return a generator that reads lines from the given stream.  The generator yields a new string for each line.  If an lvalue is given, only a reference to the stream is held.  If a rvalue is given, the generator takes ownership, the stream is moved into the generator.
*  `readLines(cout)` reads from std::cout, only a reference to cout is held
*  `readLines(istringstream(someString))` reads from the newly created string stream, the string stream is moved into the generator.
<!-- -->



### read

```c++
template <typename ReadType, typename Stream>
auto read(Stream&& stream)
```
* return a generator that reads from the given stream.  The generated type (the type of values pulled from the stream) must be specified as the first template parameter.  For example, to read integers from the stream, use `read<int>`.  If an lvalue is given, only a reference to the stream is held.  If a rvalue is given, the generator takes ownership, the stream is moved into the generator.
*  `read<double>(cin)` read double values from cin.
<!-- -->


### generator

```c++
template <typename Container>
decltype(auto) generator(Container&& container)
```
* Create a generator from a container.  The generator uses the containers begin and end iterators via `std::begin, std::end`.  If an rvalue is given, the generator will take ownership of the container and move it into the generator object, otherwise the generator will only hold a reference to the container.
*  `generator(v)` v can be a vector, map, set, anything with begin/end iterators. Only a reference to v is held.
*  `generator(V())` V can be a vector, map, set, anything with begin/end iterators. Since it is a newly created container, it is moved into the generator.
<!-- -->

### slice

```c++
template <typename Container>
decltype(auto) slice(Container&& container, size_t start, size_t last)
```
* return a generator that iterates through a container from position start (inclusive) to position end (exclusive).
  If an rvalue is given, the generator will take ownership of the container and
 move it into the generator object, otherwise the generator will only hold a
 reference to the container.
*  `slice(v,2,4)` Only a reference to v is held.
*  `slice(V(),2,4)` Since `V` is a newly created container, it is moved into the generator.
<!-- -->

```c++
template <typename Iter>
decltype(auto) slice(Iter first, Iter last)
```
* Create a generator from a pair of iterators first and last.  The generator container yields values from first (inclusive) to last (exclusive).
*  `string s; slice(s.begin(),s.end());`
<!-- -->



## Composed generators:

Composed generators are as the name suggests, building new generators from existing ones.  This can be done in the functional style  or using the pipe `|` style.  In both cases, when building a generator `g2` from `g1`, if `g1` is an rvalue (for example if it was constructed inline or moved), `g2` takes ownership, and `g1` is moved into `g2`.  If `g1` is an lvalue, (for example you previously created a generator and assign it to a variable `v`), `g2` will only hold a reference to `g1` meaning that care should be taken to make sure `v` (`g1`) is in scope as long as `g2`.


### Map

```c++
template <typename MapperFunc, typename Generator = GeneratorHole>
decltype(auto) map(MapperFunc&& mapperIn, Generator&& gen = GeneratorHole())
```
* Map one generator to another.  Produce a generator that returns the values produced by another generator applied to the given function.  The given function is invoked lazily to each value as requested. If the generator is not specified, a GeneratorBuilder is returned.  GeneratorBuilders are converted to generators by piping `|` a generator to them.
*  `map(func,generator)` or `generator | map(func)`
<!-- -->

### filter

```c++
template <typename FilterFunc, typename Generator = GeneratorHole>
decltype(auto) filter(FilterFunc&& filterIn,
                      Generator&& gen = GeneratorHole())
```
* Produce a generator that filters the output of another generator according to the given function.  The function should accept each value produced by the given generator and return true if that value is to be forwarded, false otherwise. The given function is invoked lazily to each value as requested. If the generator is not specified, a GeneratorBuilder is returned. GeneratorBuilders are converted to generators by piping `|` a generator to them.
*  `filter(func,generator)` or `generator | filter(func)`
<!-- -->



### enumerate

```c++
template <typename Generator = GeneratorHole>
decltype(auto) enumerate(size_t count = 0, Generator&& gen = GeneratorHole()) {
```
* Enumerate a generator.  Produce a generator that returns the values produced by another generator paired with an increasing count.  The default initial value of the count is 0.  Each yielded item will be pair(count,value).  If the generator is not specified, a GeneratorBuilder is returned. GeneratorBuilders are converted to generators by piping `|` a generator to them.
*  `enumerate(generator)` or `generator | enumerate()`
*  `enumerate(startingValue, generator)` or `generator | enumerate(startingValue)`
<!-- -->

### zip
```c++
template <typename Gen1, typename Gen2>
decltype(auto) zip(Gen1&& gen1, Gen2&& gen2)
```
* Combine two generators gen1 and gen2 into one.  Produce a generator that yields tuples where the first element of each tuple is a value pulled from gen1 and the second element is pulled from gen2.  The generator ends when either x or y end.
*  `zip(generator1,generator2)`
<!-- -->

### limit
```c++
template <typename Generator = GeneratorHole>
decltype(auto) limit(size_t n, Generator&& gen = GeneratorHole())
```
* Produce a generator that takes the first n values produced by another generator.If the generator is not specified, a GeneratorBuilder is returned. GeneratorBuilders are converted to generators by piping `|` a generator to them.
*  `limit(number, generator)` or `generator | limit(number)`
<!-- -->

## Collectors

Collectors evaluate generators.  This means pulling all the values from them.  Just as with composed generators, this can be done in the functional style  or using the pipe `|` style.  Note that you can pull values from a generator using its `begin/end` iterators, using a for loop `for (auto i: generator)` or using the `next(generator)` function.  

Lastly, note that if a generator yields references to values, for example a generator yielding values from a container, any collector or  for loop applied to the generator will receive these values as references, even if they are passed through another generator such as `filter`.  This allows code such as
```c++
vector<int> v = ...
auto g = lz::generator(v) | filter(...);
for (auto& i: g) {
    //change i and it will be changing values in v
}

```
However, if the generator yields rvalues, for example, the map generator, then the collector or for loop will see these as rvalues.  


### forEach

```c++
template <typename Func, typename Generator = GeneratorHole>
decltype(auto) forEach(Func&& func, Generator&& gen = GeneratorHole())
```
* Apply the given function to each value produced by the given generator.  The return value of the function is ignored and may be void.  If the generator is not specified, a collector is returned. Collectors remember the operation to be executed.  The operation is executed when a generator is piped `|` to it.
*  `generator | forEach(func)` or `forEach(func,generator)`
<!-- -->






### count


```c++
template <typename Generator = GeneratorHole>
decltype(auto) count(Generator&& gen = GeneratorHole())
```
* Count the number of values yielded by the given generator.  If the generator is not specified, a collector is returned. Collectors remember the operation to be executed.  The operation is executed when a generator is piped `|` to it.
*  `generator | count(), count(generator)`
<!-- -->

### sum
```c++
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) sum(Generator&& gen = GeneratorHole())
```
* Return the sum of the values produced by a generator.  If the generator is not specified, a collector is returned. Collectors remember the operation to be executed.  The operation is executed when a generator is piped `|` to it.
*  `generator | sum(), sum(generator)`
<!-- -->


### product

```c++
template <typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) product(Generator&& gen = GeneratorHole())
```
* Return the product (multiplication) of the values produced by a generator.If the generator is not specified, a collector is returned. Collectors remember the operation to be executed.  The operation is executed when a generator is piped `|` to it.
*  `generator | product(), product(generator)`
<!-- -->

### min

```c++
[1]
template <typename Generator = GeneratorHole>
decltype(auto) min(Generator&& gen = GeneratorHole())
```

```c++
[2]
template <typename Val, typename Generator = GeneratorHole>
decltype(auto) min(Val defaultVal, Generator&& gen = GeneratorHole())
```
* Return the minimum value produced by a generator.  If the generator yields no values, the default value is returned see [2].  If a default value is not given (see [1]), an optional is returned.  The optional holds a value in the case the generator yielded a value, otherwise the optional will be empty (nullopt). The optional object is a c++14 implementation of std::optional from c++17.  The implementation is taken from GitHub [akrzemi1/Optional](https://github.com/akrzemi1/Optional). If the generator is not specified, a collector is returned. Collectors remember the operation to be executed.  The operation is executed when a generator is piped `|` to it.
*  `auto m = generator | min(defaultVal) or `auto m =min(defaultVal, generator)`. `m` holds minimum value. `defaultVal`
*  `auto m = generator | min()` or `auto m = min(generator)`. Check if not empty with `if (m)`, access min value with `* m` 
<!-- -->

### max

```c++
[1]
template <typename Generator = GeneratorHole>
decltype(auto) max(Generator&& gen = GeneratorHole())
```

```c++
[2]
template <typename Val, typename Generator = GeneratorHole>
decltype(auto) max(Val defaultVal, Generator&& gen = GeneratorHole())
```

* Return the maximum value produced by a generator.  If the generator yields no values, the default value is returned see [2].  If a default value is not given (see [1]), an optional is returned.  The optional holds a value in the case the generator yielded a value, otherwise the optional will be empty (nullopt). The optional object is a c++14 implementation of std::optional from c++17.  The implementation is taken from GitHub [akrzemi1/Optional](https://github.com/akrzemi1/Optional). If the generator is not specified, a collector is returned. Collectors remember the operation to be executed.  The operation is executed when a generator is piped `|` to it.
*  `auto m = generator | max(defaultVal) or `auto m =max(defaultVal, generator)`. `m` holds maximum value. `defaultVal`
*  `auto m = generator | max()` or `auto m = max(generator)`. Check if not empty with `if (m)`, access max value with `* m` 
<!-- -->


### fold


```c++
template <typename Accum, typename Func, typename Generator = GeneratorHole,
          detail::EnableIfType<detail::GeneratorBase, Generator> = 0>
decltype(auto) fold(Func&& func, Accum accum,
                    Generator&& gen = GeneratorHole())
```
* Combine the values produced by a generator using the specified function and return the result.  The function should take two arguments and return a single value.  The accum parameter (accumulator) specifies the initial value. The return type of the function must be convertible to the accum type. If the generator is not specified, a collector is returned. Collectors remember the operation to be executed.  The operation is executed when a generator is piped `|` to it.
*  `generator | fold(func, accumulator )` or `fold(func,accumulator,generator)`
<!-- -->

### append

```c++
template <typename Container, typename Generator = GeneratorHole>
decltype(auto) append(Container&& container,
                      Generator&& gen = GeneratorHole())
```
* Append each value produced by the given generator to the given container, using container.emplace_back().  For unordered containers, see `insert`.  Note that a rvalue can be given as the container parameter, for example constructing a new container inline (`append(vector<int>())`), in which case the collector stores the container and returns it after appending the values. Otherwise, only a reference to the container is held by the collector.  If the generator is not specified, a collector is returned. Collectors remember the operation to be executed. The operation is executed when a generator is piped `|` to it.
*  `list<int> c; generator | append(c)` or `append(c,generator)`
*  `auto c = generator | append(vector<int>())` or `auto c = append(vector<int>(),generator)`
<!-- -->


### insert


```c++
template <typename Container, typename Generator = GeneratorHole>
decltype(auto) insert(Container&& container,
                      Generator&& gen = GeneratorHole())
```
* Insert each value produced by the given generator to the given container, using container.emplace().  This is for unordered containers.  For ordered containers, see `append`.  Note that a rvalue can be given as the container parameter, for example constructing a new container inline (`append(set<int>())`), in which case the collector stores the container and returns it after inserting the values. Otherwise, only a reference to the container is held by the collector. If the generator is not specified, a collector is returned. Collectors remember the operation to be executed. The operation is executed when a generator is piped `|` to it.
*  `set<int> c; generator | insert(c)` or `insert(c,generator)`
*  `auto c = generator | append(map<int,int>())` or `auto c = append(map<int,int>(),generator)`
<!-- -->


### write

```c++
[1]
template <typename Stream, typename Generator = GeneratorHole>
decltype(auto) write(Stream&& stream, Generator&& gen = GeneratorHole())
```

```c++
[2]
template <typename Stream, typename Interleave,
          typename Generator = GeneratorHole>
decltype(auto) write(Stream&& stream, Interleave i,
                     Generator&& gen = GeneratorHole())
``` 
* Write each value produced by the given generator to the given stream.  An interleave value can be optionally specified (see [2]), in which case the interleave value will be written to the stream before each generated value apart from the first.  This is sometimes known as join.  Note that a rvalue can be given as the stream parameter, for example constructing a new stream inline (`write(ostringstream())`), in which case the collector stores the stream and returns it after printing the values to it. Otherwise, only a reference to the stream is held by the collector.  If the generator is not specified, a collector is returned. Collectors remember the operation to be executed.  The operation is executed when a generator is piped `|` to it.
*  Write to existing stream s: `generator | write(s)` or `write(s,generator)`
*  Write to existing stream s interleaved with separator i: `generator | write(s,i)` or `write(s,i,generator)`
*  Create new stream and write to it: `auto s = generator | write(Stream())` or `auto s = write(Stream(),generator)`
*  Create new stream and write to it with separator i: `auto s = generator | write(Stream(),i)` or `auto s = write(Stream(),i,generator)`
*  Do a string join operation, that is, write to a string, using the above interface: `string s = (generator | write(std::ostringstream(),join)).str();`
<!-- -->



## Useful functions:
* `unpack(f)` Create a function `g` from `f`.  `g` accepts pairs/tuples and unpacks the tuple members and gives them as arguments to `f`.
        *  `map<int,int> m; generator(m) | forEach(unpack([] (auto&& key, auto&& value) { //something with key and value }))`
        *  `enumerate(generator) | forEach(unpack([] (auto&& index, auto&& value) { //something with index and value }))`
*  `std::string tostring(m1,m2,...,mn)` varadic function, prints `m1,m2,...,mn` using `ooperator<<` to a string.

## Creating your own generators:


* More details coming soon.  Here a toy example, a generator that generates powers of 2 up to 1024.
    ```c++
    auto g = lz::generator(1, [](int i) -> lz::optional<int> {
        i * = 2;
        if (i <= 1024) {
            return i;
        } else {
            return lz::nullopt;
        }
    });
    
    ```




# For contributers: rebuilding the single header file:

* The single header is built via a python3 script: [single_header/makeSingleHeader.py](single_header/makeSingleHeader.py).
* Usage: `./makeSingleHeader.py starting_file [include_path_1] [include_path_2] ... [include_path_n]`
* The script builds a string, copying the source from starting_file, replacing any includes with with the source contained at the file pointed to by the include.  This is done recursively for all included files.  
* The script (by choice) only identifies includes specified by quotes `include "..."` and not other others such as `include <...>`.  This can be trivially changed if necessary.
* Preprocess macros are not executed.  The only guard put in place is that a file is not included more than once.
* If a file `a` includes another file `b` via a path not relative to `a`, a path to the parent of `b` must be listed as an additional argument.  For example, currently the current source [include/lazyCode/lazyCode.h](include/lazyCode/lazyCode.h)  contains the include `#include "optional/optional.hpp"`.  However, this file is located in [include/optional/optional.hpp](include/optional/optional.hpp).  Therefore, the script is run as follows to specifie [include](include) as an additional include path: 
    ```single_header/makeSingleHeader.py include/lazyCode/lazyCode.h include > single_header/lazyCode.h```
