#include <iostream>

class Animal {
public:
    virtual void speak() { std::cout << "Animal sound\n"; }
};

class Dog : public Animal {
public:
    void speak() override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
    void speak() override { std::cout << "Meow!\n"; }
};

void makeSound(Animal* a) {
    a->speak(); // 動態綁定：根據實際型別呼叫對應的方法
}

int main() {
    Dog dog;
    Cat cat;
    makeSound(&dog); // ➜ Woof!
    makeSound(&cat); // ➜ Meow!
    return 0;
}
    