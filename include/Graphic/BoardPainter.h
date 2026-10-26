#pragma once

#include "Graphic/GameGeometry.h"
#include "Logic/Core/Board.h"
#include <SFML/Graphics.hpp>


namespace Go
{
    class BoardPainter
    {
    private:
        sf::RenderWindow& m_window;
        const Board& m_board;
        const GameGeometry& m_gameGeometry;
    
        sf::VertexArray m_linesArray;
        sf::CircleShape m_hoshiShape;
        std::vector<sf::Vector2f> m_hoshiPositions;
        sf::CircleShape m_stoneShape;
        sf::RectangleShape m_boardShape;
    
        void drawLines();
        void drawHoshi(); 
    
    public:
        BoardPainter(sf::RenderWindow& window, const Board& board, const GameGeometry& gameGeometry);
    
        void drawStoneHighlight(Stone currentPlayer);
        void drawStone(int index);
        void drawBoard();
        
    };
}
