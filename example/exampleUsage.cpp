#include <lazyCode/lazyCode.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace LazyCode;
int main() {
string a = read<int>(cin) | writeString(",");
cout << a << endl;
}
