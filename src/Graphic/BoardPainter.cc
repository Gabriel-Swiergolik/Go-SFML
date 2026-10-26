#include "Graphic/BoardPainter.h"

namespace Go
{
    BoardPainter::BoardPainter(sf::RenderWindow& window, const Board& board, const GameGeometry& gameGeometry) :
        m_window(window),
        m_board(board),
        m_gameGeometry(gameGeometry)
    {
        int boardSize = m_gameGeometry.getBoardSize();
        float cellSize = m_gameGeometry.getCellSize();
        float margin = m_gameGeometry.getMargin();
    
        m_linesArray = sf::VertexArray(sf::Lines, m_gameGeometry.getBoardSize() * 4);
        for(int i = 0; i < boardSize; ++i)
        {
            float offset = i * cellSize;
            int baseIndex = i * 4;
    
            m_linesArray[baseIndex + 0].position = {margin, margin + offset};
            m_linesArray[baseIndex + 1].position = {margin + (boardSize - 1) * cellSize, margin + offset};
    
            m_linesArray[baseIndex + 2].position = {margin + offset, margin};
            m_linesArray[baseIndex + 3].position = {margin + offset, margin + (boardSize - 1) * cellSize};
    
            for(int j = 0; j < 4; j++)
                m_linesArray[baseIndex + j].color = sf::Color::Black;
        }
    
        m_hoshiShape = sf::CircleShape(3.f);  
        m_hoshiShape.setFillColor(sf::Color::Black);
        m_hoshiShape.setOrigin(m_hoshiShape.getRadius(), m_hoshiShape.getRadius());
        std::vector<sf::Vector2f> relativeHoshiPoints;
        switch(boardSize)
        {
            case 9:
                relativeHoshiPoints = {{2,2}, {6,2},
                                          {4, 4}, 
                                       {2,6}, {6,6}};
                break;
            case 13:
                relativeHoshiPoints = {{3,3}, {9,3}, 
                                          {6, 6},
                                       {3,9}, {9,9}};
                break;
            case 19:
                relativeHoshiPoints = {{3, 3}, {9, 3}, {15, 3},
                                       {3, 9}, {9, 9}, {15, 9},
                                       {3,15}, {9,15}, {15,15}}; 
                break;
            default:
                //todo
                break;
        }
    
        for(sf::Vector2f& point : relativeHoshiPoints)
        {
            m_hoshiPositions.push_back(
            {
                margin + point.x * cellSize, 
                margin + point.y * cellSize
            });
        }
                                         
        m_stoneShape = sf::CircleShape((m_gameGeometry.getCellSize() / 2) - 2);
        m_stoneShape.setOrigin(m_stoneShape.getRadius(), m_stoneShape.getRadius());
    
        float boardPixelSize = (boardSize - 1) * cellSize + 2 * margin; 
        m_boardShape = sf::RectangleShape(sf::Vector2f(boardPixelSize, boardPixelSize));
        m_boardShape.setFillColor(sf::Color(245, 170, 59, 255));
        m_boardShape.setOrigin(m_boardShape.getSize() / 2.f);
        m_boardShape.setPosition(sf::Vector2f(m_window.getSize()) / 2.f);
    }
    
    void BoardPainter::drawLines()
    {
        m_window.draw(m_linesArray);
    }
    
    void BoardPainter::drawHoshi()
    {
        for (auto& position : m_hoshiPositions)
        {
            m_hoshiShape.setPosition(position);
            m_window.draw(m_hoshiShape);
        }
    }
    
    void BoardPainter::drawStoneHighlight(Stone currentPlayer)
    {
        currentPlayer == Stone::Black ? m_stoneShape.setFillColor(sf::Color(0, 0, 0, 128)) : m_stoneShape.setFillColor(sf::Color(255, 255, 255, 128));
    
        sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
    
    
        //Maybe do a function later?
        Coordinate coordinates = m_gameGeometry.pixelsToCoordinates(mousePosition.x, mousePosition.y);
        int index = m_board.convertCoordinatesToIndex(coordinates);
        //endComment
    
        auto boardState = m_board.getBoard();
        float margin = m_gameGeometry.getMargin();
        float cellSize = m_gameGeometry.getCellSize();
        if(boardState[index].isEmpty())
        {
            m_stoneShape.setPosition(margin + coordinates.x * cellSize, margin + coordinates.y * cellSize);
            m_window.draw(m_stoneShape);
        }
    
    }
    
    void BoardPainter::drawStone(int index)
    {
        Stone logicStone = m_board.getStone(index);
        logicStone == Stone::Black ? m_stoneShape.setFillColor(sf::Color::Black) : m_stoneShape.setFillColor(sf::Color::White);
    
        Coordinate coordinates = m_board.convertIndexToCoordinates(index);
        float cellSize = m_gameGeometry.getCellSize();
        float margin = m_gameGeometry.getMargin();
        m_stoneShape.setPosition(margin + coordinates.x * cellSize, margin + coordinates.y * cellSize);
        m_window.draw(m_stoneShape);
    }
    
    void BoardPainter::drawBoard()
    {
        m_window.draw(m_boardShape);
    
        this->drawLines();
        this->drawHoshi();
    
        auto boardState = m_board.getBoard();
        for(int index = 0; index < boardState.size(); index++)
        {
            auto intersection = boardState[index];
            if( !(intersection.isEmpty() || intersection.isWall()) )
                drawStone(index);
        }
    }
}
