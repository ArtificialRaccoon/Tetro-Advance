#include "GameState.h"

void GameState::InitState()
{
    mmStart( MOD_TETRIS, MM_PLAY_LOOP );
    memcpy16(pal_bg_mem, GAMEUIPal, GAMEUIPalLen/2);
    memcpy32(&tile_mem[0][0], GAMEUITiles, GAMEUITilesLen/4);
    //Need to change the 0x1E to the right colour index; it's currently a random colour
    tte_init_chr4c(1, BG_CBB(2) | BG_SBB(TEXT_LAYER_ID), 0xF000, 0x1E, (u32)&sys8Font, NULL, NULL);
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
    tte_erase_screen();
    memcpy16(se_mem[BACKGROUND_LAYER_ID], PLAYAREA, sizeof(PLAYAREA)/2);

    //Lines
    PrintText(std::string("Lines: ") + formatInteger(3, GameContext::Instance()->GetCurrentLines()), 168, 5);

    //Current Level
    PrintText(std::string("Level"), 168, 29);  
    //Need to read the docs to find out why, but this seems to cause the score to render weirdly
    PrintText(formatInteger(2, GameContext::Instance()->GetCurrentLevel()), 220, 29);

    //Score
    PrintText(std::string("Score: ") + formatInteger(6, GameContext::Instance()->GetCurrentScore()), 168, 56);

    //Draw Next Piece
    PrintText(std::string("Next"), 188, 100);
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