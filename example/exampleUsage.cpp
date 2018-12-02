
#include <lazyCode/lazyCode.h>
#include <fstream>
#include <iostream>
#include <vector>
namespace lz = LazyCode;
using namespace std;
void readmeExample1() {
    auto lines = lz::readLines(cin) | lz::append(vector<string>());
    sort(lines.begin(), lines.end());
    lz::generator(lines) | lz::write(cout, "\n");
}

void readmeExample2() {
    int total = lz::read<int>(ifstream("test.txt")) | lz::limit(10) |
                lz::filter([](int i) { return i % 2 == 0; }) |
                lz::map([](int i) { return i * i; }) | lz::sum();

    cout << total << endl;
}

void readmeExample3() {
    auto numbers = lz::read<int>(ifstream("test.txt")) | lz::limit(10);
    auto evenFilter = numbers | lz::filter([](int i) { return i % 2 == 0; });
    auto squares = evenFilter | lz::map([](int i) { return i * i; });
    int total = squares | lz::sum();
    cout << total << endl;
}

void readmeExample4() {
    auto numbers = lz::limit(10, lz::read<int>(ifstream("test.txt")));
    auto evenFilter = lz::filter([](int i) { return i % 2 == 0; }, numbers);
    auto squares = lz::map([](int i) { return i * i; }, evenFilter);
    int total = lz::sum(squares);
    cout << total << endl;
}

void readmeExample5() {
    int total = lz::read<int>(cin) | lz::limit(10) |
                lz::filter(lambda(i, i % 2 == 0)) | lz::map(lambda(i, i * i)) |
                lz::sum();
    cout << total << endl;
}
int main() {}