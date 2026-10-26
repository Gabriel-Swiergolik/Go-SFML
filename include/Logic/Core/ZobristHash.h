#pragma once

#include <array>
#include "Common.h"
#include <cstdint>
#include "Logic/Core/SplitMix.h"

namespace Go 
{ 
    using ZobristTable = std::array<std::array<uint64_t, 2>, MAX_BOARD_AREA>;

    /**
     * about indexdes:
     * 0 -> White
     * 1 -> Black
     */
    constexpr ZobristTable initZobristTable()
    {
        ZobristTable zobristHashTable{};
        SplitMix64 rng(123456789ULL);

        for(size_t i = 0; i < MAX_BOARD_AREA; i++)
        {
            zobristHashTable[i][0] = rng.next();
            zobristHashTable[i][1] = rng.next();
        }

        return zobristHashTable;
    }

    inline constexpr ZobristTable zobristHashTable = initZobristTable();
}