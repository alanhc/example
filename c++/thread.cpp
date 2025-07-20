#include <thread>
#include <iostream>

void hello() { std::cout << "Hello from thread\n"; }

int main() {
    std::thread t(hello);
    t.join(); // 等待執行緒結束
}
