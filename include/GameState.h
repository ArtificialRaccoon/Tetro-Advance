#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdlib>
#include <algorithm>
#include <memory>
#include <cmath>
#include <cstdlib> 
#include <ctime>
#include "tonc.h"
#include "GameContext.h"
#include "GameConstants.h"
#include "Tetromino.h"
#include "GameOverState.h"
#include "BaseState.h"
#include "GAMEUI.h"
#include "PLAYAREA.h"

class GameState : public BaseState
{
    public:
        void InitState();
        void Pause();
        void Resume();
        void AquireInput(GameProcessor* game);
        void ProcessInput(GameProcessor* game);
        void Render(GameProcessor* game);
        void Reset();
        void UnloadResources();
        static GameState* Instance()
        {
            static GameState mGameState; 
            return &mGameState;
        }

    protected:
        GameState() { }
        void DrawBackground();        
        std::string formatInteger(int leadingZeros, int value);  
};

#endif // GAMESTATE_H
