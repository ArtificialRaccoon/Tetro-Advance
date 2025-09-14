#include "TitleState.h"

void TitleState::InitState()
{
    mmStart( MOD_TITLE, MM_PLAY_LOOP );
    memcpy16(pal_bg_mem, GAMEUIPal, GAMEUIPalLen/2);
    memcpy32(&tile_mem[0][0], GAMEUITiles, GAMEUITilesLen/4);
}

void TitleState::Pause()
{

} 

void TitleState::Resume()
{

}

void TitleState::AquireInput(GameProcessor* game)
{
    u16 keys_pressed = ~REG_KEYINPUT & KEY_MASK;
    if (keys_pressed & KEY_DOWN) selectedItem++;
    else if (keys_pressed & KEY_UP) selectedItem--;
    else if (keys_pressed & KEY_B) trigger = true;
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
    memcpy16(se_mem[31], TITLESCREEN, sizeof(TITLESCREEN)/2);
}

void TitleState::UnloadResources()
{
    mmStop();
}