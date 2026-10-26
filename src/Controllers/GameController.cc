#include "Controllers/GameController.h"

// GameController::GameController(float cellSize, float margin, int boardSize, sf::ContextSettings windowSettings) :
//     m_window(sf::VideoMode((boardSize - 1) * cellSize + 2 * margin,(boardSize - 1) * cellSize + 2 * margin), "Go Prot", sf::Style::Close, windowSettings),
//     m_board(boardSize),
//     m_gameLogic(m_board),
//     m_gameGeometry(cellSize, margin, boardSize),
//     m_boardPainter(m_window, m_board, m_gameGeometry)
// {
//     m_window.setFramerateLimit(60);
// }

/**
 * Plik: 
 *      GameController.cc
 * Rola:
 *      inijcalizacja i wywolanie obiektow aplikacji,
 *      kontrola przeplywu aplikacji
 */

namespace Go
{
    GameController::GameController(float cellSize, float margin, int boardSize, sf::RenderWindow& window) :
        //window(window),
        m_board(boardSize),
        m_gameGeometry(cellSize, margin, boardSize),
        m_boardPainter(window, m_board, m_gameGeometry)
    {}
    

    Board& GameController::getBoard()
    {
        return m_board;
    }
    const GameGeometry& GameController::getGameGemetry() const
    {
        return m_gameGeometry;
    }
    BoardPainter& GameController::getBoardPainter()
    {
        return m_boardPainter;
    }


    /**
     * Funkcja:
     *      processEvents
     * Rola:
     *      obsluga wydarzen za pomoca funkcjonalnosci SFML
     *      glowna petla rozgrywki
     */
    
    // void GameController::processEvents(sf::RenderWindow& window)
    // {
    //     sf::Font font;
    //     if(!font.loadFromFile("resources/fonts/firaSans.ttf"))
    //     {
    //         std::cerr << "Error loading font\n";
    //         return;
    //     }
    //     sf::Text text;
    //     text.setFont(font); // Apply the loaded font
    //     text.setString("Hello, SFML!"); // Set the string to display
    //     text.setCharacterSize(24); // Set size in pixels
    //     text.setFillColor(sf::Color::White); // Set text color
    //     text.setPosition(50.f, 50.f); // Set position on screen

        
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         //todo: 
    //         //switch case?
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //         else if (event.type == sf::Event::MouseButtonPressed)
    //         {
    
    //             //todo:
    //             //zmienic pipline pixel -> coordinate -> index
    //             //obecna forma jest mylaca
    //             sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    //             Coordinate coordinates = m_gameGeometry.pixelsToCoordinates(mousePosition.x, mousePosition.y);
    
    //             int index = m_board.convertCoordinatesToIndex(coordinates);
    //             m_board.placeStone(index);
    //             m_board.printBoardState(); //DEBUG
    //         }
    //         else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
    //         {
                
    //         }
    //     }
    // }
    
    /**
     * Funkcja: 
     *      render
     * Rola:
     *      kontrola grafiki i jej zmian w ognie
     */
    
    // void GameController::render(sf::RenderWindow& window)
    // {
    //     window.clear();
    //     m_boardPainter.drawBoard();
    //     m_boardPainter.drawStoneHighlight(m_board.getCurrentPlayer());
    //     window.display();
    // }
    
    /**
     * Funkcja: 
     *      run
     * Rola:
     *      inicjalizacja okna
     */
    
    // void GameController::run(sf::RenderWindow& window)
    // {
    //     while(window.isOpen())
    //     {
    //         processEvents(window);
    //         render(window);
    //     }
    // }
}