#pragma once
#include "Creature.h"
#include "TimeManager.h"

class BehaviorTree;
class Player;

class Monster : public Creature
{
public:
	Monster();
	virtual ~Monster();

	void executeBehavior();

	void ChasePlayer();
	void Attack();
	void MoveToRandom();

public:
	BehaviorTree* _behaviorTree;

	weak_ptr<Player> _target;

	TimeManager _time;
	chrono::steady_clock::time_point _lastSendTime;

	FVector _newLocation;
};

