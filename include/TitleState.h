#ifndef TITLESTATE_H
#define TITLESTATE_H

#include <cmath>
#include "tonc.h"
#include "GameState.h"
#include "GameContext.h"
#include "GameConstants.h"
#include "BaseState.h"
#include "soundbank.h"
#include "soundbank_bin.h"
#include "GAMEUI.h"
#include "TITLESCREEN.h"

class TitleState : public BaseState
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
        static TitleState* Instance()
        {
            static TitleState mTitleState;
            return &mTitleState;
        }

    protected:
        TitleState() { } 

    private:
        int selectedItem = 0;
        int debounceTimer = 0;
        bool trigger = false;
};

#endif // TitleState_H
