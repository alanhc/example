// g++ coroutine.cpp -std=c++20
#include <coroutine>
#include <iostream>

struct SimpleCoroutine {
    struct promise_type {
        SimpleCoroutine get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

SimpleCoroutine hello_coroutine() {
    std::cout << "Hello, ";
    co_await std::suspend_never{};
    std::cout << "Coroutine!\n";
}

int main() {
    hello_coroutine();  // 執行協程
}
