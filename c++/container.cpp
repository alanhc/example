#include <vector>
#include <map>
#include <unordered_set>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3};
    v.push_back(4);

    std::map<std::string, int> score;
    score["Alan"] = 95;

    std::unordered_set<int> uniq = {1, 2, 2, 3}; // 只會留下1,2,3

    for (auto x : v) std::cout << x << " ";        // 1 2 3 4
    std::cout << "\nAlan: " << score["Alan"] << "\n"; // Alan: 95
    std::cout << "Set size: " << uniq.size() << "\n"; // Set size: 3
}
