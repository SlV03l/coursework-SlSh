#include "Map.h"
#include <iostream>
using namespace sf;

Map::Map() { // конструктор по умолчанию
	Over = Border = false;
	ShowBoats = true;
	Hits = 0;
}

Map::Map(int i_start, int j_start, bool ShowBoats) { // расстановка клеток (поле для игрока и компьютера)
	cells.clear();
	for (int i = 0, i_shift = i_start; i < 10; i++, i_shift += 32) { // сдвиг для установки клетки
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

Cell* Map::getCell(int i, int j) { // возвращает картинку
	return cells[i][j];
}

bool Map::isOver() { // возвращает значение, что игра окончена
	return Over;
}

int Map::getSunkNumber() { //
	return SunkList.size();
}

void Map::setBoatList() { // запись списка всех кораблей
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

void Map::setBoats() { // установка кораблей
	setBoatList();
	for (int count = 0; count < BoatList.size(); count++) {
		BoatList[count].setDirection(rand() % 2); // установка случайных направлений кораблей
		string direction = BoatList[count].getDirection(); // присваивание названия направления
		BoatList[count].setCoords(); // установка координат корабля
		while (FreeSpace(BoatList[count]) == false) { // проверка на свободное место для корабля, если нет свободной области, то мы ее меняем
			BoatList[count].setDirection(rand() % 2);
			string direction = BoatList[count].getDirection();
			BoatList[count].setCoords();
		}
		if (BoatList[count].getDirection() == "Horizontal") // установка корабля (+ установка картинки корабля)
			for (int col = BoatList[count].getCol(); col < BoatList[count].getCol() + BoatList[count].getSize(); col++) {
				cells[BoatList[count].getRow()][col]->setBoat(); // устанавливает корабль
				if (ShowBoats) // условие, что корабль видно только на своем поле
					cells[BoatList[count].getRow()][col]->SetUpSprite("Images\\Ship.png");
			}
		else // условия для корабля по вертикалм
			for (int row = BoatList[count].getRow(); row < BoatList[count].getRow() + BoatList[count].getSize(); row++) {
				cells[row][BoatList[count].getCol()]->setBoat();
				if (ShowBoats)
					cells[row][BoatList[count].getCol()]->SetUpSprite("Images\\Ship.png");
			}
	}
}

bool Map::FreeSpace(Ship& ship) { // функция для проверки на свободную область для установки корабля
	if (ship.getDirection() == "Horizontal"){
		for (int i = ship.getRow() - 1; i < ship.getRow() + 2; i++)
			for (int j = ship.getCol() - 1; j < ship.getCol() + ship.getSize() + 1; j++)
				if (i < 0 || i >= 10 || j < 0 || j >= 10) // проверка на нахождение в границах поля
					continue;
				else if (cells[i][j]->getBoat() || cells[i][j]->getBlocked() || cells[i][j]->getBorder()) // проверка есть ли что-то в этой клетке
					return false;
	}
	else{ // для вертикали
		for (int i = ship.getRow() - 1; i < ship.getRow() + ship.getSize() + 1; i++)
			for (int j = ship.getCol() - 1; j < ship.getCol() + 2; j++)
				if (i < 0 || i >= 10 || j < 0 || j >= 10)
					continue;
				else if (cells[i][j]->getBoat() || cells[i][j]->getBlocked() || cells[i][j]->getBorder())
					return false;
	}
	return true;
}


bool Map::setClick(RenderWindow& WinPlay, Event& GameEvent, RenderWindow& win, bool& isSunk) { // проверка попал ли игрок в корабль
	bool repeat = true, isHit = false;
	while (repeat) {
		while (WinPlay.pollEvent(GameEvent)) {
			if (GameEvent.type == Event::Closed) {
				WinPlay.close();
				win.close();
			}
		}
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (!cells[i][j]->getBlocked()) { // проверка на возможность нажатия
					if (Mouse::isButtonPressed(Mouse::Left) && cells[i][j]->getSprite().getGlobalBounds().contains(WinPlay.mapPixelToCoords(Mouse::getPosition(WinPlay)))) { //проверка нажатия
						isHit = Strike(i, j, isSunk); // проверка на попадание по кораблю
						repeat = false;
						break;
					}
				}
			}
		}
	}
	return isHit;
}

void Map::setBorder(Ship& ship) { // функция для установки границ вокруг корабля
	if (ship.getDirection() == "Horizontal") {
		for (int i = ship.getRow() - 1; i < ship.getRow() + 2; i++)
			for (int j = ship.getCol() - 1; j < ship.getCol() + ship.getSize() + 1; j++)
				if (i < 0 || i >= 10 || j < 0 || j >= 10)
					continue;
				else
					cells[i][j]->setBorder();
	}
	else {
		for (int i = ship.getRow() - 1; i < ship.getRow() + ship.getSize() + 1; i++)
			for (int j = ship.getCol() - 1; j < ship.getCol() + 2; j++)
				if (i < 0 || i >= 10 || j < 0 || j >= 10)
					continue;
				else
					cells[i][j]->setBorder();
	}
}

void Map::setBorderCoords(Ship& ship) { // запись координат границ вокург уничтоженного корабля
	if (ship.getDirection() == "Horizontal") {
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
	else { // для вертикпли
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

vector<pair<int, int>> Map::getBorderCoords() { // функция для возвращения координат вокруг корабля
	return BorderCoords;
}

bool Map::Strike(int i, int j, bool& isSunk) { // функция проверки на попадание по кораблю
	bool check;
	Border = false;

	if (cells[i][j]->getBoat()) { // если в этом месте есть корабль, то идет замена на попадание (+ картинка)
		cells[i][j]->setHit();
		Hits++;
		check = true;
	}
	else { // при промахе
		cells[i][j]->setMiss();
		check = false;
	}

	isSunk = false;
	for (int i = 0; i < BoatList.size(); i++) { 
		if (BoatList[i].isSunk(cells) && !(Contains(BoatList[i]))) { // проверка полностью ли уничтожен корабль
			setBorder(BoatList[i]); // если корабль уничтожен, то происходит установка границ вокруг него (+ картинка)
			BorderCoords.clear();
			setBorderCoords(BoatList[i]); // запись границ вокруг корабля (функция нужна для компьютера)
			SunkList.push_back(BoatList[i]); // запись корабля в список уничтоженных кораблей 
			BoatList.erase(BoatList.begin() + i); // удаление корабля из списка
			isSunk = Border = true;
			break;
		}
	}
	if (Hits == 20) // условия окончания игры
		Over = true;
	return check;
}

bool Map::Contains(Ship& ship) { // проверка уничтожен ли корабль
	for (Ship SunkBoat : SunkList)
		if (SunkBoat == ship)
			return true;
	return false;
}