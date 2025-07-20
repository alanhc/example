#include <algorithm>
#include <iostream>
#include <vector>

// template 可以讓我們寫出通用的函式或類別，能夠處理不同型別的資料。
template<typename T>
void print_sorted(std::vector<T> vec) {
    std::sort(vec.begin(), vec.end());
    for (const auto& v : vec)
        std::cout << v << " ";
    std::cout << "\n";
}
int main() {
    std::vector<int> numbers = {5, 3, 8, 1, 2};
    std::cout << "Original vector: ";
    for (const auto& n : numbers)
        std::cout << n << " ";
    std::cout << "\n";

    std::cout << "Sorted vector: ";
    print_sorted(numbers);
    print_sorted<std::string>({"cat", "apple", "banana"});

    return 0;
}