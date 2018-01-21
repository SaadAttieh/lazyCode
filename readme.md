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
    int total = read<int>(cin) | limit(10) | filter(_l1(i, i % 2 == 0)) |
                map(_l1(i, i * i)) | sum();
    cout << total << endl;
```

__Wow__, that's compact.  Maybe too compact? If you are concerned, you can split that line up into multiple expressions.  Take a look:

```c++
    auto numbers = read<int>(cin) | limit(10);
    auto evenNumbers = numbers | filter(_l1(i, i % 2 == 0));
    auto squares = evenNumbers | map(_l1(i, i * i));
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

##The docs:

Here we cover the API.
* Number ranges
* Piping
* Mapping
* Enumerate
* Filtering
* folding
* Reading
* Writing
