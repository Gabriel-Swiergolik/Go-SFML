#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "Graphic/BoardPainter.h"
#include "Logic/Core/GameLogic.h"

class GameController
{
private:
    sf::RenderWindow& m_window;
    Board m_board;
    GameLogic m_gameLogic;
    GameGeometry m_gameGeometry;
    BoardPainter m_boardPainter;

    void processEvents();
    void update();        
    void render();        

public:
    //GameController(float cellSize, float margin, int boardSize, sf::ContextSettings windowSettings);
    GameController(float cellSize, float margin, int boardSize, sf::RenderWindow& window);

    void run();
};

#endif