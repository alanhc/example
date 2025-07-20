#include <iostream>
#include <future>

int compute_sum() {
    int sum = 0;
    for (int i = 1; i <= 100; ++i) sum += i;
    return sum;
}

int main() {
    // 使用 std::async 來非同步執行 compute_sum 函式
    std::future<int> result = std::async(std::launch::async, compute_sum);

    std::cout << "Doing something else...\n";

    // 等待結果
    std::cout << "Result: " << result.get() << "\n";
}
