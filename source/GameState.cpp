#include "GameState.h"

void GameState::InitState()
{
    mmStart( MOD_TETRIS, MM_PLAY_LOOP );
    memcpy16(pal_bg_mem, GAMEUIPal, GAMEUIPalLen/2);
    memcpy32(&tile_mem[0][0], GAMEUITiles, GAMEUITilesLen/4);
    tte_init_chr4c(1, BG_CBB(2) | BG_SBB(TEXT_LAYER_ID), 0xF000, 0x1E, (u32)&sys8Font, NULL, NULL); //Need to change tghe 0x1E to the right colour index...
    tte_erase_screen();
}

void GameState::Pause()
{

}

void GameState::Resume()
{

}

void GameState::AquireInput(GameProcessor* game)
{
    u16 keys_pressed = ~REG_KEYINPUT & KEY_MASK;
    if (keys_pressed & KEY_UP)
        GameContext::Instance()->GetCurrentPiece().Rotate(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());        
    else if (keys_pressed & KEY_DOWN)
        GameContext::Instance()->GetCurrentPiece().Down(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());
    else if (keys_pressed & KEY_LEFT)    
        GameContext::Instance()->GetCurrentPiece().Left(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());
    else if (keys_pressed & KEY_RIGHT)
        GameContext::Instance()->GetCurrentPiece().Right(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());
    else if ((keys_pressed & KEY_A) || (keys_pressed & KEY_B))
        GameContext::Instance()->GetCurrentPiece().HardDrop(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());   
}

void GameState::ProcessInput(GameProcessor* game)
{ 
    if (GameContext::Instance()->ShouldDrop())
        GameContext::Instance()->GetCurrentPiece().Down(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());

    if (GameContext::Instance()->GetCurrentPiece().CanLock(GameContext::Instance()->PlayGrid())) 
    {
        GameContext::Instance()->IncreaseTetrominoTally(GameContext::Instance()->GetCurrentPiece().GetType());
        GameContext::Instance()->GetCurrentPiece().LockPiece(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());
        GameContext::Instance()->CheckForCompletedLines();
        
        if (GameContext::Instance()->CanSpawn())
            GameContext::Instance()->SpawnTetromino();
        else
        {
            mmStop();
            mmEffectEx(&(GameContext::Instance()->GAMEOVER));

            // Show the player HOW they lost
            GameContext::Instance()->SpawnTetromino();
            Render(game);
            this->UnloadResources();
            game->ChangeState(GameOverState::Instance());
        }
    }
}

void GameState::Render(GameProcessor* game)
{
    tte_erase_screen();
    memcpy16(se_mem[BACKGROUND_LAYER_ID], PLAYAREA, sizeof(PLAYAREA)/2);

    //Lines
    PrintText(std::string("Lines: ") + formatInteger(3, GameContext::Instance()->GetCurrentLines()), 168, 8);

    //Current Level
    PrintText(std::string("Level: ") + formatInteger(2, GameContext::Instance()->GetCurrentLevel()), 168, 32);  

    //Score
    PrintText(std::string("Score: ") + formatInteger(6, GameContext::Instance()->GetCurrentScore()), 168, 56);

    //Draw Next Piece
    if(GameContext::Instance()->NextPieceChanged())
    {
        //clear_to_color(NEXTWINDOW, makecol(57,85,113));
        //GameContext::Instance()->GetNextPiece().Draw(NEXTWINDOW, GameContext::Instance()->GAMEUI, true);
        //blit(NEXTWINDOW, BUFFER, 0, 0, 272, 48, blockSize * 4, blockSize * 4);
        GameContext::Instance()->SetNextPieceChanged(false);
    }

    //Draw Play Grid
    for (int y = 0; y < playGridHeight; y++) 
    {
        for (int x = 0; x < playGridWidth; x++) 
        {
            if (GameContext::Instance()->ChangedGrid()[y][x] == true)
            {
                //if (GameContext::Instance()->PlayGrid()[y][x] > 0)
                    //blit(GameContext::Instance()->GAMEUI, PLAYGRID, 72 + ((GameContext::Instance()->PlayGrid()[y][x] - 1) * 8), 56, x * blockSize, y * blockSize, blockSize, blockSize);
                //else
                    //blit(GameContext::Instance()->GAMEUI, PLAYGRID, 120, 0, x * blockSize, y * blockSize, blockSize, blockSize);
                GameContext::Instance()->ChangedGrid()[y][x] = false;
            }
        }
    }    

    //Draw Current Piece
    //GameContext::Instance()->GetCurrentPiece().Draw(PLAYGRID, GameContext::Instance()->GAMEUI, 0);
}

void GameState::DrawBackground()
{

}

std::string GameState::formatInteger(int leadingZeros, int value)
{
    std::string returnValue = std::to_string(value);
    int neededPadding = leadingZeros - returnValue.size();
    if (neededPadding > 0)
        returnValue.insert(0, neededPadding, '0');
    return returnValue;
}

void GameState::Reset()
{
    GameContext::Instance()->Reset();
}

void GameState::UnloadResources()
{
    mmStop();
    tte_erase_screen();
}

void GameState::PrintText(std::string text, int x, int y)
{    
    char cursor[16];
    snprintf(cursor, sizeof(cursor), "#{P:%d,%d}", x, y);
    tte_write(cursor);
    tte_write(text.c_str());
}