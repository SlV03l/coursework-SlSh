#pragma once
#include "Ship.h"
#include "Map.h"
#include "Computer.h"

enum class State { // установка пространства имен
	User, Computer 
};

class Play {
private:
	State state;
	sf::RenderWindow WinPlay;
	sf::Texture MapBackTexture, QueueTexture, UserFieldTexture, CompFieldTexture, TurnTexture, button01;
	sf::Sprite MapBackSprite, QueueSprite, UserFieldSprite, CompFieldSprite, TurnSprite;
	sf::Event GameEvent{};
	Map UserMap, CompMap;
	bool GameOver;
	sf::Music music;
private:
	void Turn();
	bool Player(Map& CompMap, sf::RenderWindow& window, sf::RenderWindow& win, bool& isSunk);
public:
	Play(sf::RenderWindow& win);
};