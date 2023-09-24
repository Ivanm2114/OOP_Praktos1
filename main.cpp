#include <iostream>
#include <utility>
#include <vector>

using namespace std;

struct coords {
    int x;
    int y;
    int z;
};


class ConstructorBrick {

public:
    ConstructorBrick();

    ConstructorBrick(vector<vector<char>> structure);
//    ConstructorBrick(ConstructorBrick &brick);
//    ~ConstructorBrick();

    bool setStructure(vector<vector<char>> NewStructure);

    bool ConnectWith(ConstructorBrick NewPart, coords coors);

    bool IsConnectableSector(int i, int j);

    int getLength();

    int getWidth();

    void printStructure();

    auto getStructure();

private:
    vector<vector<char>> structure;
//    vector<ConstructorBrick> connectedBricks;
    vector<coords> connectedBricksCoords;
    int length = 0;
    int width = 0;
};


auto ConstructorBrick::getStructure() {
    return structure;
}


ConstructorBrick::ConstructorBrick() {
    length = 0;
    width = 0;
}

ConstructorBrick::ConstructorBrick(vector<vector<char>> structure) {
    setStructure(structure);
}


//ConstructorBrick::ConstructorBrick(ConstructorBrick &brick) {
//    setStructure(brick.getStructure());
//    connectedBricksCoords = brick.connectedBricksCoords;
//    for(int i=0;i<brick.connectedBricks.size();i++){
//        connectedBricks.push_back(brick.connectedBricks[i]);
//    }
//
//}
//
//

bool ConstructorBrick::IsConnectableSector(int i, int j) {
    return structure[i][j] == '0';
}


bool ConstructorBrick::setStructure(vector<vector<char>> NewStructure) {
    bool flag = false;
    for (int i = 0; i < NewStructure.size(); i++) {
        for (int j = 0; j < NewStructure[i].size(); j++) {
            flag = true;
        }
    }
    if (flag) {
        structure = NewStructure;
        int maxLength = 0;
        for (int i = 0; i < structure.size(); i++) {
            if (maxLength < structure[i].size()) length = structure[i].size();
        }
        width = structure.size();
    }
    return flag;
}

void ConstructorBrick::printStructure() {
    for (int i = 0; i < structure.size(); i++) {
        for (int j = 0; j < structure[i].size(); j++) {
            cout << structure[i][j];
        }
        cout << '\n';
    }
}


int ConstructorBrick::getLength() {
    return length;
}

int ConstructorBrick::getWidth() {
    return width;
}

bool ConstructorBrick::ConnectWith(ConstructorBrick NewPart, coords coors) {
    cout << "Here\n";
    if ((coors.x < 0 && coors.x + NewPart.getLength() <= 0) || coors.x > length ||
        (coors.y < 0 && coors.y + NewPart.getWidth() <= 0) || coors.y > width) {
        return false;
    }
    int startRow = 0, startColumn = 0, newPartStartRow = 0, newPartStartColumn = 0;
    if (coors.x < 0) {
        startRow = coors.x + NewPart.getLength() - 1;
        newPartStartRow = NewPart.length + coors.x - 1;
    } else {
        startRow = coors.x;
        newPartStartRow = 0;
    }
    if (coors.y < 0) {
        startColumn = coors.y + NewPart.getWidth() - 1;
        newPartStartColumn = NewPart.width + coors.y - 1;
    }
    else {
        startRow = coors.y;
        newPartStartColumn = 0;
    }
    cout << "Here 2\n";
    for (int i = startRow; i < width && i < coors.y + NewPart.getWidth(); i++) {
        for (int j = startColumn; j < length && j < coors.x + NewPart.getLength(); j++) {
            if (IsConnectableSector(i, j) &&
                NewPart.IsConnectableSector(newPartStartRow + i, newPartStartColumn + j))
                return true;
        }
    }
    return false;
}


int main() {
    vector<vector<char>> structure;
    for (int i = 0; i < 2; i++) {
        structure.push_back(vector<char>());
        for (int j = 0; j < 3; j++) {
            structure[i].push_back('0');
        }
    }
    ConstructorBrick brick1(structure);
    structure.clear();
    brick1.printStructure();
    for (int i = 0; i < 1; i++) {
        structure.push_back(vector<char>());
        for (int j = 0; j < 2; j++) {
            structure[i].push_back('0');
        }
    }
    ConstructorBrick brick2(structure);
    coords brick2_coors{-1, 0, 0};
    brick2.printStructure();
    bool rez = brick1.ConnectWith(brick2, brick2_coors);
    cout << rez;
    return 0;
}
