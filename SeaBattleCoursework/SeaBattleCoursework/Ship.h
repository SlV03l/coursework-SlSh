#pragma once
#include "Cell.h"

class Ship{
private:
	string Direction;
	int row, col, size; // ������, �������, ������
public:
	Ship();
	Ship(int size);
	void setDirection(int num);
	string getDirection();
	void setCoords(); // ����������
	int getRow();
	int getCol();
	int getSize();
	bool isSunk(vector<vector<Cell*>>& cells);
	bool operator==(const Ship& other);
};

