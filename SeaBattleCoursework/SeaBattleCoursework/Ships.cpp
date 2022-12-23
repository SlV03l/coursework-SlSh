#include "Ship.h"
#include <SFML/Graphics.hpp>

Ship::Ship() {
    size = col = row = -1;
}

Ship::Ship(int size) {
    this->size = size;
    col = row = -1;
}

void Ship::setDirection(int num) {
    if (num == 0)
        Direction = "Horizontal";
    else
        Direction = "Vertical";
}

string Ship::getDirection() {
    return Direction;
}

void Ship::setCoords() {

    switch (size)
    {
    case 1:
        if (Direction == "Horizontal")
        {
            col = rand() % 10;
            row = rand() % 10;
        }
        else
        {
            col = rand() % 10;
            row = rand() % 10;
        }
        break;
    case 2:
        if (Direction == "Horizontal")
        {
            col = rand() % 9;
            row = rand() % 10;
        }
        else
        {
            col = rand() % 10;
            row = rand() % 9;
        }
        break;
    case 3:
        if (Direction == "Horizontal")
        {
            col = rand() % 8;
            row = rand() % 10;
        }
        else
        {
            col = rand() % 10;
            row = rand() % 8;
        }
        break;
    case 4:
        if (Direction == "Horizontal")
        {
            col = rand() % 7;
            row = rand() % 10;
        }
        else
        {
            col = rand() % 10;
            row = rand() % 7;
        }
    }
}

int Ship::getRow() {
    return row;
}

int Ship::getCol() {
    return col;
}

int Ship::getSize() {
    return size;
}

bool Ship::isSunk(vector<vector<Cell*>>& cells) {
    if (Direction == "Horizontal") {
        for (int j = col; j < col + size; j++)
            if (!cells[row][j]->getHit())
                return false;
    }
    else
    {
        for (int i = row; i < row + size; i++)
            if (!cells[i][col]->getHit())
                return false;
    }
    return true;
}

bool Ship::operator==(const Ship& other) {
    if (this->Direction == other.Direction && this->row == other.row && this->col == other.col && this->size == other.size)
        return true;
    else
        return false;
}