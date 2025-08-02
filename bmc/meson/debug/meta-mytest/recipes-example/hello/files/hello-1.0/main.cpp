// main.cpp
#include <iostream>

int main() {
	std::cout << "Hello, World!" << std::endl;
	int* ptr = nullptr;
	std::cout << *ptr << std::endl; // Dereferencing a null pointer causes a segmentation fault (core dump)
	return 0;
}
