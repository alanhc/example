#include <iostream>
#include <string>
#include <utility>

class Person {
public:
    Person(std::string name) {
        std::cout << "Constructing " << name << "\n";
    }
};

template<typename T>
Person create(T&& name) {
    return Person(std::forward<T>(name)); // 完美保留 lvalue/rvalue 特性
}

int main() {
    std::string n = "Alan";
    create(n);             // lvalue 傳入
    create("Temporary");   // rvalue 傳入
}
