#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    if(fileName =="")
    {
        //cout<<fileName<<endl;
        return nullptr;
    }
    ifstream file(fileName);
    //go through all test issues and conditions
    if(!file.is_open())
    {
        //cout<<"+"<<fileName<<endl;
        return nullptr;
    }
    
    //check if variables are a digit
    file >> maxRow ;
    if(file.fail()|| file.bad())//|| !isdigit(file.peek()))
    {
        //cout<<"wrong data"<<endl;
        return nullptr;
    }
    file >>maxCol;
    if(file.fail()|| file.bad())//||  !isdigit(file.peek()))
    {
        //cout<<"wrong data"<<endl;
        return nullptr;
    }
    file >> player.row ;
    if(file.fail()|| file.bad())//||  !isdigit(file.peek()))
    {
        //cout<<"wrong data"<<endl;
        return nullptr;
    }
    file>>player.col;
    if(file.fail()||  file.bad())//|| !isdigit(file.peek()))
    {
        //cout<<"wrong data"<<endl;
        return nullptr;
    }

    //int tileCount = 0;
    if(static_cast<double>(INT32_MAX)/maxRow < maxCol || static_cast<double>(INT32_MAX)/maxCol < maxRow)
    {
        return nullptr;
    }
    //int tileMax = maxRow*maxCol;

    if(maxRow<=0 || maxCol<=0|| player.row<0 || player.col<0 || player.row>=maxRow || player.col>=maxCol)
    {
        return nullptr;
    }
    char** map = createMap(maxRow,maxCol);
    if(map==nullptr)
        return nullptr;
    for(int i = 0; i<maxRow;i++)
    {
        for(int j = 0;j<maxCol;j++)
        {
            //check that there is a way out
            file >> map[i][j];
            if(file.fail()|| file.bad())//|| file.eof())
            {
                //cout<<"too little tiles"<<endl;
                deleteMap(map,maxRow);
                return nullptr;
            }
            //tileCount++;
            if(i==player.row && j == player.col)
                map[i][j] = TILE_PLAYER;
            //check that there are only valid tiles
            if(map[i][j]!=TILE_AMULET && map[i][j]!=TILE_DOOR && map[i][j]!=TILE_EXIT && map[i][j]!=TILE_MONSTER && map[i][j]!=TILE_OPEN && map[i][j]!=TILE_PILLAR && map[i][j]!=TILE_PLAYER && map[i][j]!=TILE_TREASURE)
            {
                deleteMap(map,maxRow);
                return nullptr;
            }
        }
    }

    //wrong number of tiles
    char more = 'b';
    file >> more;
    if(more!='b')
    {
        deleteMap(map,maxRow);
        return nullptr;
    }

    bool exit = false;
    for(int i = 0; i<maxRow;i++)
    {
        for(int j = 0;j<maxCol;j++)
        {
            //check that there is a way out
            if(map[i][j]==TILE_EXIT||map[i][j]==TILE_DOOR)
                exit = true;
        }
    }
    if(!exit)
    {
        cout<<"no exit"<<endl;
        deleteMap(map,maxRow);
        return nullptr;
    }
    
    return map;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if(input==MOVE_DOWN)
    {
        nextRow++;
    }
    if(input==MOVE_LEFT)
    {
        nextCol--;
    }
    if(input==MOVE_RIGHT)
    {
        nextCol++;
    }
    if(input==MOVE_UP)
    {
        nextRow--;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    char** map= new char*[maxRow];
    for(int i = 0;i<maxRow;i++)
        map[i] = new char[maxCol];
    for(int i = 0;i<maxRow;i++)
    {
        for(int j = 0; j<maxCol;j++)
        {
            map[i][j] = TILE_OPEN;
        }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if(map==nullptr || maxRow <=0)
    {
        map = nullptr;
        maxRow = 0;
    }
    else{
        for(int i = 0;i<maxRow;i++)
        {
            delete[] map[i];
        }
        delete[] map;
        map = nullptr;
        maxRow = 0;
    }
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    if(map==nullptr||maxRow<=0||maxCol<=0)
    {
        return nullptr;
    }
    int newMaxRow = maxRow*2, newMaxCol = maxCol*2;

    char** newMap = createMap(newMaxRow,newMaxCol);

    //A
    for(int i = 0;i<maxRow;i++)
    {
        for(int j = 0; j<maxCol;j++)
        {
            newMap[i][j] = map[i][j];
        }
    }
    //B
    int x=0,y=0;
    for(int i = 0;i<maxRow;i++)
    {
        for(int j = maxCol; j<newMaxCol;j++)
        {
            newMap[i][j] = map[i][y];
            if(map[i][y]==TILE_PLAYER)
            {
                newMap[i][j]= TILE_OPEN;
            }
            y++;
        }
        y=0;
        x++;
    }
    //C
    x=0,y=0;
    for(int i = maxRow;i<newMaxRow;i++)
    {
        for(int j = 0; j<maxCol;j++)
        {
            newMap[i][j] = map[x][y];
            if(map[x][y]==TILE_PLAYER)
            {
                newMap[i][j]= TILE_OPEN;
            } 
            y++;       
        }
        y=0;
        x++;
    }
    //D
    x=0,y=0;
    for(int i = maxRow;i<newMaxRow;i++)
    {
        for(int j = maxCol; j<newMaxCol;j++)
        {
            newMap[i][j] = map[x][y];
            if(map[x][y]==TILE_PLAYER)
            {
                newMap[i][j]= TILE_OPEN;
            }
            y++;
        }
        y=0;
        x++;
    }
    
    deleteMap(map,maxRow);
    maxRow = newMaxRow;
    maxCol = newMaxCol;
    return newMap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    if(map==nullptr||maxRow<=0||maxCol<=0||nextRow<0||nextCol<0)
    {
        return STATUS_STAY;
    }
    int playerMove = 0;

    if((nextRow==player.row && nextCol==player.col)||nextRow>=maxRow||nextCol>=maxCol||map[nextRow][nextCol]==TILE_PILLAR||map[nextRow][nextCol]==TILE_MONSTER)
    {
        nextRow = player.row, nextCol = player.col;
        playerMove = STATUS_STAY;
    }
    else if(map[nextRow][nextCol]==TILE_TREASURE)
    {
        playerMove = STATUS_TREASURE;
        player.treasure++;
    }
    else if(map[nextRow][nextCol]==TILE_AMULET)
    {
        playerMove = STATUS_AMULET;
    }
    else if(map[nextRow][nextCol]==TILE_DOOR)
    {
        playerMove = STATUS_LEAVE;
    }
    else if(map[nextRow][nextCol]==TILE_EXIT)
    {
        if(player.treasure>=1)
            playerMove = STATUS_ESCAPE;
        else 
        {
            playerMove = STATUS_STAY;
            nextRow = player.row, nextCol = player.col;
        }
    }
    else
    {
        playerMove = STATUS_MOVE;
    }

    if(playerMove!=0)
    {
        map[player.row][player.col]=TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow, player.col = nextCol;
    }

    return playerMove;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    bool monsterMove = false;
    bool pillarHit = false;
    if(map==nullptr||maxRow<=0||maxCol<=0)
    {
        return false;
    }
    int pRow = player.row, pCol = player.col;
    //above
    if(pRow!=0)
    {
        for(int i = pRow-1;i>=0;i--)
        {
            if(map[i][pCol]==TILE_PILLAR)
            {
                pillarHit = true;
            }
            if(map[i][pCol]==TILE_MONSTER)
            {
                map[i+1][pCol] = TILE_MONSTER;
                map[i][pCol] = TILE_OPEN;
                if(i+1==pRow&&!monsterMove)
                    monsterMove = true;
            }
            if(pillarHit)
                break;
        }
    }
    //below
    pillarHit = false;
    if(pRow!=maxRow-1)
    {
        for(int i = pRow+1;i<maxRow;i++)
        {
            if(map[i][pCol]==TILE_PILLAR)
            {
                pillarHit = true;
            }
            if(map[i][pCol]==TILE_MONSTER)
            {
                map[i-1][pCol] = TILE_MONSTER;
                map[i][pCol] = TILE_OPEN;
                if(i-1==pRow&&!monsterMove)
                    monsterMove = true;
            }
            if(pillarHit)
                break;
        }
    }
    //left
    pillarHit = false;
    if(pCol!=0)
    {
        for(int i = pCol-1;i>=0;i--)
        {
            if(map[pRow][i]==TILE_PILLAR)
            {
                pillarHit = true;
            }
            if(map[pRow][i]==TILE_MONSTER)
            {
                map[pRow][i+1] = TILE_MONSTER;
                map[pRow][i] = TILE_OPEN;
                if(i+1==pCol&&!monsterMove)
                    monsterMove = true;
            }
            if(pillarHit)
                break;
        }
    }
    //right
    pillarHit = false;
    if(pCol!=maxCol-1)
    {
        for(int i = pCol+1;i<maxCol;i++)
        {
            if(map[pRow][i]==TILE_PILLAR)
            {
                pillarHit = true;
            }
            if(map[pRow][i]==TILE_MONSTER)
            {
                map[pRow][i-1] = TILE_MONSTER;
                map[pRow][i] = TILE_OPEN;
                if(i-1==pCol&&!monsterMove)
                    monsterMove = true;
            }
            if(pillarHit)
                break;
        }
    }

    return monsterMove;
}
