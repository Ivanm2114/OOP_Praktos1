#include <iostream>
#include <vector>

using namespace std;

class ConstructorBrick {
public:
    void setStructure(vector<vector<char>> NewStructure);

private:
    vector<vector<char>> structure;
};

void ConstructorBrick::setStructure(vector<vector<char>> NewStructure) {
    structure = NewStructure;
}


int main() {
    ConstructorBrick brick{};
    vector<vector<char>> structure;
    for (int i = 0; i < 5; i++) {
        structure.push_back(vector<char>());
        for (int j = 0; j < 3; j++) {
            structure[i].push_back('5');
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << structure[i][j] << ' ';
        }
        std::cout << '\n';
    }
    brick.setStructure(structure);
    return 0;
}
