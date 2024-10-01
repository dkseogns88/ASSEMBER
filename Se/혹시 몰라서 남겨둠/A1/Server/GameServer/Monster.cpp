#include "pch.h"
#include "Monster.h"
#include "BehaviorTree.h"
#include "Room.h"
#include "PathFinder.h"
#include "CoroutineJob.h"

Monster::Monster()
{
	_time.Init();
	_lastSendTime = chrono::high_resolution_clock::now();
	_newLocation = FVector(posInfo->x(), posInfo->y(), posInfo->z());
	_behaviorTree = nullptr;
}

Monster::~Monster()
{
	cout << "Delete Monster" << endl;
}

void Monster::executeBehavior()
{
	_time.Update();
	_behaviorTree->execute();
}

void Monster::MoveToRandom()
{
	float DeltaTime = _time.GetDeltaTime();
	float speed = 1000.f;

	FVector nowLocation(posInfo->x(), posInfo->y(), posInfo->z());
	FVector direction = _newLocation - nowLocation;

	if (nowLocation == _newLocation || direction.length() < speed * DeltaTime) {
		_newLocation = FVector(Utils::GetRandom(0.f, 2000.f), Utils::GetRandom(0.f, 2000.f), posInfo->z());
	}

	// ���� ��ġ�� ĳ���� ��ġ ��!
	{
		FVector player1Location{ GRoom->_player1->posInfo->x(), GRoom->_player1->posInfo->y(),GRoom->_player1->posInfo->z() };

		if ((player1Location - nowLocation).length() < 1000.f) {
			cout << "Target!" << endl;
			_target = GRoom->_player1;
			return;
		}
	}

	FVector move = direction.normalized() * DeltaTime * speed;

	//cout << "���� ��ġ: " << nowLocation.x << ", " << nowLocation.y << ", " << nowLocation.z << endl;

	nowLocation = nowLocation + move;
	posInfo->set_x(nowLocation.x);
	posInfo->set_y(nowLocation.y);
	posInfo->set_z(nowLocation.z);

	//cout << "���ο� ��ġ: " << nowLocation.x << ", " << nowLocation.y << ", " << nowLocation.z << endl;

	chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = currentTime - _lastSendTime;

	if (elapsed.count() >= 0.05) {
		{
			Protocol::S_MOVE movePkt;
			Protocol::PosInfo* info = movePkt.mutable_info();
			info->CopyFrom(*posInfo);
			info->set_state(Protocol::MOVE_STATE_RUN);
			SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
			if (RoomRef _room = room.load().lock()) {
				_room->Broadcast(sendBuffer);
			}
			_lastSendTime = currentTime;
		}
	}
}

void Monster::ChasePlayer()
{
	float DeltaTime = _time.GetDeltaTime();
	float speed = 1000.f;

	PlayerRef target = _target.lock();

	FVector nowLocation(posInfo->x(), posInfo->y(), posInfo->z());
	_newLocation = FVector(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());

	FVector direction = _newLocation - nowLocation;

	// ���� ��ġ�� ĳ���� ��ġ ��!
	{
		FVector playerLocation{ target->posInfo->x(), target->posInfo->y(), target->posInfo->z() };

		/*if ((playerLocation - nowLocation).length() < 500.f) {
			Protocol::S_ATTACK attackPkt;
			Protocol::PosInfo* info = attackPkt.mutable_info();
			info->CopyFrom(*posInfo);
			info->set_state(Protocol::MOVE_STATE_ATTACK);
			SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(attackPkt);
			if (RoomRef _room = room.load().lock()) {
				_room->Broadcast(sendBuffer);
			}
			return;
		}*/
	}


	FVector move = direction.normalized() * DeltaTime * speed;
	nowLocation = nowLocation + move;
	posInfo->set_x(nowLocation.x);
	posInfo->set_y(nowLocation.y);
	posInfo->set_z(nowLocation.z);

	chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = currentTime - _lastSendTime;


	if (elapsed.count() >= 0.05) {
		{
			Protocol::S_MOVE movePkt;
			Protocol::PosInfo* info = movePkt.mutable_info();
			info->CopyFrom(*posInfo);
			info->set_state(Protocol::MOVE_STATE_RUN);
			SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
			if (RoomRef _room = room.load().lock()) {
				_room->Broadcast(sendBuffer);
			}
			_lastSendTime = currentTime;
		}
	}
}

void  Monster::DoPathFinding(MonsterRef npc)
{
	DestVec.clear();
	isArrvied = false;

	Protocol::S_PATHFINDING pkt;

	float x = npc->vector3D.x;
	float y = npc->vector3D.y;
	float z = npc->vector3D.z;

	//���� npc�� ��ġ���� ���� ����� ��� ���ؼ� �������� �־��ֱ�.
	FVector start = GPathFinder->CreateVec(x, y, z);

	//���� ��ġ
	FVector goal = GPathFinder->ReturnRandomPos();
	
	goal.x = 1900.f;
	goal.y = 1900.f;
	auto coroutine = CoroutineJob::CoroutineFunc(GPathFinder, start, goal);

	while (true)
	{
		if (GPathFinder->IsRead)
		{
			coroutine.resume();
			break;
		}
	}

	for (auto p : GPathFinder->path)
	{
		DestVec.push_back(p);

		cout << "(" << p.x << "," << p.y << "," << p.z << ") ->";
	}

	idx = 1;
	size = DestVec.size();
	lastDest = DestVec[DestVec.size() - 1];
	posInfo->set_state(Protocol::MoveState::MOVE_STATE_RUN);

	cout << endl;
}

void Monster::CheckIdx()
{
	if (!DestVec.empty() && idx < size)
	{
		curDest = DestVec[idx];
		auto dist = vector3D.DistanceTo2D(curDest);
		//cout << "�Ÿ� : " << dist << endl;
		//cout << "��� idx : " << idx << endl;
		//cout << "��� ���� : " << size << endl;

		if (dist < 100.f)
		{
			idx++;

			if (idx == size)
				idx--;

			posInfo->set_state(Protocol::MoveState::MOVE_STATE_RUN);
		}

		if (vector3D.DistanceTo2D(lastDest) < 100.f)
		{
			isArrvied = true;
			posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);
		}
	}
}

FVector Monster::GetDestByIdx(int idx_)
{
	return DestVec[idx_];
}

