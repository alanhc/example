#include <iostream>
#include <type_traits>

template<typename T>
auto add(T a, T b) -> typename std::enable_if<std::is_arithmetic<T>::value, T>::type {
    return a + b;
}

int main() {
    std::cout << add(1, 2) << "\n";       // ✅ OK
    // add("a", "b");                    // ❌ 編譯失敗，但不報錯，SFINAE 生效
}
