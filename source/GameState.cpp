#include "GameState.h"

void GameState::InitState()
{
    mmStart( MOD_TETRIS, MM_PLAY_LOOP );
    memcpy16(pal_bg_mem, GAMEUIPal, GAMEUIPalLen/2);
    memcpy32(&tile_mem[0][0], GAMEUITiles, GAMEUITilesLen/4);
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
    memcpy16(se_mem[31], PLAYAREA, sizeof(PLAYAREA)/2);

    //Lines
    if(GameContext::Instance()->CurrentLinesChanged())
    {
        //rectfill(BUFFER, 186, 8, 210, 16, makecol(57,85,113));
        //textout_ex(BUFFER, GameContext::Instance()->GAME_FONT, formatInteger(3, GameContext::Instance()->GetCurrentLines()).c_str(), 186, 8, makecol(255, 255, 255), -1);
        GameContext::Instance()->SetCurrentLinesChanged(false);
    }

    //Statistics
    for(int i = 0; i < 7; i++)
    {
        if(GameContext::Instance()->TetrominoTallyChanged(i))
        {
            //rectfill(BUFFER, 56, 24 + (i * 24), 56 + (3 * 8), 24 + (i * 24) + 8, makecol(57,85,113));
            //textout_ex(BUFFER, GameContext::Instance()->GAME_FONT, formatInteger(3, GameContext::Instance()->GetTetrominoTally(i)).c_str(), 56, 24 + (i * 24), makecol(255, 255, 255), -1);
            GameContext::Instance()->SetTetrominoTallyChanged(i, false);
        }
    }

    //Current Level   
    if(GameContext::Instance()->CurrentLevelChanged())
    {
        //rectfill(BUFFER, 303, 12, 313, 20, makecol(101,101,101));
        //textout_ex(BUFFER, GameContext::Instance()->GAME_FONT, formatInteger(2, GameContext::Instance()->GetCurrentLevel()).c_str(), 303, 12, makecol(255, 255, 255), -1);
        GameContext::Instance()->SetCurrentLevelChanged(false);
    }

    //Scores
    if(GameContext::Instance()->TopScoreChanged() || GameContext::Instance()->CurrentScoreChanged())
    {
        //rectfill(BUFFER, 262, 118, 310, 126, makecol(101,101,101));
        //textout_ex(BUFFER, GameContext::Instance()->GAME_FONT, formatInteger(6, GameContext::Instance()->GetTopScore()).c_str(), 262, 118, makecol(255, 255, 255), -1); 
        //rectfill(BUFFER, 262, 138, 310, 146, makecol(101,101,101)); 
        //textout_ex(BUFFER, GameContext::Instance()->GAME_FONT, formatInteger(6, GameContext::Instance()->GetCurrentScore()).c_str(), 262, 138, makecol(255, 255, 255), -1); 
        GameContext::Instance()->SetCurrentScoreChanged(false);
        GameContext::Instance()->SetTopScoreChanged(false);
    }

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

}