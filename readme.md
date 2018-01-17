# Lazy Code
# Cute and Efficient Is Definitely Possible 

* Bringing the missing elements to C++.
* Compossible, lazily evaluated algorithms for processing data such as map, filter and fold.
* Easy handling of input and output streams, parsing integers, reading line by line, regex matching.
* Compact syntax showing clear flow of data.
* Zero cost abstractions leveraging C++ templates.
* Optionally enabled  macros to make the syntax even more cute.

## Example
Read in lines of text into a vector, one line per element.  Keep going until EOF is reached.  Sort the vector and then re print the lines.

```c++
    vector<string> lines;
    readLines(cin) | lines;
    sort(lines.begin(), lines.end());
    lines | write(cout, "\n");
```

Yup, that's it.  And it gets better...

Read in up to 10 numbers from standard input.  For the numbers that are odd, ignore them.  For the numbers that are even, square them.  Sum up the squares and print the total.  Use minimal memory, i.e. do not store the numbers in a vector, after all, it could easily be more than 10 numbers.

```c++
    int total = read<int>(cin) | limit(10) | filter(_l1(i, i % 2 == 0)) |
                map(_l1(i, i * i)) | sum();
    cout << total << endl;
```

__Wow__, that's compact.  Maybe too compact? If you are concerned, you can split that line up by assigning the intermediary objects to variables.  Since it is lazily evaluated, nothing will be read/written until the final sum() is applied.

```c++
    auto evenFilter = read<int>(cin) | limit(10) | filter(_l1(i, i % 2 == 0));
    auto squares = evenFilter | map(_l1(i, i * i));
    // trigger execution here
    int total = squares | sum();
```

Writing this in standard C++ is more cumbersome but more importantly, increases the chance for error.  For example this has a bug:
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

The bug?  What if the user enters less than 10 numbers.  You'll be reading EOF into your total.


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

Here we cover the API.  Map, fold, filter, sum, product, fold, reading and writing files, counting and more. (TBC)