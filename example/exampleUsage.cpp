#include <lazyCode/lazyCode.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace LazyCode;
int main() { readLines(cin) | map(_l1(i, "$" + i)) | write(cout, "\n"); }
