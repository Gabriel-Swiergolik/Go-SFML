# Go-SFML

A digital implementation of the classic board game **Go** (also known as Weiqi or Baduk), developed in C++ using the [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library) for graphics and CMake for the build system. 
##  About the Project

**Go-SFML** is mostly a learning project, it aims to provide a clean, lightweight, and interactive graphical interface to play the ancient game of Go. It is designed to be a solid foundation for local or multiplayer gameplay.

##  Current Features

* **Classic Go Mechanics**: Play the traditional game of Go with standard stone placement.
* **Graphical Interface**: 2D rendering of the board, grid, and stones using SFML.
* **C++ & CMake Architecture**: Cross-platform-ready codebase using modern C++ and CMake. 

##  Planned Features

Project is far from finished and several major updates are currently on the roadmap. Upcoming features include:

* **Configuration Menu**: A pre-game setup screen that will allow players to choose between local and multiplayer modes, select custom board sizes (e.g., 9x9, 13x13, 19x19), and adjust other match preferences.
* **Pass System & Point Counting**: Implementation of the standard Go passing mechanic. Once both players choose to pass their turn, the game will automatically calculate the score based on territory and captured stones to declare the winner.
* **Basic Multiplayer (TCP Protocol)**: Networked peer-to-peer gameplay. Players will be able to host a game or connect to an opponent's IP address over the internet/LAN using TCP sockets.
* **Multiplayer Chat**: An integrated in-game chat box allowing players to communicate with each other in real time during multiplayer network matches.

##  Getting Started

### Prerequisites

To build and run this project locally, you will need the following installed on your system:
* A C++ compiler (supporting C++17 or later)
* [CMake](https://cmake.org/download/) (v3.10 or higher)
* [SFML](https://www.sfml-dev.org/download.php) (v2.6 or higher)

### Building the Project (ubuntu)

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Gabriel-Swiergolik/Go-SFML.git

2. **Install necessary dependencies:**
    ```bash
    sudo apt install libx11-dev libxcursor-dev libxrandr-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libudev-dev libopenal-dev

3. **Build the application:**
    ```bash
    cmake -B yourBuildName; cmake --build yourBuildName

4. **Launch the game:**
    ```bash
    ./yourBuildName/bin/main

