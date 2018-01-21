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
    int total = read<int>(cin) | limit(10) | filter(_l1(i, i % 2 == 0)) |
                map(_l1(i, i * i)) | sum();
    cout << total << endl;
    // without macros
    total = read<int>(cin) | limit(10) |
            filter([](int i) { return i % 2 == 0; }) |
            map([](int i) { return i * i; }) | sum();
    cout << total << endl;
}

void readmeExample3() {
    auto numbers = read<int>(cin) | limit(10);
    auto evenNumbers = numbers | filter(_l1(i, i % 2 == 0));
    auto squares = evenNumbers | map(_l1(i, i * i));
    int total = squares | sum();
    cout << total << endl;
}
int main() {
    // readmeExample();
    //    readmeExample2();
    //    readmeExample3();
    for (auto x : enumerate(read<int>(cin))) {
        cout << x.first << "," << x.second<< endl;
    }
}
