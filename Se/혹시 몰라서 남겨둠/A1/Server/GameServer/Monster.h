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

	void MoveToRandom();
	void ChasePlayer();

	void  DoPathFinding(MonsterRef npc);
	void CheckIdx();

	FVector GetDestByIdx(int idx);

public:
	BehaviorTree* _behaviorTree;

	weak_ptr<Player> _target;

	TimeManager _time;
	chrono::steady_clock::time_point _lastSendTime;

	FVector _newLocation;

	FVector vector3D;
	FVector curDest;
	FVector lastDest;

	vector<FVector> DestVec;
	int size = 0;

	bool isArrvied = false;
	int idx = 0;
};

