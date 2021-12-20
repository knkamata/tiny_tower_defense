#pragma once
#include "Component.h"
#include "AIState.h"

class AIComponent : public Component
{
public:
	AIComponent(class Actor *owner);

	void Update(float deltaTime) override;
	void ChangeState(const string &name);
	void RegisterState(class AIState *state);
private:
	class AIState *mCurrentState;
	unordered_map<string, class AIState *> mStateMap;
};
