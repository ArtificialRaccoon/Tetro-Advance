#include "TitleState.h"

void TitleState::InitState()
{
    mmStart( MOD_TITLE, MM_PLAY_LOOP );
    memcpy16(pal_bg_mem, GAMEUIPal, GAMEUIPalLen/2);
    memcpy32(&tile_mem[0][0], GAMEUITiles, GAMEUITilesLen/4);
    tte_init_chr4c(1, BG_CBB(2) | BG_SBB(TEXT_LAYER_ID), 0xF000, bytes2word(13,15,0,0), CLR_WHITE, NULL, NULL);
    tte_erase_screen();
}

void TitleState::Pause()
{

} 

void TitleState::Resume()
{

}

void TitleState::AquireInput(GameProcessor* game)
{
    key_poll();
    unsigned short currentlyPressedButtons = key_hit(KEY_UP | KEY_DOWN | KEY_B);
    unsigned short currentlyHeldButtons = key_is_down(KEY_UP | KEY_DOWN | KEY_B);

    if (currentlyPressedButtons)
    {
        if (currentlyPressedButtons & KEY_DOWN)
            selectedItem++;
        else if (currentlyPressedButtons & KEY_UP)
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
            if (heldButtons & KEY_DOWN)
                selectedItem++;
            else if (heldButtons & KEY_UP)
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

void TitleState::ProcessInput(GameProcessor* game)
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
            this->UnloadResources();
            GameState::Instance()->Reset();
            game->ChangeState(GameState::Instance());
        }
        else if(selectedItem == 1)
            GameContext::Instance()->SwapRandomizer();
    }
}

void TitleState::Render(GameProcessor* game)
{
    tte_erase_screen();
    memset16(se_mem[ACTION_LAYER_ID], BLANK_TILE, 32*32);
    memcpy16(se_mem[BACKGROUND_LAYER_ID], TITLESCREEN, sizeof(TITLESCREEN)/2);
    
    //I shouldn't be re-rendering the text every frame but I don't care enough to fix it right now
    PrintText("Play Game", menuXStart, menuYStart);
    PrintText(std::string("Randomizer: ") + (GameContext::Instance()->UseSevenBag() ? "7-Bag" : "TGM"), menuXStart, menuYRandom);
    se_mem[ACTION_LAYER_ID][selectedItem == 0 ? CURSOR_TILE_POS_PLAY : CURSOR_TILE_POS_SWAP] = CURSOR_TILE;
}

void TitleState::PrintText(std::string text, int x, int y)
{    
    char cursor[16];
    snprintf(cursor, sizeof(cursor), "#{P:%d,%d}", x, y);
    tte_write(cursor);
    tte_write(text.c_str());
}

void TitleState::UnloadResources()
{
    mmStop();
    tte_erase_screen();
    se_mem[ACTION_LAYER_ID][CURSOR_TILE_POS_PLAY] = BLANK_TILE;
    se_mem[ACTION_LAYER_ID][CURSOR_TILE_POS_SWAP] = BLANK_TILE;
}