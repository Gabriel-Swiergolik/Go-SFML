#pragma once

#include "Common.h"
#include "LibertyBitset.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <cmath>

#include <set>
#include <queue>

namespace Go
{
    //probably depreciated
    /**
     * Obiekt:
     *      Group
     * Rola:
     *      przechowuje ilosc "zyc" grupy oraz koordynaty wszystkich kamieni
     *      uzywany do obliczen stanu planszy
     */
    struct Group
    {
        int libertyCount;
        std::vector<Go::Coordinate> stoneCoordinates;
    };

    struct Score
    {
        float White;
        float Black;
    };
    
    /**
     * Obiekt:
     *      Intersection
     * Rola:
     *      okresla charakter kazdego skrzyzowania na planszy i jej stan
     *      uzywana w trakcie rysowania planszy oraz obliczen jej stanu
     */
    struct Intersection 
    {
        Stone stone = Stone::None;
    
        bool isEmpty() const { return stone == Stone::None; }
        bool isWall() const { return stone == Stone::Wall; }
    
        bool operator==(const Intersection& i) const {return stone == i.stone; }
    };
    
    
     // Prawdopodobnie potrzebny refactor //
    
    /**
     * Obiekt:
     *      Board
     * Rola:
     *      przechowuje stany planszy
     *      odpowiada za operacja logiczne na planszy
     */
    
     // Prawdopodobnie potrzebny refactor //
    
    class Board
    {
    private:
        //array holding root index of each element
        std::array<int16_t, MAX_BOARD_AREA> m_rootArray{};
        //array holding chain size of each root
        std::array<int16_t, MAX_BOARD_AREA> m_sizeArray{};
        //array holding index of the next stone of a given element
        std::array<int16_t, MAX_BOARD_AREA> m_circularRingArray{};
        //array holding liberty bitsets of each element
        std::array<LibertyBitset, MAX_BOARD_AREA> m_libertyArray {};
        //array holding board data
        std::array<Intersection, MAX_BOARD_AREA> m_board{};

        int m_playSize;
        int m_boardSize;
        uint64_t m_currentHash;
        uint64_t m_previousHash;

        Stone m_currentPlayerColor;
        Score m_score;  

        //unionFind helpers
        int unionChains(int parentA, int parentB);
        int findRoot(int child);

    
    public:
        Board(const int playSize);
    
        int getBoardSize() const;
        const std::array<Intersection, MAX_BOARD_AREA>& getBoard() const;

        Stone getCurrentPlayer() const;

        Stone getStone(int index) const;
        void setStone(int index, Stone color);

        int convertCoordinatesToIndex(Coordinate coordinates) const;
        Coordinate convertIndexToCoordinates(int index) const;
    
        bool isEmptyAt(int index) const;
        bool isLegal(int index) const;
        bool isLegal(Coordinate coordinates) const;

        //maybe private? should be used only within this class anyway
        bool tryPlacingStone(const int sourceIndex);
        void placeStone(int index);

        void changePlayer();

    
        //test function
        void printBoardState() const;
        void printDataArray(const std::array<int16_t,MAX_BOARD_AREA>& array) const;
        void printStats() const;


        //Depreciated
        // std::vector<Intersection> getBoardState() const;
        // void setBoardState(const std::vector<Intersection>& newBoardState);
        // Stone getStone(Coordinate coordinates) const;
        // void setPreviousBoardState(std::vector<Intersection> boardState);
        // std::vector<Intersection> getPreviousBoardState() const;
        // void setStone(Coordinate coordinates, Stone color);
        // Group floodFill(Coordinate startCoordinates, Stone groupColor) const;
    };
}

