#include <lazyCode/lazyCode.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace LazyCode;
int main() {
    vector<int> a;
    range(20) | a;
    a | map([](auto i) { return i * 2; }) |
        filter([](auto i) { return i % 3 == 0; }) |
        map([](auto i) { std::cout << i << " "; }) | eval;
}
