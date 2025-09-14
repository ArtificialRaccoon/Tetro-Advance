#ifndef GAMEPROCESSOR_H
#define GAMEPROCESSOR_H

#include <vector>
#include <iostream>
#include <gba.h>
#include <maxmod.h>

using namespace std;

class BaseState;
class GameProcessor 
{
	public:
		void InitializeGame();
		void HandleEvents();
		void ProcessEvents();
		void Render();
		void DeallocateResources();

		void ChangeState(BaseState *state);
		void PushState(BaseState *state);
		void PopState();

	private:
		std::vector<BaseState *> states;		
};

#endif// GAMEPROCESSOR_H
