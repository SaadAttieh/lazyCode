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
    int total = read<int>(cin) | limit(1) | filter(_l1(i, i % 2 == 0)) |
                map(_l1(i, i * i)) | sum();
    cout << total << endl;
}

int main() { readmeExample(); }
