#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using std::ostream;
using std::istringstream;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

struct PersonInfo {
    string name;
    std::vector<string> phones;
};

int main() {
    string line, word;
    std::vector<PersonInfo> people;
    istringstream iss;
    while (std::getline(std::cin, line)) {
        PersonInfo info;
        iss.clear();    // 上一次读到eof，先重置状态。
        iss.str(line);
        iss >> info.name;
        while (iss >> word)
            info.phones.push_back(word);
        people.push_back(info);
    }

    for (const auto &s : people) {
        cout << s.name << " ";
        for (const auto &p : s.phones)
            cout << p << " ";
        cout << endl;
    }
    return 0;
}
