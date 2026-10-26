#include "Graphic/GameGeometry.h"

namespace Go
{
    GameGeometry::GameGeometry(float cellSize, float margin, int boardSize) :
        m_cellSize(cellSize), 
        m_margin(margin),
        m_boardSize(boardSize) {}
    
    
    Coordinate GameGeometry::pixelsToCoordinates(int px, int py) const
    {
        int x = std::round((static_cast<float>(px) - m_margin) / m_cellSize);
        int y = std::round((static_cast<float>(py) - m_margin) / m_cellSize);
    
        x = std::clamp(x, 0, m_boardSize - 1);
        y = std::clamp(y, 0, m_boardSize - 1);
    
        return {x, y};
    }
    
    float GameGeometry::getCellSize() const
    {
        return m_cellSize;
    }
    
    float GameGeometry::getMargin() const
    {
        return m_margin;
    }
    
    float GameGeometry::getBoardSize() const
    {
        return m_boardSize;
    }
}    
