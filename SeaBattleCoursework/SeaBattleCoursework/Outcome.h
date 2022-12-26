#pragma once
#include "Play.h"

using namespace sf;

class Outcome {
private:
	RenderWindow *ResWindow;
	Texture ResTexture;
	Sprite ResSprite;
	Event ResEvent{};
	Music music;
public:
	Outcome(sf::RenderWindow* window, State& state);
	void draw() { // отрисовка
		this->ResWindow->draw(this->ResSprite);
	}
};