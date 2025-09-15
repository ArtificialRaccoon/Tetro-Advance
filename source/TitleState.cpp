#include "TitleState.h"

void TitleState::InitState()
{
    mmStart( MOD_TITLE, MM_PLAY_LOOP );
    memcpy16(pal_bg_mem, GAMEUIPal, GAMEUIPalLen/2);
    memcpy32(&tile_mem[0][0], GAMEUITiles, GAMEUITilesLen/4);
    tte_init_chr4c(1, BG_CBB(2) | BG_SBB(TEXT_LAYER_ID), 0xF000, 0x1E, (u32)&sys8Font, NULL, NULL); //Need to change tghe 0x1E to the right colour index...
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
    if (debounceTimer > 0)
    {
        debounceTimer--;
        return;
    }

    u16 keys_pressed = ~REG_KEYINPUT & KEY_MASK;
    if (keys_pressed & KEY_DOWN)
    {
        selectedItem++;
        debounceTimer = 10;
    }
    else if (keys_pressed & KEY_UP)
    {
        selectedItem--;
        debounceTimer = 10;
    }
    else if (keys_pressed & KEY_B)
    {
        trigger = true;
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
}