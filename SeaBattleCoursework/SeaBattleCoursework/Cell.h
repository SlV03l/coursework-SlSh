#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class Cell
{
private:
	sf::Vector2f pos;
	sf::Texture texture;
	sf::Sprite sprite;
	string TextureName;
	bool isHit, isMiss, isBorder, isBoat;
public:
	Cell(float x, float y);
	bool SetUpSprite(string TextureName);
	sf::Sprite getSprite();
	void setHit();
	void setMiss();
	void setBorder();
	void setBoat();
	bool getBorder();
	bool getBlocked();
	bool getBoat();
	bool getHit();
};