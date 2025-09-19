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
        void HandleInput(u16 keys_pressed);
        void ProcessInput(GameProcessor* game);
        void Render(GameProcessor* game);
        void Reset();
        void UnloadResources();
        void PrintText(std::string text, int x, int y);
        static GameState* Instance()
        {
            static GameState mGameState; 
            return &mGameState;
        }

    protected:
        GameState() { }
        std::string formatInteger(int leadingZeros, int value);  
    
    private:
        int debounceTimer = 0;
        unsigned short heldButtons = 0;
        const int initial = 10;
        const int repeat = 4;
};

#endif // GAMESTATE_H
