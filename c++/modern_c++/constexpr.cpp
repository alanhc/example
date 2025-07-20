#include <iostream>
// constexpr 可以在編譯期計算值，並且可以用於常數表達式。
// 這樣可以提高程式的效率，因為不需要在執行期重新計算。
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int lookup[] = {
    factorial(0), factorial(1), factorial(2), factorial(3)
};

void use_constexpr() {
    std::cout << "3! = " << lookup[3] << "\n";  // 輸出：6
}
int main() {
    use_constexpr();
    return 0;
}
// Output: 3! = 6