// g++ concept.cpp -std=c++20
#include <concepts>
#include <iostream>

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template<Addable T>
T add(T a, T b) {
    return a + b;
}

int main() {
    std::cout << add(1, 2) << "\n";       // ✅
    // add("a", "b"); // ❌ 編譯期報錯
}
