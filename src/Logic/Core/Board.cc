#include "Logic/Core/Board.h"
#include "Logic/Core/ZobristHash.h"


namespace Go 
{
    /**
     * petla okresla granice planszy i wyznaczy jej sciany
     * sciany to niewidoczne dla gracza pozycje znajdujaca sie na skraju planszy,
     * potrzebna do obliczen stanu planszy
     * 
     * przykladowy output w postaci graficznej dla planszy 9x9:
     *   * * * * * * * * * * * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * - - - - - - - - - * 
     *   * * * * * * * * * * *
     */
    Board::Board(const int playSize)
    {
        //improvised bound-check, to be refactored
        if(!(playSize == 9) && !(playSize == 13) && !(playSize == 19))
        {
            std::cerr << "Invalid playSize" << '\n';
            return;
        }
    
        //black always starts first
        m_currentPlayerColor = Stone::Black;
        m_playSize = playSize;
        m_boardSize = playSize + 2;

        m_currentHash = 0;
        m_previousHash = 0;
        
        for(size_t i = 0; i < m_boardSize*m_boardSize; i++)
        {
            int rowIndex = i / m_boardSize;
            int columnIndex = i - ((m_boardSize) * rowIndex);
    
            if(rowIndex == 0  || columnIndex == 0 || rowIndex == m_boardSize-1 || columnIndex == m_boardSize-1)
                m_board[i] = Intersection{Stone::Wall};
            else
                m_board[i] = Intersection{Stone::None};
        }
    }
    
    int Board::getBoardSize() const
    {
        return m_boardSize;
    }

    const std::array<Intersection, MAX_BOARD_AREA>& Board::getBoard() const
    {
        return m_board;
    }

    Stone Board::getCurrentPlayer() const
    {
        return m_currentPlayerColor;
    }

    Stone Board::getStone(int index) const
    {
        return m_board[index].stone;
    }

    void Board::setStone(int index, Stone color)
    {
        m_board[index].stone = color;
    }
    
    int Board::convertCoordinatesToIndex(Coordinate coordinates) const
    {
        return (m_boardSize * (coordinates.y + 1)) + (coordinates.x + 1);         
    }
    
    //is it even used?
    Coordinate Board::convertIndexToCoordinates(int index) const
    {
        int y = index / m_boardSize;
        int x = index - ((m_boardSize) * y);
    
        return {x-1, y-1};
    }
    
    bool Board::isEmptyAt(int index) const
    {
        return m_board[index].isEmpty();
    }
    
    //checks if move is within bounds
    bool Board::isLegal(int index) const
    {
        int rowIndex = index / m_boardSize;
        int columnIndex = index - ((m_boardSize) * rowIndex);

        return rowIndex > 0 && columnIndex > 0 && rowIndex < m_boardSize-1 && columnIndex < m_boardSize-1;
    }
    
    //checks if move is within bounds (cord version)
    bool Board::isLegal(Coordinate coordinates) const
    {
        return coordinates.x >= 0 && coordinates.y >= 0 && coordinates.x < m_boardSize  && coordinates.y < m_boardSize ;
    }
    
    //always gets valid index, so no check
    bool Board::tryPlacingStone(const int sourceIndex)
    {
        if(!isEmptyAt(sourceIndex))
            return false;

        const std::array<int,4> dirs = { 
            sourceIndex-m_boardSize, 
            sourceIndex+m_boardSize, 
            sourceIndex+1, 
            sourceIndex-1
        };

        //counts used later for validation
        //each scanned neighbour is stored into one of these arrays
        std::array<int,4> emptyNeighbours;      int emptyCount = 0;
        std::array<int,4> friendlyRoots;        int friendlyCount = 0;
        std::array<int,4> survivingEnemyRoots;  int survivingCount = 0;
        std::array<int,4> capturedEnemyRoots;    int capturedCount = 0;
        int totalCapturedStones = 0;
        //a friendly chain that has more then 1 liberty
        bool hasFriendWithSpareLiberty = false;
        Stone enemyStoneColor = m_currentPlayerColor == Stone::White ? Stone::Black : Stone::White;

        for(const int neighbourIndex : dirs)
        {
            //EVALUATION

            const Stone neighbourStone = m_board[neighbourIndex].stone;
            if(neighbourStone == Stone::Wall)
                continue;

            if(neighbourStone == Stone::None)
            {
                emptyNeighbours[emptyCount++] = neighbourIndex;
            }
            //neighbour is a friend
            else if(neighbourStone == m_currentPlayerColor)
            {
                unsigned root = findRoot(neighbourIndex);

                //validation to avoid adding one chain twice 
                bool visited = false;
                for(int i = 0; i < friendlyCount; i++)
                    if(friendlyRoots[i] == root) 
                    {
                        visited = true;
                        break;
                    }

                //mark neighbour as a part of a friendly group
                //only if he's not in group with other neighbours
                if(!visited)
                {
                    friendlyRoots[friendlyCount++] = root;
                    if(m_libertyArray[root].libertyCount() > 1)
                        hasFriendWithSpareLiberty = true;
                }
            }
            //neighbour is an enemy
            else 
            {
                unsigned root = findRoot(neighbourIndex);

                //same logic as with friendly groups
                //check if placing a stone will take out a group...
                if(m_libertyArray[root].libertyCount() == 1)
                {
                    bool visited = false;
                    for(int i = 0; i < capturedCount; i++)
                        if(capturedEnemyRoots[i] == root)
                        {
                            visited = true;
                            break;
                        }
                    
                    if(!visited){
                        capturedEnemyRoots[capturedCount++] = root;
                        totalCapturedStones += m_sizeArray[root];
                    }
                }
                //if not then add it to the corresponding array
                else
                {
                    bool visited = false;
                    for(int i = 0; i < survivingCount; i++)
                        if(survivingEnemyRoots[i] == root)
                        {
                            visited = true;
                            break;
                        }
                    if(!visited)
                        survivingEnemyRoots[survivingCount++] = root;
                }
            }
        }

        //VALIDATION
        //suicide check
        if(emptyCount == 0 && capturedCount == 0 && !hasFriendWithSpareLiberty)
            return false;

        //simple ko check
        if(emptyCount == 0 && totalCapturedStones == 1 && friendlyCount == 0)
        {
            unsigned singleCapturedStoneIndex = capturedEnemyRoots[0];

            //we make a hypothetical move with hash
            //in practice we remove enemy stone and add ours to the hash
            uint64_t hypotheticalHash = (m_currentHash 
                ^ zobristHashTable[sourceIndex][static_cast<int>(m_currentPlayerColor)] 
                ^ zobristHashTable[singleCapturedStoneIndex][static_cast<int>(enemyStoneColor)]);

            //if our newly calculated hash i equal to the previous hash
            //we've got ourselfs a ko -> return false
            if(hypotheticalHash == m_previousHash)
                return false;
        }

        //MOVE COMMIT

        //update hashes first
        m_previousHash = m_currentHash;
        m_currentHash ^= zobristHashTable[sourceIndex][static_cast<int>(m_currentPlayerColor)];

        //update logic arrays to note placing a stone
        m_board[sourceIndex].stone = m_currentPlayerColor;
        m_rootArray[sourceIndex] = sourceIndex;
        m_circularRingArray[sourceIndex] = sourceIndex;
        m_sizeArray[sourceIndex]++;

        unsigned sourceRoot = sourceIndex;
        //add a liberty to the root for each empty field
        for(size_t i = 0; i < emptyCount; i++)
            m_libertyArray[sourceRoot].setLiberty(emptyNeighbours[i]);

        //merge with each independent friendly group
        for(size_t i = 0; i < friendlyCount; i++)
        {
            unsigned friendRoot = friendlyRoots[i];
            m_libertyArray[friendRoot].clearLiberty(sourceIndex);
            //unionChains returns winning root
            sourceRoot = unionChains(sourceRoot, friendRoot);
        }

        //clear current index from surving enemies liberties
        for(size_t i = 0; i < survivingCount; i++)
            m_libertyArray[survivingEnemyRoots[i]].clearLiberty(sourceIndex);

        //capture logic
        for(size_t i = 0; i < capturedCount; i++)
        {
            unsigned deadRoot = capturedEnemyRoots[i];

            m_sizeArray[deadRoot] = 0;
            m_libertyArray[deadRoot].clearLiberty(sourceIndex);

            unsigned currentIndex = deadRoot;
            do{
                unsigned nextIndex = m_circularRingArray[currentIndex];

                //removing stone from the hash
                m_currentHash ^= zobristHashTable[currentIndex][static_cast<int>(enemyStoneColor)];

                m_board[currentIndex].stone = Stone::None;
                m_rootArray[currentIndex] = 0;
                m_circularRingArray[currentIndex] = 0;

                const std::array<unsigned,4> capturedStoneNeighbours = { 
                    currentIndex-m_boardSize, 
                    currentIndex+m_boardSize, 
                    currentIndex+1, 
                    currentIndex-1
                };

                for(const int neighbour : capturedStoneNeighbours)
                    if(m_board[neighbour].stone == m_currentPlayerColor)
                        m_libertyArray[findRoot(neighbour)].setLiberty(currentIndex);
                
                currentIndex = nextIndex;
                //stop after making a full circle
            } while(currentIndex != deadRoot);
        }
        changePlayer();
        return true;

        

        // unsigned sourceRoot = findRoot(sourceIndex);

        // for(const int neigbourIndex : dirs){
        //     if(!isLegal(neigbourIndex))
        //         continue;
        //     const Stone neigbour = m_board[neigbourIndex].stone;

        //     if(neigbour == Stone::Wall)
        //         continue;

        //     //if the field is empty, add a liberty to the root
        //     if(neigbour == Stone::None)
        //     {
        //         m_libertyArray[sourceRoot].setLiberty(neigbourIndex);
        //     } 
        //     //check if neigbour is on our team
        //     else if(neigbour == m_currentPlayerColor) 
        //     {
        //         unsigned neigbourRoot = findRoot(neigbourIndex);

        //         //if we've found a friend, the new stone takes one liberty his root(group)...
        //         m_libertyArray[neigbourRoot].clearLiberty(sourceIndex);
        //         if(sourceRoot == neigbourRoot)
        //             continue;
                
        //         //and merge the rest, if they do not share a root
        //         unionChains(sourceRoot, neigbourRoot);
        //         //we set our source to the new root, so we add liberties to the correct stone
        //         sourceRoot = findRoot(sourceRoot);
        //     } 
        //     //neighbour must be an enemy
        //     else 
        //     {
        //         unsigned neigbourRoot = findRoot(neigbourIndex);
        //         //remove enemy's liberty at sourceIndex
        //         m_libertyArray[neigbourRoot].clearLiberty(sourceIndex);

        //         if(m_libertyArray[neigbourRoot].libertyCount() == 0)
        //         {
        //             //clear circularArray
        //             unsigned currentIndex = neigbourRoot;
        //             unsigned nextIndex = 0;

        //             /**
        //              * point logic to be implemented
        //              */
        //             int points_placeholder = 0;
        //             points_placeholder += m_sizeArray[neigbourRoot];

        //             //remove size
        //             m_sizeArray[neigbourRoot] = 0;
                    
        //             do{
        //                 //remove stone from the board
        //                 m_board[currentIndex].stone = Stone::None;
        //                 //clear a stone by setting its root to -1
        //                 m_rootArray[currentIndex] = -1;

        //                 //setup next next element to be processed
        //                 nextIndex = m_circularRingArray[currentIndex];

        //                 //clear element from ring array
        //                 m_circularRingArray[currentIndex] = 0;

        //                 currentIndex = nextIndex;
        //             } while(m_circularRingArray[currentIndex] != 0);
        //             continue;
        //         }
        //         //if there's no capture remove sourceRoot's liberty at neigbour index
        //         m_libertyArray[sourceRoot].clearLiberty(neigbourIndex);
        //     }
        // }
    }

    //passed index will never be wall, so no check
    void Board::placeStone(int index)
    {
        if(!tryPlacingStone(index))
            std::cerr << "error while placing a stone at index: " << index << '\n';
    }
    
    //UnionFind helper methods
    /**
     * I am going to use path-halving instead of recursive or full-flatening
     * - recursive version with TCO would end up using at least 2 whiles anyway
     * - full flattening needs 2 while loops
     * - path-halving works as full-flatening for trees with depth <= 3,
     * so basically almost all cases in go -> mega big win
     */
    int Board::findRoot(int child)
    {
        // this wastes CPU cycles, we should bypass it during neighbour scanning after placing the rock
        // if(child == 0 || m_rootArray[child] == 0 )
        // {
        //     std::cerr << "AHTUNG -> negative number ind findRoot (how?)" << '\n';
        //     return -1;
        // }

        //path halving
        while(m_rootArray[child] != child){
            m_rootArray[child] = m_rootArray[m_rootArray[child]]; //replace parent with grandparent
            child = m_rootArray[child]; //replace child with its parent-turned-granparent
        }
        return child;
    }


    //union by size
    //parentA is always < then parentB in later logic thanks to the swap
    int Board::unionChains(int parentA, int parentB)
    { 
        //'swap' instead of 'else' allows me to save clock cycles 
        //probably meaningless but makes me feel cool
        //for parents that is
        if(m_sizeArray[parentA] > m_sizeArray[parentB]){
            std::swap(parentA,parentB);
        } 
        m_rootArray[parentA] = parentB;

        //path compression optimalization
        int currentIndex = m_circularRingArray[parentA];
        do{
            m_rootArray[currentIndex] = parentB;
            currentIndex = m_circularRingArray[currentIndex];
        } while(currentIndex != parentA);

        //liberty merge
        m_libertyArray[parentB].mergeWith(m_libertyArray[parentA]);

        //swapping the roots connects two separate chains
        std::swap(m_circularRingArray[parentA], m_circularRingArray[parentB]);
        m_sizeArray[parentB] += m_sizeArray[parentA];
        m_sizeArray[parentA] = 0;    

        return parentB;
    }


    void Board::changePlayer()
    {
        m_currentPlayerColor = (m_currentPlayerColor == Stone::Black) ? Stone::White : Stone::Black;
    }
    









    
    /**
     * todo
     *      wrzucic gdzies indziej niz board.cc
     */
    void Board::printBoardState() const
    {
        int spaceCounter = 0;
        int indexCntr = 0;
        for(const auto& i : m_board)
        {
            if(indexCntr > m_boardSize*m_boardSize-1)
                break;
            if (i.isWall())
                std::cout << "*" << " ";
            else if (i.isEmpty())
                std::cout << "-" << " ";
            else if (i.stone == Stone::Black)
                std::cout << "B" << " ";
            else
                std::cout << "W" << " ";

            spaceCounter = (spaceCounter + 1) % (m_boardSize);
            if (spaceCounter == 0)
                std::cout << std::endl;
            indexCntr++;
        }
        std::cout << std::endl;
        printStats();
    }

    void Board::printDataArray(const std::array<int16_t,MAX_BOARD_AREA>& array) const
    {
        int cntr = 0;
        for(const int i : array)
        {
            if(cntr == 21){
                cntr = 0;
                std::cout << '\n';
            }
            cntr++;
            std::cout << i << " ";
        }
        std::cout << '\n';
    }

    void Board::printStats() const
    {
        std::cout << "parent array: " << '\n';
        printDataArray(m_rootArray);
        std::cout << "size array: " << '\n';
        printDataArray(m_sizeArray);
        std::cout << "ring array: " << '\n';
        printDataArray(m_circularRingArray);
}



    //Depreciated
    // std::vector<Intersection> Board::getBoardState() const
    // {
    //     return m_boardState;
    // }
    //Logic will never be handled by cords (I hope)
    // Stone Board::getStone(Coordinate coordinates) const
    // {
    //     int index = this->convertCoordinatesToIndex(coordinates);
    //     return m_board[index].stone;
    // }
    
    /**
     * Floodfill
     * will be used later, during initial points calculations
     */
    // Group Board::floodFill(Coordinate startCoordinates, Stone groupColor) const
    // {
    //     //nie potrzebne
    //     //int boardSize = this->getBoardSize();
    
    //     /**
    //      * todo:
    //      *      zastapic liberties int-em? 
    //      *      done
    //      */
    //     std::vector<bool> visited(m_boardSize*m_boardSize, false);
    //     std::vector<Coordinate> group;
    //     //std::set<Coordinate> liberties;
    //     int libertiesCount = 0;
    //     std::queue<Coordinate> queue;
    
    //     int startIndex = convertCoordinatesToIndex(startCoordinates);
    //     visited[startIndex] = true;
    //     queue.push(startCoordinates);
    //     group.push_back(startCoordinates);
        
    //     while(!queue.empty())
    //     {
    //         Coordinate currentStoneCoordinates = queue.front();
    //         queue.pop();
    
    
    //         for(const auto& direction : DIRECTIONS)
    //         {
    //             Coordinate neighbourCoordinates = {currentStoneCoordinates.x + direction.x, currentStoneCoordinates.y + direction.y};
    
    //             //jezeli sciana, pomin
    //             if(!isLegal(neighbourCoordinates))
    //                 continue;
    
    //             Stone neighbourColor = getStone(neighbourCoordinates);
    //             int index = convertCoordinatesToIndex(neighbourCoordinates);
    
    //             if(neighbourColor == groupColor && !visited[index])
    //             {
    //                 visited[index] = true;
    //                 group.push_back(neighbourCoordinates);
    //                 queue.push(neighbourCoordinates);
    //             }
    //             else if(neighbourColor == Stone::None)
    //             {
    //                 //liberties.insert(neighbourCoordinates);
    //                 libertiesCount++;
    //             }
    //         }
    //     }
    //     //return {(int)liberties.size(), stones};
    //     return {libertiesCount, group};
    // }
    //Logic will never be handled by cords (I hope)
    // void Board::setStone(Coordinate coordinates, Stone color)
    // {
    //     int index = this->convertCoordinatesToIndex(coordinates);
    //     m_board[index].stone = color;
    // }
    
    // void Board::setPreviousBoardState(std::vector<Intersection> boardState)
    // {
    //     m_previousBoardState = boardState;
    // }
    
    // std::vector<Intersection> Board::getPreviousBoardState() const
    // {
    //     return m_previousBoardState;
    // }
    // Depreciated
    // void Board::setBoardState(const std::vector<Intersection>& newBoardState)
    // {
    //     m_boardState = newBoardState;
    // }
}
