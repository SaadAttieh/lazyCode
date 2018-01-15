#include <fstream>
#include <iostream>
#include <lazyCode/lazyCode.h>
#include <vector>
using namespace std;
using namespace LazyCode;
int main() {
    using namespace LazyCode;
    vector<int> a = {1, 2, 3, 4, 5};

    for (auto i : a | map([](int i) { return i * i; }) |
                      filter([](int i) { return i % 2 == 0; })) {
        cout << i << endl;
    }
}

