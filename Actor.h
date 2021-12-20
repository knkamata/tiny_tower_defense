#pragma once

#include "Math.h"
#include "Game.h"
#include "Component.h"

using namespace std;

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game *game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const uint8_t *keyState);
	virtual void ActorInput(const uint8_t *keyState);

	// Getters/Setters
	State GetState() const { return mState; }
	void  SetState(State state) { mState = state; }

	float GetScale() const { return mScale; }
	void  SetScale(float scale) { mScale = scale; }

	float GetRotation() const { return mRotation; }
	void  SetRotation(float rotation) { mRotation = rotation; }

	const Vector2 &GetPosition() const { return mPosition; }
	void           SetPosition(const Vector2 &pos) { mPosition = pos; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

	class Game *GetGame() { return mGame; }

	// Add/Remove components
	void AddComponent(class Component *component);
	void RemoveComponent(class Component *component);
private:
	class Game *mGame;
	vector<class Component *> mComponents;

	State   mState;
	float   mScale;
	float   mRotation;
	Vector2 mPosition;
};
