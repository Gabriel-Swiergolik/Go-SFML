#ifndef COMMON_H
#define COMMON_H
#include <cstdint>


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

static constexpr Coordinate DIRECTIONS[4] = 
{
    {0,  1}, {0, -1}, {-1, 0}, {1, 0}
};


#endif