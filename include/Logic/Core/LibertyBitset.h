#pragma once

#include <bitset>
#include <array>
#include <bit>
#include "Common.h"


namespace Go 
{
    /**
     * Defined here to guarantee its inlined.
     * During monte carlo these function will be called in thousands,
     * so I opted to make then inline, as to avoid the overhead caused
     * by excessive calls
     */
    /**
     * no use of std::bitset, as we need to lookup the position of the last liberty when in atari.
     * We can do it manually by using countr_zero from <bit>, with std::bitset we'd have to use for loop -> much slower
     * this trailing zeros count neatly tells us the exact bit position of the last liberty - very handy!
     * 
     * L -> libertyIndex
     * 
     * Setting liberty:
     * 1. row provided by caller
     * 2. find column -> int col = ceil(L/64)
     * 3. set correct bit:
     *  - create a bit mask at liberty index -> int bitMask = 1 << (L % 64)
     *  - set the bit at libertyArray        -> col | bitMask
     */
    struct LibertyBitset{
        static constexpr size_t bitsPerWord = sizeof(unsigned long) * 8;
        static constexpr size_t bitsetSize = (MAX_BOARD_SIZE * MAX_BOARD_SIZE + bitsPerWord - 1) / bitsPerWord;
        std::array<long unsigned, bitsetSize> bitset{};
    
        void setLiberty(unsigned libertyBitLocation){
            unsigned column = libertyBitLocation / (sizeof(long unsigned) * 8); //floor insteal of ceil i guess
            //if not for 'ul' next to 1 it is interpeted as int and fucks up everything (32 vs 64 bits)
            long unsigned bitMask = 1ul << (libertyBitLocation % (sizeof(long unsigned) * 8));
            bitset[column] |= bitMask;
        }
    
        void clearLiberty(unsigned libertyBitLocation){
            unsigned column = libertyBitLocation / (sizeof(long unsigned) * 8);
            long unsigned bitMask = 1ul << (libertyBitLocation % (sizeof(long unsigned) * 8));
            bitMask = ~bitMask;
            bitset[column] &= bitMask;
        }
    
    
        //argument is getting absorbed and zeroed
        void mergeWith(LibertyBitset& otherLibertyBitset){
            for(size_t i = 0; i < bitset.size(); i++){
                bitset[i] |= otherLibertyBitset.bitset[i];
                otherLibertyBitset.bitset[i] ^= otherLibertyBitset.bitset[i];
            }   
        }
    
        //no discards spits out a warning if returning value is ignored
        [[nodiscard("LibertyCount ignored")]]
        unsigned libertyCount(){ 
            unsigned libertyCount = 0;
            for(const long unsigned bitSection : bitset)
                libertyCount += std::popcount(bitSection);
            return libertyCount;
        }
    
        [[nodiscard("AtariBitLocation igored")]]
        unsigned atariBitLocation(){
            unsigned libertyBitLocation = 0;
            constexpr unsigned wordSize = sizeof(long unsigned) * 8;
    
            for(size_t i = 0; i < bitset.size(); i++){
                if(bitset[i] == 0)
                    libertyBitLocation += wordSize;
                else{
                    libertyBitLocation += std::countr_zero(bitset[i]);
                    break;
                }
            }
    
            return libertyBitLocation;
        }
    
        //debug function, to be yeeted out of here, needs not be inlined
        // void printBits(){
        //     unsigned long filler = 0;
        //     for(const long unsigned word : bitset)
        //     {
        //         std::cout << std::bitset<sizeof(word)*8>(word) << '\n';
        //         for (int i = 0; i < 64; i++) 
        //         {
        //             if (word & (1ul << i)) 
        //             {
        //                 std::cout << i+filler << " ";
        //             }
        //         }
        //         filler+=64;
        //     std::cout << '\n';
        //     }
        // }
    };

}