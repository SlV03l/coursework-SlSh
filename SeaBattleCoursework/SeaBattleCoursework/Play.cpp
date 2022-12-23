#include "Play.h"
//#include "Result.hpp"

using namespace sf;

Play::Play(RenderWindow& win) {

    state = State::User; // ход начинается с игрока
    GameOver = false; // конец игры

    // звуки
    SoundBuffer water, hit, destruction;
    water.loadFromFile("Sounds/Splash.ogg");
    hit.loadFromFile("Sounds/Explosion.ogg");
    destruction.loadFromFile("Sounds/Shoot.ogg");
    Sound waterPl(water);
    Sound hitPl(hit);
    Sound destructionPl(destruction);

    // картинки
    RenderWindow WinPlay(VideoMode(1280, 720), "Play");

    MapBackTexture.loadFromFile("Images/Map.jpg");
    QueueTexture.loadFromFile("Images/Queue.png");
    TurnTexture.loadFromFile("Images/Miss.png");
    button01.loadFromFile("Images/buttons_01.png");
    MapBackSprite.setTexture(MapBackTexture);
    QueueSprite.setTexture(QueueTexture);
    TurnSprite.setTexture(TurnTexture);
    Sprite buttonSprite2(button01);
    buttonSprite2.setTextureRect(IntRect(0, 63, 222, 48));
    MapBackSprite.setPosition(0, 0);
    QueueSprite.setPosition(595, 280);
    TurnSprite.setPosition(659,280);
    buttonSprite2.setPosition(530, 348);

    // местоположение начала карты игрока и компьютера
    Map UserMap(152, 179, true);
    Map CompMap(152, 783, false);
    
    Computer Comp; // подключение компьютера

    while (WinPlay.isOpen() && !GameOver) { // отрисовка картинок в окне
        while (WinPlay.pollEvent(GameEvent)) {
            if (GameEvent.type == Event::Closed) {
                WinPlay.close();
            }
        }
        if (Mouse::isButtonPressed(Mouse::Left) && buttonSprite2.getGlobalBounds().contains(WinPlay.mapPixelToCoords(Mouse::getPosition(WinPlay)))) {
            destructionPl.play();
            sleep(sf::milliseconds(500));
            WinPlay.close();
        }
        WinPlay.clear();
        WinPlay.draw(MapBackSprite);
        WinPlay.draw(QueueSprite);
        WinPlay.draw(buttonSprite2);
        WinPlay.draw(TurnSprite);

        for (int i = 0; i < 10; i++) { // отрисовка поля для игрока и компьютера
            for (int j = 0; j < 10; j++) {
                WinPlay.draw(CompMap.getCell(i, j)->getSprite());
                WinPlay.draw(UserMap.getCell(i, j)->getSprite());
            }
        }
        WinPlay.display();

        GameOver = UserMap.isOver() || CompMap.isOver(); // вывод картинки при победе/проигрыше
        //if (GameOver) //{
            //if (state == State::User)
             //   music.openFromFile("Sound\\Ta_Da.wav");
           // else
             //   music.openFromFile("Sound\\sea_gulls.wav");
            //Result res(state);
       // }

        bool isSunk = false; // потоплен
        if (state == State::User) {
            if (Player(CompMap, WinPlay, win, isSunk)) {
                if (!isSunk)
                    hitPl.play();
                else
                    destructionPl.play();
            }
            else
                waterPl.play();
        }
        else {
            sleep(milliseconds(1000));
            if (Comp.Strike(UserMap, isSunk)) { //
                if (!isSunk)
                    hitPl.play();
                else { //
                    destructionPl.play();
                    vector<pair<int, int>> coords = UserMap.getBorderCoords();
                    Comp.UpdateTacticList(UserMap, coords);
                }
            }
            else { // промах
                waterPl.play();
                Turn();
            }
        }

    }

}

void Play::Turn() { 
    if (state == State::User) {
        state = State::Computer;
    }
    else {
        state = State::User;
    }
}

bool Play::Player(Map& CompMap, RenderWindow& MapWindow, RenderWindow& win, bool& isSunk) {
    bool isHit = CompMap.setClick(MapWindow, GameEvent, win, isSunk);
    if (!isHit)
        Turn();
    return isHit;
}