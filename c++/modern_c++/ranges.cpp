#include <ranges>
#include <vector>
#include <iostream>

void use_ranges() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6};

    for (int x : nums 
            | std::views::filter([](int n){ return n % 2 == 0; }) // 過濾出偶數
            | std::views::transform([](int n){ return n * 10; })) // 將偶數乘以 10
        std::cout << x << " ";  // 輸出：20 40 60
}
int main() {
    use_ranges();
    return 0;
}