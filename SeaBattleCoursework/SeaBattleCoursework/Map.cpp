#include "Map.h"
using namespace sf;

Map::Map() {
	Over = Border = false;
	ShowBoats = true;
	Hits = 0;
}

Map::Map(int i_start, int j_start, bool ShowBoats) {
	cells.clear();
	for (int i = 0, i_shift = i_start; i < 10; i++, i_shift += 32) {
		vector <Cell*> Row;
		for (int j = 0, j_shift = j_start; j < 10; j++, j_shift += 32) {
			Row.push_back(new Cell((float)j_shift, (float)i_shift));
			Row[j]->SetUpSprite("Images\\Cell.png");
		}
		cells.push_back(Row);
	}
	this->ShowBoats = ShowBoats;
	setBoats();
	Over = false;
	Hits = 0;
}

Cell* Map::getCell(int i, int j) {
	return cells[i][j];
}

bool Map::isOver() {
	return Over;
}

int Map::getSunkNumber() {
	return SunkList.size();
}

void Map::setBoatList() {
	Ship Carrier(4); BoatList.push_back(Carrier);
	Ship Battleship1(3); BoatList.push_back(Battleship1);
	Ship Battleship2(3); BoatList.push_back(Battleship2);
	Ship Destoyer1(2); BoatList.push_back(Destoyer1);
	Ship Destoyer2(2); BoatList.push_back(Destoyer2);
	Ship Destoyer3(2); BoatList.push_back(Destoyer3);
	Ship Submarine1(1); BoatList.push_back(Submarine1);
	Ship Submarine2(1); BoatList.push_back(Submarine2);
	Ship Submarine3(1); BoatList.push_back(Submarine3);
	Ship Submarine4(1); BoatList.push_back(Submarine4);
}

void Map::setBoats() {
	setBoatList();
	for (size_t count = 0; count < BoatList.size(); count++) {
		BoatList[count].setDirection(rand() % 2);
		string direction = BoatList[count].getDirection();
		BoatList[count].setCoords();
		while (FreeSpace(BoatList[count]) == false) {
			BoatList[count].setDirection(rand() % 2);
			string direction = BoatList[count].getDirection();
			BoatList[count].setCoords();
		}
		if (BoatList[count].getDirection() == "Horizontal")
			for (int col = BoatList[count].getCol(); col < BoatList[count].getCol() + BoatList[count].getSize(); col++) {
				cells[BoatList[count].getRow()][col]->setBoat();
				if (ShowBoats)
					cells[BoatList[count].getRow()][col]->SetUpSprite("Images\\Ship.png");
			}
		else
			for (int row = BoatList[count].getRow(); row < BoatList[count].getRow() + BoatList[count].getSize(); row++) {
				cells[row][BoatList[count].getCol()]->setBoat();
				if (ShowBoats)
					cells[row][BoatList[count].getCol()]->SetUpSprite("Images\\Ship.png");
			}
	}
}

bool Map::FreeSpace(Ship& ship) {
	if (ship.getDirection() == "Horizontal"){

		for (int i = ship.getRow() - 1; i < ship.getRow() + 2; i++)
			for (int j = ship.getCol() - 1; j < ship.getCol() + ship.getSize() + 1; j++)
				if (i < 0 || i >= 10 || j < 0 || j >= 10)
					continue;
				else if (cells[i][j]->getBoat() || cells[i][j]->getBlocked() || cells[i][j]->getBorder())
					return false;
	}
	else{
		for (int i = ship.getRow() - 1; i < ship.getRow() + ship.getSize() + 1; i++)
			for (int j = ship.getCol() - 1; j < ship.getCol() + 2; j++)
				if (i < 0 || i >= 10 || j < 0 || j >= 10)
					continue;
				else if (cells[i][j]->getBoat() || cells[i][j]->getBlocked() || cells[i][j]->getBorder())
					return false;
	}
	return true;
}


bool Map::setClick(RenderWindow& window, Event& GameEvent, RenderWindow& win, bool& isSunk) {
	bool repeat = true, isHit = false;
	while (repeat) {
		while (window.pollEvent(GameEvent)) {
			if (GameEvent.type == Event::Closed) {
				window.close();
				win.close();
			}
		}
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (!cells[i][j]->getBlocked()) {
					if (Mouse::isButtonPressed(Mouse::Left) && cells[i][j]->getSprite().getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
						isHit = Strike(i, j, isSunk);
						repeat = false;
						break;
					}
				}
			}
		}
	}

	return isHit;
}

void Map::setBorder(Ship& ship) {
	if (ship.getDirection() == "Horizontal")
	{
		for (int i = ship.getRow() - 1; i < ship.getRow() + 2; i++)
			for (int j = ship.getCol() - 1; j < ship.getCol() + ship.getSize() + 1; j++)
				if (i < 0 || i >= 10 || j < 0 || j >= 10)
					continue;
				else
					cells[i][j]->setBorder();
	}
	else
	{
		for (int i = ship.getRow() - 1; i < ship.getRow() + ship.getSize() + 1; i++)
			for (int j = ship.getCol() - 1; j < ship.getCol() + 2; j++)
				if (i < 0 || i >= 10 || j < 0 || j >= 10)
					continue;
				else
					cells[i][j]->setBorder();
	}
}

void Map::setBorderCoords(Ship& ship) {
	if (ship.getDirection() == "Horizontal")
	{
		for (int row = ship.getRow() - 1; row < ship.getRow() + 2; row++) {
			for (int col = ship.getCol() - 1; col < ship.getCol() + ship.getSize() + 1; col++) {
				if (row < 0 || row >= 10 || col < 0 || col >= 10)
					continue;
				else {
					pair <int, int> temp(row, col);
					BorderCoords.push_back(temp);
				}
			}
		}
	}
	else
	{
		for (int row = ship.getRow() - 1; row < ship.getRow() + ship.getSize() + 1; row++) {
			for (int col = ship.getCol() - 1; col < ship.getCol() + 2; col++) {
				if (row < 0 || row >= 10 || col < 0 || col >= 10)
					continue;
				else {
					pair <int, int> temp(row, col);
					BorderCoords.push_back(temp);
				}
			}
		}
	}
}

vector<pair<int, int>> Map::getBorderCoords() {
	return BorderCoords;
}

bool Map::Strike(int i, int j, bool& isSunk) {

	int flag;
	Border = false;

	if (cells[i][j]->getBoat()) {
		cells[i][j]->setHit();
		Hits++;
		flag = true;
	}
	else {
		cells[i][j]->setMiss();
		flag = false;
	}

	isSunk = false;
	for (size_t i = 0; i < BoatList.size(); i++) {
		if (BoatList[i].isSunk(cells) && !(Contains(BoatList[i]))) {
			setBorder(BoatList[i]);
			BorderCoords.clear();
			setBorderCoords(BoatList[i]);
			SunkList.push_back(BoatList[i]);
			BoatList.erase(BoatList.begin() + i);
			isSunk = Border = true;
			break;
		}
	}

	if (Hits == 20)
		Over = true;

	return flag;
}

bool Map::Contains(Ship& ship) {
	for (Ship SunkBoat : SunkList)
		if (SunkBoat == ship)
			return true;
	return false;
}