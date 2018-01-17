#include <lazyCode/lazyCode.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace LazyCode;
int main() {
    int a = range(2,5,2) | filter(_l1(i, i % 2 == 0)) | map(_l1(i, i * i)) | product();
    cout << a << endl;
}
