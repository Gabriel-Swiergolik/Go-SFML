#ifndef BOARD_H
#define BOARD_H

#include "Common.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <set>
#include <queue>

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
    std::vector<Coordinate> stoneCoordinates;
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
    int m_boardSize;
    //hard-coded, moze potem do modyfikacji z pliku//
    int m_trueBoardSize;
    int m_fieldCount;
    int m_trueFieldCount;
    std::vector<Intersection> m_boardState;
    std::vector<Intersection> m_previousBoardState;

public:
    Board(int boardSize);

    int getBoardSize() const;
    int convertCoordinatesToIndex(Coordinate coordinates) const;
    Coordinate convertIndexToCoordinates(int index) const;

    std::vector<Intersection> getBoardState() const;
    void setBoardState(const std::vector<Intersection>& newBoardState);

    //Maybe move to gameLogic?
    bool isEmptyAt(int index) const;
    bool isLegal(int index) const;
    bool isLegal(Coordinate coordinates) const;

    void setStone(int index, Stone color);
    void setStone(Coordinate coordinates, Stone color);

    void setPreviousBoardState(std::vector<Intersection> boardState);
    std::vector<Intersection> getPreviousBoardState() const;

    Stone getStone(int index) const;
    Stone getStone(Coordinate coordinates) const;

    // void deleteGroup(std::vector<Coordinate>& stones);
    Group floodFill(Coordinate startCoordinates, Stone groupColor) const;

    //test function
    void printBoardState() const;
};


#endif