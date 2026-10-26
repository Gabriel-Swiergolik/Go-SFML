#include <SFML/Graphics.hpp>
#include <iostream>

#include <Controllers/GameController.h>
#include "Common.h"

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

    sf::Clock gameTime;
    sf::Clock eventTime;
    bool showText = false;

    //todo, load from dedicated ui menu
    int boardSize = 19; //controls board size, legal inputs: 9, 13, 19
    float cellSize = 40.f;
    float margin = 25.f;

    sf::RenderWindow window(sf::VideoMode((boardSize - 1) * cellSize + 2 * margin,(boardSize - 1) * cellSize + 2 * margin), "Go Prot", sf::Style::Close, settings);
    window.setFramerateLimit(30);

    Go::GameController gameController(cellSize, margin, boardSize, window);
    //gameController.run(window);

    sf::Font font;
    sf::Text text;
    if(!font.loadFromFile("resources/fonts/firaSans.ttf"))
    {
        std::cerr << "Error loading font\n";
        return -1;
    }
    else
    {
        text.setFont(font); // Apply the loaded font
        text.setString("Hello, SFML!"); // Set the string to display
        text.setCharacterSize(24); // Set size in pixels
        text.setFillColor(sf::Color::White); // Set text color
        text.setPosition(50.f, 50.f); // Set position on screen
    }
    
    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            //todo: 
            //switch case?
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {

                //todo:
                //zmienic pipline pixel -> coordinate -> index
                //obecna forma jest mylaca
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                Go::Coordinate coordinates = gameController.getGameGemetry().pixelsToCoordinates(mousePosition.x, mousePosition.y);

                int index = gameController.getBoard().convertCoordinatesToIndex(coordinates);
                gameController.getBoard().placeStone(index);
                gameController.getBoard().printBoardState(); //DEBUG
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                eventTime.restart();
                showText = true;
            }
            if (showText && eventTime.getElapsedTime().asSeconds() >= 3.0f) 
            {
                showText = false; // Hide the text
            }
        }
        window.clear();
        gameController.getBoardPainter().drawBoard();
        gameController.getBoardPainter().drawStoneHighlight(gameController.getBoard().getCurrentPlayer());
        if(showText)
            window.draw(text);
        window.display();
    }

    return 0;
}