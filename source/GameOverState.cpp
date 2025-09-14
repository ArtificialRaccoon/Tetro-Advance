#include "GameOverState.h"

void GameOverState::InitState()
{
    //Save Top Score
    if(GameContext::Instance()->GetTopScore() > GameContext::Instance()->GetCurrentScore())
        GameContext::Instance()->SaveTopScore();
}

void GameOverState::Pause()
{

} 


void GameOverState::Resume()
{

}

void GameOverState::AquireInput(GameProcessor* game)
{
    u16 keys_pressed = ~REG_KEYINPUT & KEY_MASK;
    if(keys_pressed & KEY_A)
    {
        GameState::Instance()->Reset();
        this->UnloadResources();
        game->ChangeState(GameState::Instance());
    }
    else if(keys_pressed & KEY_B)
    {
        this->UnloadResources();
        game->ChangeState(TitleState::Instance());
    }
}

void GameOverState::ProcessInput(GameProcessor* game)
{ 

}

void GameOverState::Render(GameProcessor* game)
{

}

void GameOverState::UnloadResources()
{

}