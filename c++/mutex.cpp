#include <mutex>
#include <thread>
#include <iostream>

int value = 0;
std::mutex mtx;

void add() {
    for (int i = 0; i < 10000; ++i) {
        std::lock_guard<std::mutex> lock(mtx); // 自動lock/unlock // 這一行只有拿到 lock 的 thread 能執行
        ++value;
    }
}

int main() {
    std::thread t1(add), t2(add);
    t1.join(); t2.join();
    std::cout << "Value: " << value << std::endl; // 應該是20000
}
