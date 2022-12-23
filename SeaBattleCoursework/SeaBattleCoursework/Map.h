#pragma once
#include "Cell.h"
#include "Ship.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Map {
private:
	vector<vector<Cell*>> cells;
	vector <Ship> BoatList, SunkList;
	vector<pair<int, int>> BorderCoords;
	int Hits;
	bool Over, ShowBoats, Border;
private:
	void setBoatList();
	void setBoats();
	bool FreeSpace(Ship& ship);
	void setBorderCoords(Ship& ship);
	void setBorder(Ship& ship);
	bool Contains(Ship& ship);
public:
	Map();
	Map(int i_start, int j_start, bool ShowBoats);
	Cell* getCell(int i, int j);
	bool setClick(sf::RenderWindow& window, sf::Event& GameEvent, sf::RenderWindow& win, bool& isSunk);
	vector<pair<int, int>> getBorderCoords();
	bool isOver();
	int getSunkNumber();
	bool Strike(int i, int j, bool& isSunk);
};