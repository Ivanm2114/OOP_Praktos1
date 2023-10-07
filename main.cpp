#include <iostream>
#include <utility>
#include <vector>
#include <assert.h>

#define BOTH_FREE '2'
#define ONLY_TOP_FREE '1'
#define ONLY_BOTTOM_FREE '3'
#define BOTH_OCCUPIED '0'


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
    bool IsConnectableSector(int x, int y, int z) const;

    void connectSector(int x, int y, int z);

    vector<vector<char>> structure;
    vector<ConstructorBrick *> connectedBricks;
    vector<coords> connectedBricksCoords;
    int length = 0;
    int width = 0;
};

ConstructorBrick::ConstructorBrick() {
    length = 0;
    width = 0;
}


ConstructorBrick::ConstructorBrick(vector<vector<char>> structure) {
    setStructure(structure);
}


auto ConstructorBrick::getStructure() const {
    return structure;
}

ConstructorBrick::ConstructorBrick(ConstructorBrick &brick) {
    setStructure(brick.getStructure());
    connectedBricksCoords = brick.connectedBricksCoords;
    for (auto connectedBrick: brick.connectedBricks) {
        connectedBricks.push_back(connectedBrick);
    }

}


bool ConstructorBrick::IsConnectableSector(const int x, const int y, const int z) const {
    if (structure[x][y] == BOTH_FREE || structure[x][y] == ONLY_BOTTOM_FREE & z < 0 || structure[x][y] == ONLY_TOP_FREE & z > 0) return true;
    return false;
}

void ConstructorBrick::connectSector(int x, int y, int z) {
    if (structure[x][y] == BOTH_FREE & z > 0) structure[x][y] = ONLY_BOTTOM_FREE;
    else if (structure[x][y] == BOTH_FREE & z < 0) structure[x][y] = ONLY_TOP_FREE;
    else if (structure[x][y] == ONLY_BOTTOM_FREE || structure[x][y] == ONLY_TOP_FREE)
        structure[x][y] = BOTH_OCCUPIED;

}


bool ConstructorBrick::setStructure(const vector<vector<char>> &newStructure) {
    bool flag = false;
    for (const auto &i: newStructure) {
        for (int j = 0; j < i.size(); j++) {
            flag = true;
        }
    }
    if (flag) {
        structure = newStructure;
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

bool ConstructorBrick::ConnectWith(ConstructorBrick &newPart, coords coors) {
    coords oldBrickCoords{coors.x * -1, coors.y * -1, coors.z * -1};
    bool flag = false;
    if ((coors.x < 0 && coors.x + newPart.getLength() <= 0) || coors.x > length ||
        (coors.y < 0 && coors.y + newPart.getWidth() <= 0) || coors.y > width ||
        abs(coors.z) != 1) {
        return false;
    }
    if(&newPart == this) return false;
    auto test = &newPart;
    for (auto &connectedBrick: connectedBricks) {
        if (&newPart == connectedBrick) return false;
    }
    int startRow = 0, startColumn = 0, newPartStartRow = 0, newPartStartColumn = 0;
    if (coors.x < 0) {
        startColumn = coors.x + newPart.getLength() - 1;
        newPartStartColumn = newPart.length + coors.x - 1;
    } else {
        startColumn = coors.x;
        newPartStartColumn = 0;
    }
    if (coors.y < 0) {
        startRow = coors.y + newPart.getWidth() - 1;
        newPartStartRow = newPart.width + coors.y - 1;
    } else {
        startRow = coors.y;
        newPartStartRow = 0;
    }
    for (int i = startRow, ni = newPartStartRow; i < width && ni < newPart.getWidth();
         i++, ni++) {
        for (int j = startColumn, nj = newPartStartColumn; j < length && nj < newPart.getLength(); j++, nj++) {
            if (IsConnectableSector(i, j, coors.z) &&
                    newPart.IsConnectableSector(ni, nj, oldBrickCoords.z)) {
                connectSector(i, j, coors.z);
                newPart.connectSector(ni, nj, oldBrickCoords.z);
                flag = true;

            }
        }
    }
    if (flag) {
        connectedBricks.push_back(&newPart);
        connectedBricksCoords.push_back(coors);
        newPart.connectedBricks.push_back(this);
        newPart.connectedBricksCoords.push_back(oldBrickCoords);
    }
    return flag;
}


int main() {
    vector<vector<char>> structure1, structure2, empty_structure, structure_with_empty_vectors;
    for (int i = 0; i < 2; i++) {
        structure1.push_back(vector<char>());
        for (int j = 0; j < 3; j++) {
            structure1[i].push_back(BOTH_FREE);
        }
    }
    ConstructorBrick brick1(structure1);
    brick1.printStructure();
    for (int i = 0; i < 1; i++) {
        structure2.push_back(vector<char>());
        structure_with_empty_vectors.push_back(vector<char>());
        for (int j = 0; j < 2; j++) {
            structure2[i].push_back(BOTH_FREE);
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
    brick2_coors.z = 0;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.z = -2;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.z = 2;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.z = 1;
    assert(brick1.ConnectWith(brick1,brick2_coors) == false);
    assert(brick1.ConnectWith(brick2, brick2_coors) == true);
    brick2_coors.y = 1;
    assert(brick1.ConnectWith(brick2, brick2_coors) == false);
    brick2_coors.y = 0;
    brick.setStructure(structure2);
    assert(brick1.ConnectWith(brick, brick2_coors) == false);
    ConstructorBrick copied_brick(brick1);
    copied_brick.printStructure();
    cout << "All test have been passed\n";
    return 0;
}
