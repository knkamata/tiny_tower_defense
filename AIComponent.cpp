#include "AIComponent.h"

AIComponent::AIComponent(Actor *owner)
	: Component(owner),
	  mCurrentState(nullptr)
{
}

void AIComponent::Update(float deltaTime)
{
	if (mCurrentState)
	{
		// Each state has own update
		mCurrentState->Update(deltaTime);
	}
}

void AIComponent::ChangeState(const string &name)
{
	// First exit the current state
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// Try to find the new state from the map
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		// Entering the new state
		mCurrentState->OnExit();
	}
	else
	{
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState *state)
{
	mStateMap.emplace(state->GetName(), state);
}
