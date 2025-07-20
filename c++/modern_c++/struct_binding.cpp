// c++ % g++ struct_binding.cpp -std=c++17
#include <tuple>
#include <iostream>

std::tuple<int, double> get_stats() { // 返回一個 tuple，包含計數和平均值
    return {42, 3.14};
}

void destructuring() {
    auto [count, average] = get_stats(); // 使用結構化綁定，直接解構 tuple
    std::cout << "Count: " << count << ", Avg: " << average << "\n";
}

int main() {
    destructuring();
    return 0;
}
// Output: Count: 42, Avg: 3.14