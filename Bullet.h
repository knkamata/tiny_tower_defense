#pragma once
#include "Actor.h"
#include "Enemy.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"

class Bullet : public Actor
{
public:
	Bullet(class Game* game);
	void UpdateActor(float deltaTime) override;
private:
	class CircleComponent* mCircle;
	float mLiveTime;
};
