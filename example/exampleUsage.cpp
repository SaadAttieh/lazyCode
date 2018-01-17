#include <lazyCode/lazyCode.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace LazyCode;
int main() {
    auto a = range(20) | filter(_l1(i, i % 2 == 0)) | map(_l1(i, i * i)) |
             vector<int>();
    (a | print(cout, ",")) << endl;;
    std::cout << (a | count()) << endl;;
}
