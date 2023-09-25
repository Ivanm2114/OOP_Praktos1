#include <iostream>
#include <utility>
#include <vector>
#include <assert.h>

using namespace std;

struct coords {
    int x;
    int y;
    int z;
};

/* 2-свободный коннектор
 * #-пустая клетка
 * 1-свободный сверху
 * 3-свободный снизу
 * 0-занятый с двух сторон
 */



class ConstructorBrick {

public:
    ConstructorBrick();

    ConstructorBrick(vector<vector<char>> structure);

    ConstructorBrick(ConstructorBrick &brick);

    bool setStructure(const vector<vector<char>> &NewStructure);

    bool ConnectWith(ConstructorBrick &NewPart, coords coors);

    int getLength() const;

    int getWidth() const;

    void printStructure() const;

    auto getStructure() const;

    auto getConnectedBricks() const;

    auto getConnectedBricksCoords() const;

private:
    bool IsConnectableSector(int i, int j, int k) const;

    void connectSector(int i, int j, int k);

    vector<vector<char>> structure;
    vector<ConstructorBrick *> connectedBricks;
    vector<coords> connectedBricksCoords;
    int length = 0;
    int width = 0;
};


auto ConstructorBrick::getStructure() const {
    return structure;
}


ConstructorBrick::ConstructorBrick() {
    length = 0;
    width = 0;
}

ConstructorBrick::ConstructorBrick(vector<vector<char>> structure) {
    setStructure(structure);
}


ConstructorBrick::ConstructorBrick(ConstructorBrick &brick) {
    setStructure(brick.getStructure());
    connectedBricksCoords = brick.connectedBricksCoords;
    for (auto connectedBrick: brick.connectedBricks) {
        connectedBricks.push_back(connectedBrick);
    }

}


bool ConstructorBrick::IsConnectableSector(const int i, const int j, const int k) const {
    if (structure[i][j] == '2' || structure[i][j] == '3' & k < 0 || structure[i][j] == '1' & k > 0) return true;
    return false;
}

void ConstructorBrick::connectSector(int i, int j, int k) {
    if (structure[i][j] == '2' & k > 0) structure[i][j] = '3';
    else if (structure[i][j] == '2' & k < 0) structure[i][j] = '1';
    else if (structure[i][j] == '3' || structure[i][j] == '1')
        structure[i][j] = '0';

}


bool ConstructorBrick::setStructure(const vector<vector<char>> &NewStructure) {
    bool flag = false;
    for (const auto &i: NewStructure) {
        for (int j = 0; j < i.size(); j++) {
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

void ConstructorBrick::printStructure() const {
    for (int i = 0; i < structure.size(); i++) {
        for (int j = 0; j < structure[i].size(); j++) {
            cout << structure[i][j];
        }
        cout << '\n';
    }
}

auto ConstructorBrick::getConnectedBricks() const {
    return connectedBricks;
}

auto ConstructorBrick::getConnectedBricksCoords() const {
    return connectedBricksCoords;
}


int ConstructorBrick::getLength() const {
    return length;
}

int ConstructorBrick::getWidth() const {
    return width;
}

bool ConstructorBrick::ConnectWith(ConstructorBrick &NewPart, coords coors) {
    coords oldBrickCoords{coors.x * -1, coors.y * -1, coors.z * -1};
    bool flag = false;
    cout << "Here\n";
    if ((coors.x < 0 && coors.x + NewPart.getLength() <= 0) || coors.x > length ||
        (coors.y < 0 && coors.y + NewPart.getWidth() <= 0) || coors.y > width ||
        abs(coors.z) != 1) {
        return false;
    }
    auto test = &NewPart;
    for (auto &connectedBrick: connectedBricks) {
        if (&NewPart == connectedBrick) return false;
    }
    int startRow = 0, startColumn = 0, newPartStartRow = 0, newPartStartColumn = 0;
    if (coors.x < 0) {
        startColumn = coors.x + NewPart.getLength() - 1;
        newPartStartColumn = NewPart.length + coors.x - 1;
    } else {
        startColumn = coors.x;
        newPartStartColumn = 0;
    }
    if (coors.y < 0) {
        startRow = coors.y + NewPart.getWidth() - 1;
        newPartStartRow = NewPart.width + coors.y - 1;
    } else {
        startRow = coors.y;
        newPartStartRow = 0;
    }
    cout << "Here 2\n";
    for (int i = startRow, ni = newPartStartRow; i < width && ni < NewPart.getWidth();
         i++, ni++) {
        for (int j = startColumn, nj = newPartStartColumn; j < length && nj < NewPart.getLength(); j++, nj++) {
            if (IsConnectableSector(i, j, coors.z) &&
                NewPart.IsConnectableSector(ni, nj, oldBrickCoords.z)) {
                connectSector(i, j, coors.z);
                NewPart.connectSector(ni, nj, oldBrickCoords.z);
                flag = true;

            }
        }
    }
    if (flag) {
        connectedBricks.push_back(&NewPart);
        connectedBricksCoords.push_back(coors);
        NewPart.connectedBricks.push_back(this);
        NewPart.connectedBricksCoords.push_back(oldBrickCoords);
    }
    return flag;
}


int main() {
    vector<vector<char>> structure1, structure2, empty_structure, structure_with_empty_vectors;
    for (int i = 0; i < 2; i++) {
        structure1.push_back(vector<char>());
        for (int j = 0; j < 3; j++) {
            structure1[i].push_back('2');
        }
    }
    ConstructorBrick brick1(structure1);
    brick1.printStructure();
    for (int i = 0; i < 1; i++) {
        structure2.push_back(vector<char>());
        structure_with_empty_vectors.push_back(vector<char>());
        for (int j = 0; j < 2; j++) {
            structure2[i].push_back('2');
        }
    }
    ConstructorBrick brick2(structure2);
    brick2.printStructure();
    auto brick1_blocks = brick1.getConnectedBricks();
    auto test = &brick2;
    ConstructorBrick brick;
    assert(brick.setStructure(empty_structure) == false);
    assert(brick.setStructure(structure_with_empty_vectors) == false);
    assert(brick1.setStructure(structure1) == true);
    coords brick2_coors{-2, 0, 1};
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.x = 3;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.x = 0;
    brick2_coors.y = -1;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.y = 2;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.y = 0;
    brick2_coors.z=0;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.z=-2;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.z=2;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.z=1;
    assert(brick1.ConnectWith(brick2, brick2_coors) == true);
    brick2_coors.y = 1;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.y = 0;
    brick.setStructure(structure2);
    assert(brick1.ConnectWith(brick, brick2_coors) == false);

    return 0;
}
