#include "Outcome.h"

using namespace sf;

Outcome::Outcome(sf::RenderWindow* window, State& state) {
    this->ResWindow = window;
    if (state == State::User) { // проверка на результат игры
        ResTexture.loadFromFile("Images\\Winner.jpg");
    }
    else {
        ResTexture.loadFromFile("Images\\Loser.jpg");
    }
    ResSprite.setTexture(ResTexture);
    ResSprite.setPosition(0, 0);
};