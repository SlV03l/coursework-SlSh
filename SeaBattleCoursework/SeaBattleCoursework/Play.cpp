#include "Play.h"
#include "Outcome.h"

using namespace sf;

Play::Play(RenderWindow& win) {

    srand((unsigned int)time(NULL));
    state = State::User; // ход начинается с игрока
    GameOver = false; // конец игры

    // звуки
    SoundBuffer water, hit, destruction, start;
    water.loadFromFile("Sounds/Splash.ogg");
    hit.loadFromFile("Sounds/Explosion.ogg");
    destruction.loadFromFile("Sounds/Shoot.ogg");
    start.loadFromFile("Sounds/Shoot.ogg");
    Sound waterPl(water);
    Sound hitPl(hit);
    Sound destructionPl(destruction);
    Sound startPl(start);

    // картинки
    RenderWindow WinPlay(VideoMode(1280, 720), "Play");

    MapBackTexture.loadFromFile("Images/Map.jpg");
    QueueTexture.loadFromFile("Images/Queue.png");
    TurnTexture.loadFromFile("Images/Turn.png");
    button01.loadFromFile("Images/buttons_01.png");
    Sprite buttonSprite2(button01);
    buttonSprite2.setTextureRect(IntRect(0, 63, 222, 48));
    MapBackSprite.setTexture(MapBackTexture);
    QueueSprite.setTexture(QueueTexture);
    TurnSprite.setTexture(TurnTexture);
    MapBackSprite.setPosition(0, 0);
    QueueSprite.setPosition(595, 280);
    buttonSprite2.setPosition(530, 500);

    // местоположение начала карты игрока и компьютера
    Map UserMap(152, 179, true);
    Map CompMap(152, 783, false);
    
    // подключение компьютера
    Computer Comp; 

    // отрисовка картинок в окне
    while (WinPlay.isOpen()) {
        while (WinPlay.pollEvent(GameEvent)) {
            if (GameEvent.type == Event::Closed) {
                WinPlay.close();
            }
        }
        WinPlay.clear();
        WinPlay.draw(MapBackSprite);
        WinPlay.draw(QueueSprite);

        for (int i = 0; i < 10; i++) { // отрисовка поля для игрока и компьютера
            for (int j = 0; j < 10; j++) {
                WinPlay.draw(CompMap.getCell(i, j)->getSprite());
                WinPlay.draw(UserMap.getCell(i, j)->getSprite());
            }
        }
        
        if (state == State::User) { // показывает чей ход в данный момент
            TurnSprite.setPosition(656, 280);
            WinPlay.draw(TurnSprite);
        }
        else {
            TurnSprite.setPosition(595, 280);
            WinPlay.draw(TurnSprite);
        }

        WinPlay.display();

        GameOver = UserMap.isOver() || CompMap.isOver(); 
       
        if (GameOver) { // проверка исхода игры 
            Outcome res(&WinPlay, state);
            res.draw();
            WinPlay.draw(buttonSprite2);
            WinPlay.display();
            if (Mouse::isButtonPressed(Mouse::Left) && buttonSprite2.getGlobalBounds().contains(WinPlay.mapPixelToCoords(Mouse::getPosition(WinPlay)))) {
                startPl.play();
                sleep(milliseconds(1000));
                WinPlay.close();
                return;
            }
            sleep(sf::milliseconds(10000));
            WinPlay.close();
            Play game(WinPlay);
        }

        bool isSunk = false;

        if (state == State::User) { // проверка хода
            if (Player(CompMap, WinPlay, win, isSunk)) { // ход пользователя
                if (!isSunk)
                    hitPl.play();
                else
                    destructionPl.play();
            }
            else
                waterPl.play();
        }
        else { // ход игрока
            sleep(milliseconds(1000));
            if (Comp.Strike(UserMap, isSunk)) { // попадание
                if (!isSunk)
                    hitPl.play();
                else { // уничтожение
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

void Play::Turn() { // меняет очередь
    if (state == State::User) {
        state = State::Computer;
    }
    else {
        state = State::User;
    }
}

bool Play::Player(Map& CompMap, RenderWindow& MapWindow, RenderWindow& win, bool& isSunk) { // проверка попал ли игрок в корабль
    bool isHit = CompMap.setClick(MapWindow, GameEvent, win, isSunk);
    if (!isHit)
        Turn();
    return isHit;
}