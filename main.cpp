#include <iostream>
#include <utility>
#include <vector>
#include <assert.h>
#include <string.h>

#define BOTH_FREE '2'
#define ONLY_TOP_FREE '1'
#define ONLY_BOTTOM_FREE '3'
#define BOTH_OCCUPIED '0'
#define zero_coords coords{0,0,0}

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

    ConstructorBrick(const vector<vector<char>> &structure);

    ConstructorBrick(const ConstructorBrick &brick);

    bool canSetStructure(const vector<vector<char>> &NewStructure) const;

    void setStructure(const vector<vector<char>> &NewStructure);

    bool canConnectWith(const ConstructorBrick &NewPart, coords coors) const;

    void connectWith(ConstructorBrick &NewPart, coords coors);

    int getLength() const;

    int getWidth() const;

    void printStructure() const;

    auto getStructure() const;

    auto getConnectedBricks() const;

    auto getConnectedBricksCoords() const;

    bool isConnectableSector(int x, int y, int z) const;


    virtual string getClass() const;

protected:

    void connectSector(int x, int y, int z);

    vector<vector<char>> structure;
    vector<ConstructorBrick *> connectedBricks;
    vector<coords> connectedBricksCoords;
    int length = 0;
    int width = 0;
};


class FunctionalBrick : public ConstructorBrick {
public :
    FunctionalBrick();

    FunctionalBrick(const vector<vector<char>> &structure, string action);

    FunctionalBrick(const FunctionalBrick &brick);

    void setAction(const string &newAction);

    void doAction() const;

    bool canSetAction(const string &newAction) const;

    string getAction() const;

    string getClass() const override;

private:
    string action = "";
};


class Assembly {
public:
    Assembly();

    Assembly(const ConstructorBrick &newPart);

    Assembly(const Assembly &baseAssembly);

    bool canAdd(const ConstructorBrick &newPart, coords coors) const;

    void add(ConstructorBrick &newPart, coords coors);

    auto getAllLayers() const;

    auto getLayer(int layer_num) const;

    auto getCoords() const;

    bool partInAssembly(ConstructorBrick &NewPart) const;

    string getClass() const;

private:
    void recursive_add(ConstructorBrick &newPart, coords coors);

    vector<ConstructorBrick *> structure;
    vector<coords> brickCoords;

};


ConstructorBrick::ConstructorBrick() {
    length = 0;
    width = 0;
}


ConstructorBrick::ConstructorBrick(const vector<vector<char>> &structure) {
    setStructure(structure);
}

FunctionalBrick::FunctionalBrick() {
    length = 0;
    width = 0;
    action = "";
}

FunctionalBrick::FunctionalBrick(const vector<vector<char>> &structure, string action) {
    setStructure(structure);
    setAction(action);
}

Assembly::Assembly() = default;


string Assembly::getClass() const {
    return "Assembly";
}

string FunctionalBrick::getAction() const {
    return action;
}


auto ConstructorBrick::getStructure() const {
    return structure;
}

ConstructorBrick::ConstructorBrick(const ConstructorBrick &brick) {
    structure = vector<vector<char>>();
    setStructure(brick.getStructure());
    connectedBricksCoords = brick.connectedBricksCoords;
    for (auto connectedBrick: brick.connectedBricks) {
        connectedBricks.push_back(connectedBrick);
    }

}

FunctionalBrick::FunctionalBrick(const FunctionalBrick &brick) {
    setStructure(brick.getStructure());
    setAction(brick.getAction());


}


string ConstructorBrick::getClass() const {
    return "ContructorBrick";
}


string FunctionalBrick::getClass() const {
    return "FunctionalBrick";

}

bool ConstructorBrick::isConnectableSector(const int x, const int y, const int z) const {
    if (structure[x][y] == BOTH_FREE || structure[x][y] == ONLY_BOTTOM_FREE & z < 0 ||
        structure[x][y] == ONLY_TOP_FREE & z > 0)
        return true;
    return false;
}

void ConstructorBrick::connectSector(int x, int y, int z) {
    if (structure[x][y] == BOTH_FREE & z > 0) structure[x][y] = ONLY_BOTTOM_FREE;
    else if (structure[x][y] == BOTH_FREE & z < 0) structure[x][y] = ONLY_TOP_FREE;
    else if (structure[x][y] == ONLY_BOTTOM_FREE || structure[x][y] == ONLY_TOP_FREE)
        structure[x][y] = BOTH_OCCUPIED;

}

bool ConstructorBrick::canSetStructure(const vector<vector<char>> &newStructure) const {
    bool flag = false;
    for (const auto &i: newStructure) {
        for (int j = 0; j < i.size(); j++) {
            flag = true;
        }
    }
    return flag;
}


void ConstructorBrick::setStructure(const vector<vector<char>> &newStructure) {
    if (canSetStructure(newStructure)) {
        structure = newStructure;
        int maxLength = 0;
        for (int i = 0; i < structure.size(); i++) {
            if (maxLength < structure[i].size()) length = structure[i].size();
        }
        width = structure.size();
    } else {
        cout << "This Structure can`t be setted\n";
    }
}

void ConstructorBrick::printStructure() const {
    cout << getClass() << endl;
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

bool ConstructorBrick::canConnectWith(const ConstructorBrick &newPart, coords coors) const {
    coords thisBrickCoords{coors.x * -1, coors.y * -1, coors.z * -1};
    bool flag = false;
    if ((coors.x < 0 && coors.x + newPart.getLength() <= 0) || coors.x > length ||
        (coors.y < 0 && coors.y + newPart.getWidth() <= 0) || coors.y > width ||
        abs(coors.z) != 1) {
        return false;
    }
    for (int i = 0; i < connectedBricks.size(); i++) {
        if ((((connectedBricksCoords[i].x <= coors.x and
               coors.x <= connectedBricksCoords[i].x + connectedBricks[i]->length - 1) and
              (connectedBricksCoords[i].y <= coors.y and
               coors.y <= connectedBricksCoords[i].y + connectedBricks[i]->width - 1)) or
             ((coors.x <= connectedBricksCoords[i].x and
               connectedBricksCoords[i].x <= coors.x + newPart.length - 1) and
              (coors.y <= connectedBricksCoords[i].y and
               connectedBricksCoords[i].y <= coors.y + newPart.width - 1))) and
            connectedBricksCoords[i].z == coors.z)
            return false;
    }
    if (&newPart == this) return false;
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
            if (isConnectableSector(i, j, coors.z) &&
                newPart.isConnectableSector(ni, nj, thisBrickCoords.z)) {
                flag = true;

            }
        }
    }
    return flag;

}

void ConstructorBrick::connectWith(ConstructorBrick &newPart, coords coors) {
    coords thisBrickCoords{coors.x * -1, coors.y * -1, coors.z * -1};
    if (canConnectWith(newPart, coors)) {
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
                if (isConnectableSector(i, j, coors.z) &&
                    newPart.isConnectableSector(ni, nj, thisBrickCoords.z)) {
                    connectSector(i, j, coors.z);
                    newPart.connectSector(ni, nj, thisBrickCoords.z);

                }
            }
        }
        connectedBricks.push_back(&newPart);
        connectedBricksCoords.push_back(coors);
        newPart.connectedBricks.push_back(this);
        newPart.connectedBricksCoords.push_back(thisBrickCoords);
    } else {
        cout << "These Bricks cant be connected\n";
    }

}


bool FunctionalBrick::canSetAction(const string &newAction) const {
    return !newAction.empty();
}

void FunctionalBrick::setAction(const string &newAction) {
    if (canSetAction(newAction)) {
        this->action = newAction;
    } else {
        cout << "This action can`t be setted\n";
    }
}

void FunctionalBrick::doAction() const {
    if (!action.empty()) {
        cout << action << "\n";
    } else {
        cout << "Block doesn`t have an action\n";
    }
}


bool Assembly::canAdd(const ConstructorBrick &newPart, coords coors = zero_coords) const {
    if (structure.empty()) return structure.empty();
    else {
        bool flag = false;
        for (int i = 0; i < structure.size(); i++) {
            coords rel_coords = coors;
            rel_coords.x -= brickCoords[i].x;
            rel_coords.y -= brickCoords[i].y;
            rel_coords.z -= brickCoords[i].z;
            if (structure[i]->canConnectWith(newPart, rel_coords)) {
                flag = true;

            }
        }
        return flag;
    }
}

void Assembly::add(ConstructorBrick &newPart, coords coors = zero_coords) {
    if (structure.empty()) {
        recursive_add(newPart, coors);
    } else if (canAdd(newPart, coors)) {
        brickCoords.push_back(coors);
        structure.push_back(&newPart);

        for (int i = 0; i < structure.size(); i++) {
            coords rel_coords = coors;
            rel_coords.x -= brickCoords[i].x;
            rel_coords.y -= brickCoords[i].y;
            rel_coords.z -= brickCoords[i].z;
            if (structure[i]->canConnectWith(newPart, rel_coords)) {
                structure[i]->connectWith(newPart, rel_coords);
            }
        }
    } else {
        cout << "This brick can`t be added\n";
    }
}

bool Assembly::partInAssembly(ConstructorBrick &newPart) const {
    bool flag = false;
    for (int i = 0; i < structure.size(); i++) {
        if (structure[i] == &newPart) return true;
    }
    return false;
}


void Assembly::recursive_add(ConstructorBrick &newPart, coords coors) {
    structure.push_back(&newPart);
    brickCoords.push_back(coors);
    for (int i = 0; i < newPart.getConnectedBricks().size(); i++) {
        if (!partInAssembly(*newPart.getConnectedBricks()[i])) {
            coords new_coors = coors;
            new_coors.x += newPart.getConnectedBricksCoords()[i].x;
            new_coors.y += newPart.getConnectedBricksCoords()[i].y;
            new_coors.z += newPart.getConnectedBricksCoords()[i].z;
            recursive_add(*(newPart.getConnectedBricks()[i]), new_coors);
        }
    }
}

auto Assembly::getAllLayers() const {
    return structure;

}

auto Assembly::getCoords() const {
    return brickCoords;
}

auto Assembly::getLayer(int layer) const {
    vector<ConstructorBrick *> layer_vector;
    for (int i = 0; i < structure.size(); i++) {
        if (brickCoords[i].z == layer) layer_vector.push_back(structure[i]);
    }
    return layer_vector;

}


Assembly::Assembly(
        const ConstructorBrick &newPart) {
    recursive_add(const_cast<ConstructorBrick &>(newPart), zero_coords);
}

Assembly::Assembly(
        const Assembly &baseAssembly) {
    structure = baseAssembly.getAllLayers();
    brickCoords = baseAssembly.getCoords();
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
    ConstructorBrick brick;
    FunctionalBrick funcbrick;
    assert(brick.canSetStructure(empty_structure) == false);
    assert(brick.canSetStructure(structure_with_empty_vectors) == false);
    assert(brick1.canSetStructure(structure1) == true);
    brick1.setStructure(structure1);
    coords brick2_coors{-2, 0, 1};
    assert(brick1.canConnectWith(brick2, brick2_coors) == false);
    brick2_coors.x = 3;
    assert(brick1.canConnectWith(brick2, brick2_coors) == false);
    brick2_coors.x = 0;
    brick2_coors.y = -1;
    assert(brick1.canConnectWith(brick2, brick2_coors) == false);
    brick2_coors.y = 2;
    assert(brick1.canConnectWith(brick2, brick2_coors) == false);
    brick2_coors.y = 0;
    brick2_coors.z = 0;
    assert(brick1.canConnectWith(brick2, brick2_coors) == false);
    brick2_coors.z = -2;
    assert(brick1.canConnectWith(brick2, brick2_coors) == false);
    brick2_coors.z = 2;
    assert(brick1.canConnectWith(brick2, brick2_coors) == false);
    brick1.connectWith(brick2, brick2_coors);
    brick2_coors.z = 1;
    assert(brick1.canConnectWith(brick1, brick2_coors) == false);
    assert(brick1.canConnectWith(brick2, brick2_coors) == true);
    cout << "----------------------------------------\n";
    brick1.connectWith(brick2, brick2_coors);
    brick2_coors.y = 1;
    assert(brick1.canConnectWith(brick2, brick2_coors) == false);
    brick2_coors.y = 0;
    brick.setStructure(structure2);
    cout << "----------------------------------------\n";
    assert(funcbrick.canSetStructure(empty_structure) == false);
    assert(funcbrick.canSetStructure(structure_with_empty_vectors) == false);
    assert(funcbrick.canSetStructure(structure1) == true);
    assert(funcbrick.canSetAction("") == false);
    assert(funcbrick.canSetAction("Beep! Beep!") == true);
    funcbrick.setAction("");
    funcbrick.setAction("Beep! Beep!");
    funcbrick.doAction();
    funcbrick.setStructure(empty_structure);
    brick2_coors.z = -1;
    funcbrick.setStructure(structure1);
    brick2_coors.x = 0;
    brick2_coors.y = 0;
    brick2_coors.z = 1;
    assert(brick1.canConnectWith(funcbrick, brick2_coors) == false);
    brick2_coors.z = -1;
    assert(funcbrick.canConnectWith(brick2, brick2_coors) == true);
    funcbrick.connectWith(brick2, brick2_coors);
    assert(funcbrick.canConnectWith(brick2, brick2_coors) == false);
    funcbrick.connectWith(brick2, brick2_coors);
    FunctionalBrick funcbrick2(funcbrick);
    funcbrick2.doAction();
    FunctionalBrick funcbrick3(structure1, "NewAction");
    funcbrick3.doAction();
    assert(brick.getClass() == "ContructorBrick");
    assert(funcbrick.getClass() == "FunctionalBrick");
    ConstructorBrick copied_brick(brick1);
    cout << "----------------------------------------\n";
    Assembly assembly;
    brick1.printStructure();
    funcbrick.printStructure();
    assert(assembly.getAllLayers().empty() == true);
    assert(assembly.canAdd(funcbrick) == true);
    assembly.add(funcbrick);
    assert(assembly.getAllLayers().size() == 3);
    assert(assembly.getLayer(-1).size() == 1);
    assert(assembly.canAdd(funcbrick3, coords{1, 1, -1}) == true);
    assembly.add(funcbrick3, coords{1, 1, -1});
    assert(assembly.getAllLayers().size() == 4);
    assert(assembly.getLayer(-1).size() == 2);
    Assembly copied_assembly(assembly);
    assert(copied_assembly.getAllLayers().size() == 4);
    assert(copied_assembly.getCoords().size() == 4);
    Assembly assembly2(funcbrick);
    assert(assembly2.getAllLayers().size() == 4);
    assert(assembly.getClass() == "Assembly");
    funcbrick.printStructure();
    brick1.printStructure();
    funcbrick.doAction();



    cout << "All test have been passed\n";
    return 0;
}
