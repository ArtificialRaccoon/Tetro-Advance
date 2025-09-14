#include "TitleState.h"

void TitleState::InitState()
{
    // Start playing module
	mmStart( MOD_THEME, MM_PLAY_LOOP );
}

void TitleState::Pause()
{

} 

void TitleState::Resume()
{

}

void TitleState::AquireInput(GameProcessor* game)
{
    int keys_pressed;
    scanKeys();
    keys_pressed = keysDown();
    if (keys_pressed & KEY_DOWN) selectedItem++;
    else if (keys_pressed & KEY_UP) selectedItem--;
    else if (keys_pressed & KEY_B) trigger = true;
}

void TitleState::ProcessInput(GameProcessor* game)
{ 
    if(selectedItem < 0)
        selectedItem = 2;
    if(selectedItem > 2)
        selectedItem = 0;

    if(trigger)
    {
        trigger = false;
        if(selectedItem == 0)
        {
            this->UnloadResources();
        }
        else if(selectedItem == 1)
            GameContext::Instance()->SwapRandomizer();
        else
            exit(0);
    }
}

void TitleState::Render(GameProcessor* game)
{

}

void TitleState::UnloadResources()
{

}