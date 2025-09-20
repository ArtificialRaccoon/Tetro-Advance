#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <cmath>
#include "tonc.h"
#include "GameContext.h"
#include "GameConstants.h"
#include "BaseState.h"
#include "GameState.h"
#include "TitleState.h"
#include "GAMEOVER.h"

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
        void PrintText(std::string text, int x, int y);
        static GameOverState* Instance()
        {
            static GameOverState mGameOverState;
            return &mGameOverState;
        }

    protected:
        GameOverState() { }        

    private:
        int selectedItem = 0;
        int debounceTimer = 0;
        unsigned short heldButtons = 0;
        const int initial = 10;        
        bool trigger = false;
};

#endif // GAMEOVERSTATE_H
