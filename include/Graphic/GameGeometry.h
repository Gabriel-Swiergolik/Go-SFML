#pragma once

#include <cmath>
#include <algorithm>
#include "Common.h"

namespace Go
{
    class GameGeometry
    {
    private:
        float m_cellSize;
        float m_margin;
        int m_boardSize; //Not sure about that
    
    public:
        GameGeometry(float cellSize, float margin, int boardSize);
    
        Coordinate pixelsToCoordinates(int px, int py) const; 
        float getCellSize() const;
        float getMargin() const;
        float getBoardSize() const;
    };
}

