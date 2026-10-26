#pragma once

#include <cstdint>

namespace Go
{
    inline constexpr int MAX_PLAYABLE_SIZE = 19;
    inline constexpr int MAX_BOARD_SIZE    = MAX_PLAYABLE_SIZE + 2;
    inline constexpr int MAX_BOARD_AREA    = MAX_BOARD_SIZE * MAX_BOARD_SIZE;

    //optymalizacja do monte-carlo, 4 bytes -> 1 byte
    enum class Stone : std::uint8_t {
        White,
        Black,
        None,
        Wall,
    };

    struct Coordinate
    {
        int x;
        int y;

        bool operator<(const Coordinate& other) const 
        {
            if (x != other.x)
                return x < other.x;
            return y < other.y;
        }
    };

    // inline constexpr Coordinate DIRECTIONS[4] = 
    // {
    //     {0,  1}, {0, -1}, {-1, 0}, {1, 0}
    // };
}

