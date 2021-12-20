#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Enemy.h"
#include "Bullet.h"

class Tower : public Actor
{
public:
	Tower(class Game *game);
	void UpdateActor(float deltaTime) override;
private:
	class MoveComponent *mMove;
	float mNextAttack;
	const float AttackTime = 2.5f;
	const float AttackRange = 100.0f;
};
