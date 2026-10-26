// #pragma once

// #include "Logic/Core/Board.h"
// #include <bit>
// #include <bitset>

// //TO BE NUKED -> merged with board

// namespace Go
// {



//     class GameLogic
//     {
//     private:

        
//         Board& m_board;
  

//     public:
//         GameLogic(Board& board);

//         void incrementScore(Stone color);
//         Score getScore() const;

//         Stone getCurrentPlayer() const;
//         const Board& getBoard() const; //probably not needed

//         void checkNeighbours(const Coordinate& stoneCoordinates, std::vector<bool>& visited);
//         bool isSuicide(const Coordinate& stoneCoordinates );
//         bool isKoViolation();

//         void placeStone(int index);
//         void captureGroup(std::vector<Coordinate>& group);


//         void changePlayer();

//     };
// }


