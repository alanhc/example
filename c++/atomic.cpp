#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> counter{0};

void increment() {
    for (int i = 0; i < 10000; ++i)
        ++counter;
}

int main() {
    std::thread t1(increment), t2(increment);
    t1.join(); t2.join();
    std::cout << "Counter: " << counter << std::endl; // 應該是20000
}
