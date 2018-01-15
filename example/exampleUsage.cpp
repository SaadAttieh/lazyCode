#include <lazyCode/lazyCode.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace LazyCode;
int main() {
    int i;
    range(20) | mapQ(i, i * 2) | filterQ(i, i % 3 == 0) |
        mapQ(i, (cout << i << " ", 0)) | eval;
}
