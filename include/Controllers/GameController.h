#pragma once

#include <SFML/Graphics.hpp>
#include "Graphic/BoardPainter.h"
//#include "Logic/Core/GameLogic.h"

namespace Go
{
    class GameController
    {
    private:
        //sf::RenderWindow& m_window;
        Board m_board;
        GameGeometry m_gameGeometry;
        BoardPainter m_boardPainter;
    
    
    
    public:
        //GameController(float cellSize, float margin, int boardSize, sf::ContextSettings windowSettings);
        GameController(float cellSize, float margin, int boardSize, sf::RenderWindow& window);
    
        Board& getBoard();
        const GameGeometry& getGameGemetry() const;
        BoardPainter& getBoardPainter();

        void run(sf::RenderWindow& window);
        void processEvents(sf::RenderWindow& window);
        void update(sf::RenderWindow& window);        
        void render(sf::RenderWindow& window);    
    };
}
