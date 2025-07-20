#include <algorithm>
#include <vector>
#include <iostream>

void lambda_example() {
    std::vector<int> nums = {5, 1, 4, 2};
    // [](int a, int b) { return a > b; } // 定義一個 lambda 函式，作為比較函式 一個沒有名字的函式物件（匿名函式）
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b;
    });

    for (int x : nums)
        std::cout << x << " ";  // 輸出：5 4 2 1
}
int main() {
    lambda_example();
    return 0;
}