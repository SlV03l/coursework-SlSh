#include "Computer.h"
using namespace std;

Computer::Computer() {
    i = j = prev_i = prev_j = -1;
    isHit = PrevHit = ChangeDir = FinishBoat = false;
    dir = Direction::up;
    CurrStrategy = Tactics::diagonals;
    PrevSunk = true;
    sunk = 0;
}

bool Computer::Strike(Map& UserMap, bool& isSunk) {
    ChangeDir = false;
    if (!FinishBoat && PrevHit)
        ChangeFireDirection();
    DetectCoords(UserMap);

    while (i < 0 || i > 9 || j < 0 || j > 9 || UserMap.getCell(i, j)->getBlocked()) {
        if (!FinishBoat)
            ChangeFireDirection();
        else {
            OppositeFireDirection(UserMap);
            break;
        }
        DetectCoords(UserMap);
    }

    isHit = UserMap.Strike(i, j, isSunk);

    if ((PrevHit && isHit && !PrevSunk) || (!PrevHit && isHit && !PrevSunk && ChangeDir))
        FinishBoat = true;

    UpdateTacticList(UserMap);

    if (isHit) {
        if (UserMap.getSunkNumber() > sunk) {
            sunk++;
            PrevSunk = true;
            FinishBoat = false;
        }
        else {
            prev_i = i, prev_j = j;
            PrevSunk = false;
        }
    }

    PrevHit = isHit;
    return isHit;
}

void Computer::DetectCoords(Map& UserMap) {
    if (!PrevSunk) {
        switch (dir) {
        case Direction::up:
            i = prev_i - 1;
            j = prev_j;
            break;
        case Direction::down:
            i = prev_i + 1;
            j = prev_j;
            break;
        case Direction::right:
            i = prev_i;
            j = prev_j + 1;
            break;
        case Direction::left:
            i = prev_i;
            j = prev_j - 1;
            break;
        }
    }
    else {
        ChangeStrategy(UserMap);
        ChooseStrategy(UserMap);
    }
}

void Computer::ChangeFireDirection() {
    switch (dir) {
    case Direction::up:
        dir = Direction::right;
        break;
    case Direction::right:
        dir = Direction::down;
        break;
    case Direction::down:
        dir = Direction::left;
        break;
    case Direction::left:
        dir = Direction::up;
        break;
    }
    ChangeDir = true;
}

void Computer::OppositeFireDirection(Map& UserMap) {
    if (dir == Direction::down) {
        do {
            --i;
        } while (UserMap.getCell(i, j)->getBlocked());
        dir = Direction::up;
    }
    else if (dir == Direction::up) {
        do {
            ++i;
        } while (UserMap.getCell(i, j)->getBlocked());
        dir = Direction::down;
    }
    else if (dir == Direction::left) {
        do {
            ++j;
        } while (UserMap.getCell(i, j)->getBlocked());
        dir = Direction::right;
    }
    else if (dir == Direction::right) {
        do {
            --j;
        } while (UserMap.getCell(i, j)->getBlocked());
        dir = Direction::left;
    }
}

void Computer::ChooseStrategy(Map& UserMap) {
    switch (CurrStrategy) {
    case Tactics::diagonals:
        i = rand() % 10;
        if (rand() % 2)
            j = i;
        else
            j = 9 - i;
        break;
    case Tactics::improved_locator:
        switch (rand() % 4) {
        case 0:
            if (rand() % 2)
                i = 3;
            else
                i = 6;
            if (rand() % 2)
                j = 0;
            else
                j = 9;
            break;
        case 1:
            if (rand() % 2)
                i = 0;
            else
                i = 9;
            if (rand() % 2)
                j = 3;
            else
                j = 6;
            break;
        case 2:
            if (rand() % 2)
                i = 4;
            else
                i = 5;
            if (rand() % 2)
                j = 1;
            else
                j = 8;
            break;
        case 3:
            if (rand() % 2)
                i = 1;
            else
                i = 8;
            if (rand() % 2)
                j = 4;
            else
                j = 5;
            break;
        }
        break;
    case Tactics::random:
        i = rand() % 10;
        j = rand() % 10;
        break;
    case Tactics::chesses:
        int even[5] = { 0, 2, 4, 6, 8 };
        int odd[5] = { 1, 3, 5, 7, 9 };
        i = rand() % 10;
        if (i % 2)
            j = even[rand() % 5];
        else
            j = odd[rand() % 5];
        break;
    }
}

bool Computer::ContainsCell(vector<pair<int, int>>& TacticList, pair<int, int>& coords) {
    for (pair<int, int> elem : TacticList)
        if (elem == coords)
            return true;
    return false;
}

void Computer::ChangeStrategy(Map& UserMap) {
    pair<int, int> ZeroPair(0, 0);
    if (CurrStrategy == Tactics::diagonals && improved_locator.size() != 16)
        CurrStrategy = Tactics::improved_locator;
    else if (CurrStrategy == Tactics::improved_locator && diagonals.size() != 20)
        CurrStrategy = Tactics::diagonals;
    else if (improved_locator.size() == 16 && diagonals.size() == 20 && !EndChess(UserMap))
        CurrStrategy = Tactics::chesses;
    else if (EndChess(UserMap))
        CurrStrategy = Tactics::random;
}

void Computer::UpdateTacticList(Map& UserMap) {
    pair<int, int> temp(i, j);
    if (!ContainsCell(diagonals, temp) && (j == i || j == 9 - i))
        diagonals.push_back(temp);
    else if (!ContainsCell(improved_locator, temp) && isLocatorCell())
        improved_locator.push_back(temp);
}

void Computer::UpdateTacticList(Map& UserMap, vector<pair<int, int>>& BorderList) {
    for (size_t k = 0; k < BorderList.size(); k++) {
        pair<int, int> coords = BorderList[k];
        if (!ContainsCell(diagonals, coords) && (coords.second == coords.first || coords.second == 9 - coords.first))
            diagonals.push_back(coords);
        else if (!ContainsCell(improved_locator, coords) && isLocatorCell(coords.first, coords.second))
            improved_locator.push_back(coords);
    }
}

bool Computer::EndChess(Map& UserMap) {
    for (int row = 0; row < 10; row++) {
        if (row % 2) {
            for (int col = 0; col < 10; col += 2)
                if (!UserMap.getCell(row, col)->getBlocked())
                    return false;
        }
        else {
            for (int col = 1; col < 10; col += 2)
                if (!UserMap.getCell(row, col)->getBlocked())
                    return false;
        }
    }
    return true;
}

bool Computer::isLocatorCell() {
    if ((i == 3 || i == 6) && (j == 0 || j == 9))
        return true;
    else if ((j == 3 || j == 6) && (i == 0 || i == 9))
        return true;

    else if ((i == 1 || i == 8) && (j == 4 || j == 5))
        return true;
    else if ((j == 1 || j == 8) && (i == 4 || i == 5))
        return true;
    return false;
}

bool Computer::isLocatorCell(int& row, int& col) {
    if ((row == 3 || row == 6) && (col == 0 || col == 9))
        return true;
    else if ((col == 3 || col == 6) && (row == 0 || row == 9))
        return true;
    else if ((row == 1 || row == 8) && (col == 4 || col == 5))
        return true;
    else if ((col == 1 || col == 8) && (row == 4 || row == 5))
        return true;
    return false;
}