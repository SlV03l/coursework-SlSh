#include "Cell.h"

Cell::Cell(float x, float y) {	// позиция картинки												
	isHit = isMiss = isBorder = isBoat = false;
	pos = sf::Vector2f(x, y); // ветктор перемещения, для двумерного массива
	sprite.setPosition(pos); // установка позиции
}

bool Cell::SetUpSprite(string TextureName) { // установка картинки
	if (!texture.loadFromFile(TextureName))
		return false;
	sprite.setTexture(texture);
	return true;
}

sf::Sprite Cell::getSprite() {
	return sprite;
}

void Cell::setHit() { // картинка при попадании
	SetUpSprite("Images\\Hit.png");
	isHit = true;
}

void Cell::setMiss() { // картинка при промахе
	SetUpSprite("Images\\Miss.png");
	isMiss = true;
}

void Cell::setBorder() { // картинка по контуру корабля
	if (!isBoat) {
		SetUpSprite("Images\\Border.png");
		isBorder = true;
	}
}

void Cell::setBoat() {
	isBoat = true;
}

bool Cell::getBorder() {
	return isBorder;
}

bool Cell::getHit() {
	return isHit;
}

bool Cell::getBlocked() {
	return (isHit || isMiss || isBorder);
}

bool Cell::getBoat() {
	return isBoat;
}