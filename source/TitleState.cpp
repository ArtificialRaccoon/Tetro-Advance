#include "TitleState.h"

void TitleState::InitState()
{
	mmStart( MOD_TITLE, MM_PLAY_LOOP );
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

}

void TitleState::UnloadResources()
{
    mmStop();
}