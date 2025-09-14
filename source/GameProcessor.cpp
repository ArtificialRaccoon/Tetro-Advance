#include "BaseState.h"
#include "GameProcessor.h"

void GameProcessor::InitializeGame() {}

void GameProcessor::HandleEvents() { states.back()->ProcessInput(this); }

void GameProcessor::ProcessEvents()  { states.back()->AquireInput(this); }

void GameProcessor::Render() 
{ 
	states.back()->Render(this); 
	VBlankIntrWait();
	mmFrame();
}

void GameProcessor::DeallocateResources() {}

void GameProcessor::ChangeState(BaseState *state) 
{
	// cleanup the current state
	if (!states.empty()) 
	{
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->InitState();
}

void GameProcessor::PushState(BaseState *state) 
{
	// pause current state
	if (!states.empty())
		states.back()->Pause();

	// store and init the new state
	states.push_back(state);
	states.back()->InitState();
}

void GameProcessor::PopState() 
{
	// cleanup the current state
	if (!states.empty()) 
		states.pop_back();

	// resume previous state
	if (!states.empty())
		states.back()->Resume();
}
