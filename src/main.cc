#include <SFML/Graphics.hpp>
#include <iostream>

#include <Controllers/GameController.h>

/**
 * funkcja: 
 *      main.cc
 * rola: 
 *      entry-point dla aplikacji, 
 *      wczytywanie parametrow
 */

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    //todo, load from dedicated ui menu
    int boardSize = 9; //controls board size, legal inputs: 9, 13, 19
    float cellSize = 40.f;
    float margin = 25.f;

    sf::RenderWindow window(sf::VideoMode((boardSize - 1) * cellSize + 2 * margin,(boardSize - 1) * cellSize + 2 * margin), "Go Prot", sf::Style::Close, settings);

    GameController gameController(cellSize, margin, boardSize, window);
    gameController.run();


    return 0;
}