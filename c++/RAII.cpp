#include <iostream>
#include <fstream>
#include <string>

void read_file(const std::string& filename) {
    std::ifstream file(filename);  // 開檔：建構時開啟檔案（RAII）

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";  // 逐行印出
    }

    // 離開作用域時自動關閉 file，無需手動呼叫 file.close()
}

int main() {
    std::string filename = "example.out";

    // Create the file and write some content to it
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << "Hello, World!\n";
        outfile << "This is a test file.\n";
        outfile.close();
    } else {
        std::cerr << "Failed to create file: " << filename << "\n";
        return 1;
    }

    read_file(filename);
    return 0;
}
