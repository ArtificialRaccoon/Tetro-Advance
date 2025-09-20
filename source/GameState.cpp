#include "GameState.h"

void GameState::InitState()
{
    mmStart( MOD_TETRIS, MM_PLAY_LOOP );
    memcpy16(pal_bg_mem, GAMEUIPal, GAMEUIPalLen/2);
    memcpy32(&tile_mem[0][0], GAMEUITiles, GAMEUITilesLen/4);
    memcpy16(se_mem[BACKGROUND_LAYER_ID], PLAYAREA, sizeof(PLAYAREA)/2);

    tte_init_chr4c(1, BG_CBB(2) | BG_SBB(TEXT_LAYER_ID), 0xF000, bytes2word(13,15,0,0), CLR_WHITE, &verdana9_b4Font, (fnDrawg)chr4c_drawg_b4cts_fast);
    tte_erase_screen();

    PrintText(std::string("Lines"), 168, 5);
    PrintText(std::string("Level"), 168, 29);
    PrintText(std::string("Score"), 168, 51);
    PrintText(std::string("Top"), 168, 74);
    PrintText(std::string("Next"), 188, 108);
}

void GameState::Pause()
{

}

void GameState::Resume()
{

}

void GameState::AquireInput(GameProcessor* game)
{
    key_poll();
    unsigned short currentlyPressedButtons = key_hit(KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT | KEY_A | KEY_B);
    unsigned short currentlyHeldButtons = key_is_down(KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT | KEY_A | KEY_B);

    if (currentlyPressedButtons)
    {
        HandleInput(currentlyPressedButtons);
        heldButtons = currentlyHeldButtons;
        debounceTimer = initial;
    }
    else if (heldButtons && currentlyHeldButtons == heldButtons)
    {
        if (debounceTimer > 0)
            debounceTimer--;
        else
        {
            HandleInput(heldButtons);
            debounceTimer = repeat;
        }
    }
    else
    {
        heldButtons = 0;
        debounceTimer = 0;
    }
}

void GameState::HandleInput(unsigned short buttons)
{
    if (buttons & KEY_UP)
        GameContext::Instance()->GetCurrentPiece().Rotate(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());
    else if (buttons & KEY_DOWN)
        GameContext::Instance()->GetCurrentPiece().Down(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());
    else if (buttons & KEY_LEFT)
        GameContext::Instance()->GetCurrentPiece().Left(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());
    else if (buttons & KEY_RIGHT)
        GameContext::Instance()->GetCurrentPiece().Right(GameContext::Instance()->PlayGrid(), GameContext::Instance()->ChangedGrid());
    else if ((buttons & KEY_A) || (buttons & KEY_B))
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
    //Lines
    if(GameContext::Instance()->CurrentLinesChanged())
    {
        tte_erase_rect(217, 5, 241, 17);
        PrintText(formatInteger(3, GameContext::Instance()->GetCurrentLines()), 217, 5);
    }

    //Current Level
    if(GameContext::Instance()->CurrentLevelChanged())
    {
        tte_erase_rect(224, 29, 241, 41);
        PrintText(formatInteger(2, GameContext::Instance()->GetCurrentLevel()), 224, 29);
    }

    //Score
    if(GameContext::Instance()->CurrentScoreChanged())
    {
        tte_erase_rect(168, 61, 210, 73);
        PrintText(formatInteger(6, GameContext::Instance()->GetCurrentScore()), 168, 61);
    }

    if(GameContext::Instance()->TopScoreChanged())
    {
        tte_erase_rect(168, 85, 210, 98);
        PrintText(formatInteger(6, GameContext::Instance()->GetTopScore()), 168, 85);
    }

    //Draw Next Piece    
    if(GameContext::Instance()->NextPieceChanged())
    {
        GameContext::Instance()->GetCurrentPiece().Clear();
        GameContext::Instance()->GetNextPiece().Draw(true);
        GameContext::Instance()->SetNextPieceChanged(false);
    }

    //Draw Play Grid
    for (int y = 0; y < playGridHeight; y++) 
    {
        for (int x = 0; x < playGridWidth; x++) 
        {
            if (GameContext::Instance()->ChangedGrid()[y][x] == true)
            {
                int mapIndex = (playGridY + y) * 32 + (playGridX + x);
                if (GameContext::Instance()->PlayGrid()[y][x] > 0)
                    se_mem[ACTION_LAYER_ID][mapIndex] = 120 + (GameContext::Instance()->PlayGrid()[y][x] - 1);                    
                else
                    se_mem[ACTION_LAYER_ID][mapIndex] = BLANK_TILE;
                GameContext::Instance()->ChangedGrid()[y][x] = false;
            }
        }
    }    

    //Draw Current Piece
    GameContext::Instance()->GetCurrentPiece().Draw(false);
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