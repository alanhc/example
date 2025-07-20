#include <vector>
#include <string>
#include <iostream>
void use_vector() {
    std::vector<std::string> names;

    // 1. 建立並直接搬入
    // 直接建構在記憶體內部，避免額外複製或搬移。
    names.emplace_back("Alan");

    // 2. 使用 std::move 搬移現有的字串
    // 這樣可以避免不必要的複製，直接將資源從
    // 一個物件轉移到另一個物件。
    std::string s = "Bob";
    names.push_back(std::move(s));  // 移入
    
    // Print the vector contents
    for (const auto& name : names) {
        std::cout << name << std::endl;
    }
}
int main() {
    use_vector();
    return 0;
}