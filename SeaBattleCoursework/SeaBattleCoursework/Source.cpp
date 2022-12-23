#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Cell.h"
#include "Play.h"
using namespace sf;

int main() {
	// подключение окна
	RenderWindow win(VideoMode (1280, 720), "Coursework");
	// музыка и звуки
	Music Screensaver;
	Screensaver.openFromFile("Sounds/Pirates.ogg");
	Screensaver.play();
	SoundBuffer start;
	start.loadFromFile("Sounds/Shoot.ogg");
	Sound startPl(start);

	// установка картинок
	Texture map, button01;
	map.loadFromFile("Images/Map.jpg");
	button01.loadFromFile("Images/buttons_01.png");
	Sprite mapsprite(map), buttonSprite1(button01), buttonSprite2(button01);
	mapsprite.setPosition(0, 0);
	buttonSprite1.setTextureRect(IntRect(0, 0, 222, 48));
	buttonSprite2.setTextureRect(IntRect(0, 63, 222, 48));
	buttonSprite1.setPosition(530, 318);
	buttonSprite2.setPosition(530, 383);

	while (win.isOpen()) {
		Event ev;
		while (win.pollEvent(ev)) {
			if (ev.type == Event::Closed)
				win.close();
		}
		// кнопка для старта игры
		if (Mouse::isButtonPressed(Mouse::Left) && buttonSprite1.getGlobalBounds().contains(win.mapPixelToCoords(Mouse::getPosition(win)))) {
			startPl.play();
			win.close();
			Play game(win);
		}
		// кнопка для выхода из программы
		if (Mouse::isButtonPressed(Mouse::Left) && buttonSprite2.getGlobalBounds().contains(win.mapPixelToCoords(Mouse::getPosition(win)))) {
			startPl.play();
			sleep(milliseconds(500));
			win.close();
		}
		win.clear();
		win.draw(mapsprite);
		win.draw(buttonSprite1);
		win.draw(buttonSprite2);
		win.display();
	}
	return 0;
}