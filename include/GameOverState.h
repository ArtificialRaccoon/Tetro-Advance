#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <cmath>
#include "tonc.h"
#include "GameContext.h"
#include "GameConstants.h"
#include "BaseState.h"
#include "GameState.h"
#include "TitleState.h"

class GameOverState : public BaseState
{
    public:
        void InitState();
        void Pause();
        void Resume();
        void AquireInput(GameProcessor* game);
        void ProcessInput(GameProcessor* game);
        void Render(GameProcessor* game);
        void UnloadResources();
        static GameOverState* Instance()
        {
            static GameOverState mGameOverState;
            return &mGameOverState;
        }

    protected:
        GameOverState() { } 
};

#endif // GAMEOVERSTATE_H
