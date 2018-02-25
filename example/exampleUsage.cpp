#include <lazyCode/lazyCode.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace LazyCode;

/** Example from readme
* Read in at most 10 numbers from standard input.  For the numbers that are odd,
ignore them.  For the numbers that are even, square them.  Sum up the squares
and print the total.  Use minimal memory, i.e. do not store the numbers in a
vector, after all, it could easily be more than 10 numbers.
 */

void readmeExample() {
    auto lines = readLines(cin) | vector<string>();
    sort(lines.begin(), lines.end());
    lines | write(cout, "\n");
}

void readmeExample2() {
    // with macros
    int total = read<int>(cin) | limit(10) | filter(lambda(i, i % 2 == 0)) |
                map(lambda(i, i * i)) | sum();
    cout << total << endl;
    // without macros
    total = read<int>(cin) | limit(10) |
            filter([](int i) { return i % 2 == 0; }) |
            map([](int i) { return i * i; }) | sum();
    cout << total << endl;
}

void readmeExample3() {
    auto numbers = read<int>(cin) | limit(10);
    auto evenNumbers = numbers | filter(lambda(i, i % 2 == 0));
    auto squares = evenNumbers | map(lambda(i, i * i));
    int total = squares | sum();
    cout << total << endl;
}

inline void numberRangeExample() {
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
    // iterate backwards (negative ranges not supported yet

    // pipe into new vector
    auto vec1 = range(5) | vector<int>();
    // pipe into existing vector
    vector<int> vec2;
    range(5) | vec2;
}
int main() {
    //     readmeExample();
    //        readmeExample2();
    //    readmeExample3();
}
