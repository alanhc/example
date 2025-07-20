#include <iostream>

class Animal
{
public:
    void eat() { std::cout << "Eating\n"; }
};

class Dog : public Animal
{
public:
    void bark() { std::cout << "Barking\n"; }
};

int main() {
    Dog dog;
    dog.eat();  // 繼承自 Animal
    dog.bark(); // Dog 自己的方法
    return 0;
}