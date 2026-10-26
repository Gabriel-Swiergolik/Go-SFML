cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Go_SFML LANGUAGES CXX)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main)

target_sources(main
    PRIVATE
        src/main.cc
)

target_compile_features(main 
    PRIVATE
        cxx_std_17
)

#Defining subfiles
add_library(Headers)

target_sources(Headers
    PRIVATE
        src/Graphic/BoardPainter.cc
        src/Graphic/GameGeometry.cc
        src/Logic/Core/Board.cc
        src/Controllers/GameController.cc
        src/Logic/Analysis/MonteCarloSimulation.cc
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/Graphic/BoardPainter.h
            include/Graphic/GameGeometry.h
            include/Logic/Core/Board.h
            include/Logic/Core/LibertyBitset.h
            include/Logic/Core/SplitMix.h
            include/Logic/Core/ZobristHash.h
            include/Controllers/GameController.h
            include/Logic/Analysis/MonteCarloSimulation.h
)

target_link_libraries(Headers
    PUBLIC
        sfml-graphics
)

target_link_libraries(main 
    PRIVATE
        sfml-graphics
        Headers
)
add_compile_options(-D_GLIBCXX_ASSERTIONS)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)



