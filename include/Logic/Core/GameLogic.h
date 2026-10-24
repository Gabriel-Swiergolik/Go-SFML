#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "Logic/Core/Board.h"

struct Score
{
    float White;
    float Black;
};

class GameLogic
{
private:
    Board& m_board;
    Stone m_currentPlayer;
    Score m_score;    

public:
    GameLogic(Board& board);

    void incrementScore(Stone color);
    Score getScore() const;

    Stone getCurrentPlayer() const;
    const Board& getBoard() const; //probably not needed

    void checkNeighbours(const Coordinate& stoneCoordinates, std::vector<bool>& visited);
    bool isSuicide(const Coordinate& stoneCoordinates );
    bool isKoViolation();

    void placeStone(int index);
    void captureGroup(std::vector<Coordinate>& group);


    void changePlayer();

};

#endif