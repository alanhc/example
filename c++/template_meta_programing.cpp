template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

// 使用：Factorial<5>::value → 120

#include <iostream>

int main() {
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << std::endl;
    return 0;
}
