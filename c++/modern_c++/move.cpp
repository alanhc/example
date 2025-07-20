#include <vector>
#include <string>
#include <iostream>

void move_demo() {
    // 建立一個大型字串
    std::string large = "This is a large string..."; 
    std::vector<std::string> vec;

    // 複製大型字串到 vector
    // 複製
    vec.push_back(large);

    // 複製後，large 仍然保持原有內容
    // 搬移（不複製內容）
    vec.push_back(std::move(large));
    // 把 large 這個字串的資源搬進 vec 向量中
    // std::move(large) 轉換成右值參考 → 可以「偷走」它的內部資料（指標）。
    // vec.push_back(...) 呼叫 move constructor，把 large 的資料搬進 vector。
    // large 自己會變成「空的狀態」（但仍是有效的物件）。

    std::cout << "After move: " << large << "\n";  // Undefined, usually empty
    std::cout << "Vector size: " << vec.size() << "\n"; // 應該是 2
    // 印出 vector 中的所有字串
    for (const auto& str : vec) {
        std::cout << str << "\n";
    }
}

int main() {
    move_demo();
    return 0;
}
