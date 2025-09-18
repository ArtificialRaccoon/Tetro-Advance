#include "Tetromino.h"

Tetromino::Tetromino(int type) 
{
    this->Type = type;
    this->x = playGridWidth / 2 - 2;
    this->y = 0;
    this->rotation = 0;

    for (int i = 0; i < 4; i++) 
    { 
        this->shape[i] = this->tetrominoes[this->Type][i]; 
    }
}

void Tetromino::Draw(bool preview)
{
    int xPos = x;
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            if (this->shape[rotation] & (0x8000 >> (i * 4 + j)))
            {
                if (preview)
                    DrawBlock(xPos + j, this->y + i, previewGridX, previewGridY);
                else
                    DrawBlock(xPos + j, this->y + i, playGridX, playGridY);
            }
        }
    }
}

void Tetromino::Clear()
{
    int xPos = x;
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            if (this->shape[rotation] & (0x8000 >> (i * 4 + j)))
                ClearBlock(xPos + j, this->y + i, previewGridX, previewGridY);
        }
    }
} 

void Tetromino::DrawBlock(int x, int y, int screenSpaceX, int screenSpaceY) 
{
    int mapIndex = (screenSpaceY + y) * 32 + (screenSpaceX + x);
    u16 tileID = TETROMINO_TILE_START + this->Type;
    se_mem[ACTION_LAYER_ID][mapIndex] = tileID;
}

void Tetromino::ClearBlock(int x, int y, int screenSpaceX, int screenSpaceY) 
{
    int mapIndex = (screenSpaceY + y) * 32 + (screenSpaceX + x);
    u16 tileID = BLANK_TILE;
    se_mem[ACTION_LAYER_ID][mapIndex] = tileID;
}

bool Tetromino::CanMove(int** playGrid, int newX, int newY) 
{
    for (int i = 0; i < 16; i++) 
    {
        if (this->shape[rotation] & (0x8000 >> i)) 
        {
            int localX = i % 4;
            int localY = i / 4;
            int finalX = newX + localX;
            int finalY = newY + localY;

            if (finalX < 0 || finalX >= playGridWidth || finalY < 0 || finalY >= playGridHeight)
                return false;

            if (playGrid[finalY][finalX] > 0)
                return false;
        }
    }
    return true; 
}

bool Tetromino::CanRotate(int** playGrid, int nextRotation) 
{
    for (int i = 0; i < 16; i++) 
    {
        if (this->shape[nextRotation] & (0x8000 >> i)) 
        {
            int localX = i % 4;
            int localY = i / 4;
            int finalX = this->x + localX;
            int finalY = this->y + localY;

            if (finalX < 0 || finalX >= playGridWidth || finalY < 0 || finalY >= playGridHeight)
                return false;

            if (playGrid[finalY][finalX] > 0)
                return false;
        }
    }
    return true;
}

void Tetromino::LockPiece(int** playGrid, bool** changedGrid)
{
    for (int i = 0; i < 16; i++)
    {
        if (this->shape[rotation] & (0x8000 >> i)) 
        {
            int localX = i % 4;
            int localY = i / 4;
            int finalX = this->x + localX;
            int finalY = this->y + localY;

            if (finalX >= 0 && finalX < playGridWidth && finalY >= 0 && finalY < playGridHeight)
                playGrid[finalY][finalX] = this->Type + 1;
        }
    }

    BlankoutPiece(changedGrid);
    lockTimer = 0;
}

void Tetromino::BlankoutPiece(bool** changedGrid)
{
    for (int i = 0; i < 16; i++)
    {
        if (this->shape[rotation] & (0x8000 >> i)) 
        {
            int localX = i % 4;
            int localY = i / 4;
            int finalX = this->x + localX;
            int finalY = this->y + localY;

            if (finalX >= 0 && finalX < playGridWidth && finalY >= 0 && finalY < playGridHeight)
                changedGrid[finalY][finalX] = true;            
        }
    }
}

bool Tetromino::CanSpawn(int** playGrid)
{
    for (int i = 0; i < 16; i++)
    {
        if (this->shape[rotation] & (0x8000 >> i)) 
        {
            int localX = i % 4;
            int localY = i / 4;
            int finalX = this->x + localX;
            int finalY = this->y + localY;

            if (playGrid[finalY][finalX] > 0) 
                return false;
        }
    }
    return true;
}

bool Tetromino::CanLock(int** playGrid) 
{
    if (!CanMove(playGrid, this->x, this->y + 1))
    {
        lockTimer++;
        if (lockTimer >= lockTimerMax) 
            return true;
    } 
    else 
        lockTimer = 0;
    return false;
}