#pragma once
#include "Actor.h"
#include "Grid.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"
#include "NavComponent.h"

class Enemy : public Actor
{
public:
	Enemy(class Game* game);
	~Enemy();
	void UpdateActor(float deltaTime) override;
	class CircleComponent* GetCircle() { return mCircle; }
private:
	class CircleComponent* mCircle;
};
