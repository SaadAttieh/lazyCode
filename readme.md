# Lazy Code
# Cute and Efficient Is Definitely Possible 

* Bringing the missing elements to C++.
* Compossible, lazily evaluated algorithms for processing data such as map, filter, fold, enumerated ranges and more.
* Easy handling of input and output streams, parsing integers, reading line by line, etc.
* Compact syntax showing clear flow of data.
* Zero cost abstractions leveraging C++ templates.
* Optionally enabled  macros to make the syntax even more cute.


## Example 1:

Read in lines of text into a vector, one line per element, until EOF is reached.  Sort the vector and then re print the lines.

```c++
    auto lines = readLines(cin) | vector<string>();
    sort(lines.begin(), lines.end());
    lines | write(cout, "\n");
    lines | write(cout, "\n");
```

Yup, that's it.  And it gets better...

## Example 2:

Read in up to 10 numbers from standard input.  For the numbers that are odd, ignore them.  For the numbers that are even, square them.  Sum up the squares and print the total.  Use minimal memory, i.e. do not store the numbers in a vector, after all, it could easily be more than 10 numbers.

```c++
    //with macros enabled, see below for non macro version.
    int total = read<int>(cin) | limit(10) | filter(lambda(i, i % 2 == 0)) |
                map(lambda(i, i * i)) | sum();
    cout << total << endl;
```

__Wow__, that's compact.  Maybe too compact? If you are concerned, you can split that line up into multiple expressions.  Take a look:

```c++
    auto numbers = read<int>(cin) | limit(10);
    auto evenNumbers = numbers | filter(lambda(i, i % 2 == 0));
    auto squares = evenNumbers | map(lambda(i, i * i));
    int total = squares | sum();
    cout << total << endl;
```

  Even though it looks like multiple assignments/pass throughs are made, none of the expressions will be evaluated until the final pipe into the `sum()`.  The numbers are not stored in any intermediate container.  It is just as efficient as a loop with conditionals.  Yet such a loop would be more cumbersome an is more likely to contain errors.  For example, this has a bug.  Can you spot it?

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

__The bug?__  What if the user enters less than 10 numbers.  You'll be reading EOF symbols into your total.


## Who's concerned about the macros!

 Hopefully most of you are thinking, even with these made up examples, "that __is__ cool, what else can I do?"  

However, some may be concerned that there is clearly some macro action going on here. Perhaps you don't want to use macros in your project, "Only type safe templates for me..."  __DON'T WORRY__, the macros are only there to provide a shorter lambda syntax.  The macros can be easily disabled and it still looks good.  Take a look:

```c++
int total = read<int>(cin) | limit(10) |
            filter([](int i) { return i % 2 == 0; }) |
            map([](int i) { return i * i; }) | sum();
```


Now that we've sorted that out, from now on, examples are shown with the lambda macros enabled.

# The docs:

Here we cover the API.

* Ranges, evaluators and piping:
* Number ranges
* Enumerate
* Mapping
* Filtering
* folding
* Reading from streams
* Writing  to streams


## Behind the scenes; Ranges, evaluators and piping:

This gives some behind the scenes information. if you just want to get writing quickly, skip to the next section.

Under all those cute expressions you'll be writing with *lazyCode* are two categories of objects; ranges and evaluators.  Ranges are objects that represent some kind of iterable.  They are used to generate a stream of values.  Most of the objects like map, filter, fold, zip, etc return ranges.  Ranges can be composed together to produce new ranges, usually with the pipe `|` symbol.  For example, given a filter range `a` and a map range `b`, you can produce a range that filters then maps (in one go) with `a | b`.  The same notation works even if `a`` is an STL  container (e.g. std::vector).  The container is wrapped in a range designed for iterating over containers.

By default, ranges are lazily evaluated.  This means that unless explicitly invoked, ranges simply sit where they have been created, incurring no runtime costs.  There are three main ways to pull values out of ranges:

1.  Use a range based for loop, `for (auto i: rangeObj) { ... }`.  Since ranges support `begin()` and `end()` they can be used anywhere input iterators may be used.
1.  Pipe the range into an evaluator (see below).
1.  Use the range interface (least recommended and not guarantied to be stable).

Evaluators turn ranges into results.  When piping a range into an evaluator, LazyCode pulls all the values from the range and pushes them to the evaluator.  Evaluators may then return a result based on the pushed values.  For example, `count()`  returns the number of values pushed, `write()` writes the pushed values to a stream, etc.  Evaluators are used by composing them with ranges in the same notation `range | evaluator`.

Unless stated otherwise, whenever an object  `a`  is composed with another `b`, if `a` is a variable (lvalue/lvalue reference), `b` will only hold a reference to `a`.  However, if `a` is a temporary object (rvalue), which usually happens when you create a range inline, then `b` will own `a`.  That is, if `a` is a temporary, `a` will be moved into `b` such that `a` remains constructed until `b` is destructed.

## Jumping write in;  let's begin with Number ranges.

The most simple but often useful range, iterating over numbers.


```c++
    // iterate 0..5 (5 exclusive)
    for (int x : range(5)) {
    }
    // iterate 10..20
    for (int x : range(10, 20)) {
    }
    // iterate 0..10 in steps of 2
    for (int x : range(0, 10, 2)) {
    }
    // iterate 1.0 to 2.0 in increments of 0.1
    for (double x : range(1.0, 2.0, 0.1)) {
    }
    //iterate backwards (negative ranges not supported yet
```

## Piping:

All the above can be piped directly into containers.

```c++
    // pipe into new vector
    auto vec1 = range(5) | vector<int>();
    // pipe into existing vector
    vector<int> vec2;
    range(5) | vec2;
```


tbc 