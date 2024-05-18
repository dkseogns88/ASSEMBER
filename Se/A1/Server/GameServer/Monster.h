#pragma once
#include "Creature.h"

class BehaviorTree;

class Monster : public Creature
{
public:
	Monster();
	virtual ~Monster();


	void executeBehavior();
	void UpdateMonsterRandomPosition();

	BehaviorTree* _behaviorTree;
};

