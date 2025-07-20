#include <iostream>
#include <memory>

struct Foo {
    Foo()  { std::cout << "Construct\n"; }
    ~Foo() { std::cout << "Destruct\n"; }
    void say() { std::cout << "Hello\n"; }
};

int main() {
    std::unique_ptr<Foo> ptr = std::make_unique<Foo>();
    ptr->say();
    return 0;
}