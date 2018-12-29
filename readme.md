# Lazy Code
# Cute and Efficient Is Definitely Possible 

* Compossible, lazily evaluated generators such as map, filter, fold, enumerated ranges and more.
* Easy handling of input and output streams, reading line by line, parsing objects, etc.
* Compact syntax:  Choice between
    * Functional `fold(...,map(...,filter(...)))`
    * Piped `filter(...) | map(...) | fold(...)`
* Zero cost abstractions leveraging C++ templates, no macros involved.
* Easily create new generators and integrate with existing ones.
* _Optionally_ enabled  macros to make the syntax even more cute.


# Quick examples:

This is just some demonstrations of the coding style enabled with this library.  A full listing comes after the examples.


```c++
namespace lz = LazyCode;
using namespace std;
```

## Example 1:

* Read in lines of text into a vector until EOF is reached, one line per vector element.
* Sort the vector and then reprint the lines.

```c++
    auto lines = lz::readLines(cin) | lz::append(vector<string>());
    sort(lines.begin(), lines.end());
    lz::generator(lines) | lz::write(cout, "\n");
```

Yup, that's it.  And it gets better...

## Example 2:

* Read in up to 10 integers from a file "test.txt".  
* filter for the even numbers, square them and sum their values.
* Do not store the numbers in a container, after all, it could easily be more than 10 numbers.

```c++
    int total = lz::read<int>(ifstream("test.txt")) | lz::limit(10) |
                lz::filter([](int i) { return i % 2 == 0; }) |
                lz::map([](int i) { return i * i; }) | lz::sum();
```

__Wow__, that's compact.  Maybe too compact? If you are concerned, you can split that line up into multiple expressions.  Take a look:

```c++
    auto numbers = lz::read<int>(ifstream("test.txt")) | lz::limit(10);
    auto evenFilter = numbers | lz::filter([](int i) { return i % 2 == 0; });
    auto squares = evenFilter | lz::map([](int i) { return i * i; });
    int total = squares | lz::sum();
```

* Even though this expression is split over multiple variable assignments, it is not any less efficient.
* Each intermediate variable simply 
describes a unit of code to be executed.  All held in stack.  Nothing is executed until the final pipe into the `sum()`.
* The final value (the sum of the squares of only even numbers in a file) is calculated in a single pass with no intermediate container or memory allocations required.
* for each number in `test.txt`, evaluate `filter condition`, and add to `total`.


## Use a functional style instead:

Piping does not work with you?  Simply use the functional interface:

```c++
    auto numbers = lz::limit(10, lz::read<int>(ifstream("test.txt")));
    auto evenFilter = lz::filter([](int i) { return i % 2 == 0; }, numbers);
    auto squares = lz::map([](int i) { return i * i; }, evenFilter);
    int total = lz::sum(squares);
```

## Even more cute:

Those long lambdas, what can we do?  You could use a macro *don't panic*, macros are optional, I only offer a single one for convenience.

```c++
    int total = lz::read<int>(cin) | lz::limit(10) |
                lz::filter(lambda(i, i % 2 == 0)) | lz::map(lambda(i, i * i)) |
                lz::sum();
```

The lambda macro (*if you want it*) is there to build the standard lambda, one that captures the surrounding context and that can take both lvalue and references.  It simply uses all but the last argument as parameter names and the last argument as the return expression to evaluate.  `lambda(a,b,c,expression)` maps to `[&] (auto&& a, auto&& b, auto && c) { return expression; }`.

__It can be disabled___ by defining `#define LAZY_CODE_NO_MACROS` before including the `lazyCode` header.


## A case for safety:

Writing the equivalent of the above in plain old c++ is more cumbersome and can be argued to be less safe.  For example, this almost equivalent snippet has a bug.  Can you spot it?

```c++
    int total = 0;
    for (int i = 0; i < 10; i++) {
        int value;
        cin >> value;
        if (value % 2 == 0) {
            total += value * value;
        }
    }
```

__The bug?__  What if the user enters less than 10 numbers.  You'll be reading `EOF` symbols into your `total`.  Silly example, but the idea is there.  This is not only more compact and readable, it can enable safer code.


# installation:

* The project is built as a standard Cmake header only interface and can be included via cmake's external_project_add or as a sub directory.  Exact instructions for this are coming soon.
* __For your convenience__, a single header is also maintained in the repo's top level directory `single_header`.
* Download the header file here: [lazyCode.h](single_header/lazyCode.h)
* A __c++14__ compiler is required.


# The docs:

Here we cover the API.  First a quick index of the available functions (Full documentation follows):

## Quick Index:

1. Input:
    * `range(5)` generates `0,1,2,3,4,`
    * `range(2,5)` generates `2,3,4`
    * `range(0.1,1.0,0.2)` generates `0.1,0.3,0.5,0.7,0.9`
    * `infRange(0,2)` infinite range, generates `0,2,4,6,8,...`
    * `readLines(stream)` generates strings, one per line read from `stream`.
    * `read<T>(stream)` read objects of type `T` from `stream`.
    * `generator(container)` generate values from any iterable container, using the container's begin/end iterators. 
    * `slice(container,startIndex,endIndex)` generate values from any iterable container, starting at position `std::begin(container) + startIndex` (inclusive) and ending at `std::begin(container) + endIndex` (exclusive).
    * `slice(startIter,endIter)` generate values from a pair of iterators, yielding values from `startIter` (inclusive) to `endIter` (exclusive).
2. Processing:
    * Lazy map: `map(func,generator)` or `generator | map(func)`
    * Lazy filter: `filter(func,generator)` or `generator | filter(func)`
    * `map(toUpperCase, filter([] (auto&& l) { return l.size() > 0; }, readLines(cin) ))`
    * Enumerate: `enumerate(generator)` or `generator | enumerate()`
    * zip: `zip(generator1,generator2)`
4. Output: 
    * forEach: `generator | forEach(func)` or `forEach(func,generator)`
    * for loop: `for (auto i: generator)`,
    * Folding:
        * Count number of yields: `generator | count(), count(generator)`
        * `generator | sum(), sum(generator)`
        * `generator | product(), product(generator)`
        * `generator | min(), min(generator)`, returns optional which is empty (`nullopt`) if generator yields no values.
        * `generator | min(defaultVal), min(defaultVal, generator)`, returns `defaultVal` if generator yields no values.
        * `generator | max(), max(generator)`, returns optional which is empty (`nullopt`) if generator yields no values.
        * `generator | max(defaultVal), max(defaultVal, generator)`, returns `defaultVal` if generator yields no values.
        * `generator | fold(func, accumulator )` or `fold(func,accumulator,generator)`
    * Storing in containers:
        * Append to end of existing container c: `generator | append(c)` or `append(c,generator)`
        * Create new container C and Append to it: `auto c = generator | append(C())` or `auto c = append(C(),generator)`
        * Insert into existing unordered container c: `generator | insert(c)` or `insert(c,generator)`
        * Create new unordered container C and insert into it: `auto c = generator | insert(C())` or `auto c = insert(C(),generator)`
    * Writing to streams:
        * Write to existing stream s: `generator | write(s)` or `write(s,generator)`
        * Write to existing stream s interleaved with separator i: `generator | write(s,i)` or `write(s,i,generator)`
        * Create new stream and write to it: `auto s = generator | write(Stream())` or `auto s = write(Stream(),generator)`
        * Create new stream and write to it with separator i: `auto s = generator | write(Stream(),i)` or `auto s = write(Stream(),i,generator)`
        * Write to string using the above interface: `string s = (generator | write(std::ostringstream(),i)).str();`
4.  Useful functions:
    * `unpack(f)` Create a function `g` from `f`.  `g` accepts pairs/tuples and unpacks the tuple members and gives them as arguments to `f`.
        * `generator(map) | forEach(unpack([] (auto&& key, auto&& value) { //something with key and value }))`
        * `enumerate(generator) | forEach(unpack([] (auto&& index, auto&& value) { //something with index and value }))`
    * `std::string tostring(m1,m2,...,mn)` varadic function, prints `m1,m2,...,mn` using `ooperator<<` to a string.
6. Creating your own generators:
    * More details coming soon.  Here a toy example, a generator that generates powers of 2 up to 1024.
    ```c++
    auto g = lz::generator(1, [](int i) -> lz::optional<int> {
        i *= 2;
        if (i <= 1024) {
            return i;
        } else {
            return lz::nullopt;
        }
    });
    ```


## Full Docs:

Markdown rendered docs coming soon.  The docs can also be read directly from the source:

* [basicGenerators.h](include/lazyCode/basicGenerators.h), all the types of generators that can be made.
* [basicCollectors.h](include/lazyCode/basicCollectors.h), all the functions for evaluating generators.