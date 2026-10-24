#include "Logic/Core/Board.h"

Board::Board(int boardSize)
{
    /**
     * boardSize    -> rozmiar planszy bez scian
     * vectorWidth  -> rozmiar planszy ze scianami
     */

    m_boardSize = boardSize;
    m_trueBoardSize = m_boardSize+2;

    m_fieldCount = m_boardSize * m_boardSize;
    m_trueFieldCount = m_trueBoardSize * m_trueBoardSize;
    m_boardState.reserve(m_trueFieldCount);

    /**
     * petla okresla granice planszy i wyznaczy jej sciany
     * sciany to niewidoczne dla gracza pozycje znajdujaca sie na skraju planszy,
     * potrzebna do obliczen stanu planszy
     * 
     * przykladowy output w postaci graficznej dla planszy 9x9:
     *   * * * * * * * * * * * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * * * * * * * * * * *
     */
    for(int i = 0; i < m_trueFieldCount; i++)
    {
        int rowIndex = std::floor(i / (m_trueBoardSize));
        int columnIndex = i - ((m_trueBoardSize) * rowIndex);

        if(rowIndex == 0  || columnIndex == 0 || rowIndex == m_boardSize+1 || columnIndex == m_boardSize+1)
            m_boardState.emplace_back(Intersection{Stone::Wall});
        else
            m_boardState.emplace_back(Intersection{Stone::None});
    }
}

int Board::getBoardSize() const
{
    return m_boardSize;
}

void Board::setBoardState(const std::vector<Intersection>& newBoardState)
{
    m_boardState = newBoardState;
}

int Board::convertCoordinatesToIndex(Coordinate coordinates) const
{
    return (m_trueBoardSize * (coordinates.y + 1)) + (coordinates.x + 1);         
}

Coordinate Board::convertIndexToCoordinates(int index) const
{
    int y = std::floor(index / (m_trueBoardSize));
    int x = index - ((m_trueBoardSize) * y);

    return {x-1, y-1};
}

std::vector<Intersection> Board::getBoardState() const
{
    return m_boardState;
}

bool Board::isEmptyAt(int index) const
{
    return m_boardState[index].isEmpty();
}


//wrong?
bool Board::isLegal(int index) const
{
    return index > 0 || index < m_fieldCount-1;
}

bool Board::isLegal(Coordinate coordinates) const
{
    return coordinates.x >= 0 && coordinates.y >= 0 && coordinates.x < m_boardSize  && coordinates.y < m_boardSize ;
}

void Board::setStone(int index, Stone color)
{
    m_boardState[index].stone = color;
}

void Board::setStone(Coordinate coordinates, Stone color)
{
    int index = this->convertCoordinatesToIndex(coordinates);
    m_boardState[index].stone = color;
}

void Board::setPreviousBoardState(std::vector<Intersection> boardState)
{
    m_previousBoardState = boardState;
}

std::vector<Intersection> Board::getPreviousBoardState() const
{
    return m_previousBoardState;
}

Stone Board::getStone(int index) const
{
    return m_boardState[index].stone;
}

Stone Board::getStone(Coordinate coordinates) const
{
    int index = this->convertCoordinatesToIndex(coordinates);
    return m_boardState[index].stone;
}

Group Board::floodFill(Coordinate startCoordinates, Stone groupColor) const
{
    //nie potrzebne
    //int boardSize = this->getBoardSize();

    /**
     * todo:
     *      zastapic liberties int-em? 
     *      done
     */
    std::vector<bool> visited(m_boardSize*m_boardSize, false);
    std::vector<Coordinate> group;
    //std::set<Coordinate> liberties;
    int libertiesCount = 0;
    std::queue<Coordinate> queue;

    int startIndex = convertCoordinatesToIndex(startCoordinates);
    visited[startIndex] = true;
    queue.push(startCoordinates);
    group.push_back(startCoordinates);
    
    while(!queue.empty())
    {
        Coordinate currentStoneCoordinates = queue.front();
        queue.pop();


        for(const auto& direction : DIRECTIONS)
        {
            Coordinate neighbourCoordinates = {currentStoneCoordinates.x + direction.x, currentStoneCoordinates.y + direction.y};

            //jezeli sciana, pomin
            if(!isLegal(neighbourCoordinates))
                continue;

            Stone neighbourColor = getStone(neighbourCoordinates);
            int index = convertCoordinatesToIndex(neighbourCoordinates);

            if(neighbourColor == groupColor && !visited[index])
            {
                visited[index] = true;
                group.push_back(neighbourCoordinates);
                queue.push(neighbourCoordinates);
            }
            else if(neighbourColor == Stone::None)
            {
                //liberties.insert(neighbourCoordinates);
                libertiesCount++;
            }
        }
    }
    //return {(int)liberties.size(), stones};
    return {libertiesCount, group};
}

/**
 * todo
 *      wrzucic gdzies indziej niz board.cc
 */
void Board::printBoardState() const
{
    int spaceCounter = 0;
    for(const auto& i : m_boardState)
    {
        if (i.isWall())
            std::cout << "*" << " ";
        else if (i.isEmpty())
            std::cout << "-" << " ";
        else if (i.stone == Stone::Black)
            std::cout << "B" << " ";
        else
            std::cout << "W" << " ";

        spaceCounter = (spaceCounter + 1) % (m_boardSize+2);
        if (spaceCounter == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}
