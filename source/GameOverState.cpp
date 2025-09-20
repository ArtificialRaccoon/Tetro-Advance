#include "GameOverState.h"

void GameOverState::InitState()
{
    //Save Top Score
    if(GameContext::Instance()->GetCurrentScore() >= GameContext::Instance()->GetTopScore())
        GameContext::Instance()->SaveTopScore();

    memcpy16(se_mem[BACKGROUND_LAYER_ID], GAMEOVER, sizeof(GAMEOVER)/2);

    for (int y = 0; y < playGridHeight; y++) 
    {
        for (int x = 0; x < playGridWidth; x++) 
        {
            int mapIndex = (playGridY + y) * 32 + (playGridX + x);
            se_mem[ACTION_LAYER_ID][mapIndex] = BLANK_TILE;
        }
    }
    
    PrintText(std::string("Game Over"), 96, 54);
    PrintText(std::string("Try Again?"), 104, 68);
    PrintText(std::string("Yes"), 104, 86);
    PrintText(std::string("No"), 144, 86);
}

void GameOverState::Pause()
{

} 


void GameOverState::Resume()
{

}

void GameOverState::AquireInput(GameProcessor* game)
{
    key_poll();
    unsigned short currentlyPressedButtons = key_hit(KEY_RIGHT | KEY_LEFT | KEY_B);
    unsigned short currentlyHeldButtons = key_is_down(KEY_RIGHT | KEY_LEFT | KEY_B);

    if (currentlyPressedButtons)
    {
        if (currentlyPressedButtons & KEY_RIGHT)
            selectedItem++;
        else if (currentlyPressedButtons & KEY_LEFT)
            selectedItem--;
        else if (currentlyPressedButtons & KEY_B)
            trigger = true;

        heldButtons = currentlyHeldButtons;
        debounceTimer = initial;
    }
    else if (heldButtons && currentlyHeldButtons == heldButtons)
    {
        if (debounceTimer > 0)
            debounceTimer--;
        else
        {
            if (heldButtons & KEY_RIGHT)
                selectedItem++;
            else if (heldButtons & KEY_LEFT)
                selectedItem--;
            else if (heldButtons & KEY_B)
                trigger = true;
            debounceTimer = initial;
        }
    }
    else
    {
        heldButtons = 0;
        debounceTimer = 0;
    }
}

void GameOverState::ProcessInput(GameProcessor* game)
{ 
    if(selectedItem < 0)
        selectedItem = 1;
    if(selectedItem > 1)
        selectedItem = 0;

    if(trigger)
    {
        trigger = false;
        if(selectedItem == 0)
        {
            GameState::Instance()->Reset();
            this->UnloadResources();
            game->ChangeState(GameState::Instance());
        }
        else if(selectedItem == 1)
        {
            this->UnloadResources();
            game->ChangeState(TitleState::Instance());
        }
    }
}

void GameOverState::Render(GameProcessor* game)
{
    se_mem[ACTION_LAYER_ID][selectedItem == 0 ? CURSOR_TILE_POS_YES : CURSOR_TILE_POS_NO] = CURSOR_TILE;
    se_mem[ACTION_LAYER_ID][selectedItem == 1 ? CURSOR_TILE_POS_YES : CURSOR_TILE_POS_NO] = BLANK_TILE;
}

void GameOverState::UnloadResources()
{
    tte_erase_screen();
    se_mem[ACTION_LAYER_ID][CURSOR_TILE_POS_YES] = BLANK_TILE;
    se_mem[ACTION_LAYER_ID][CURSOR_TILE_POS_NO] = BLANK_TILE;
}

//Move this into a common location at some point
void GameOverState::PrintText(std::string text, int x, int y)
{    
    char cursor[16];
    snprintf(cursor, sizeof(cursor), "#{P:%d,%d}", x, y);
    tte_write(cursor);
    tte_write(text.c_str());
}