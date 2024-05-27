#include "pch.h"
#include "Monster.h"
#include "BehaviorTree.h"
#include "Room.h"

Monster::Monster()
{
	_time.Init();
	_lastSendTime = chrono::high_resolution_clock::now();
	_newLocation = FVector(posInfo->x(), posInfo->y(), posInfo->z());

}

Monster::~Monster()
{

}

void Monster::executeBehavior()
{
	_time.Update();
	_behaviorTree->execute();
}

void Monster::ChasePlayer()
{
	float DeltaTime = _time.GetDeltaTime();
	float speed = 1000.f;

	PlayerRef target = _target.lock();

	FVector nowLocation(posInfo->x(), posInfo->y(), posInfo->z());
	_newLocation = FVector(target->posInfo->x(), target->posInfo->y(), target->posInfo->z());

	FVector direction = _newLocation - nowLocation;

	// 현재 위치와 캐릭터 위치 비교!
	{
		FVector playerLocation{ GRoom->_testPlayer->posInfo->x(), GRoom->_testPlayer->posInfo->y(),GRoom->_testPlayer->posInfo->z() };

		if ((playerLocation - nowLocation).lengthXY() < 500.f) {
			cout << "Attack!" << endl;
			return;
		}
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

void Monster::Attack()
{

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

	// 현재 위치와 캐릭터 위치 비교!
	{
		FVector playerLocation{ GRoom->_testPlayer->posInfo->x(), GRoom->_testPlayer->posInfo->y(),GRoom->_testPlayer->posInfo->z() };

		if ((playerLocation - nowLocation).lengthXY() < 1000.f) {
			cout << "Target!" << endl;
			_target = GRoom->_testPlayer;
			return;
		}
	}

	FVector move = direction.normalized() * DeltaTime * speed;

	//cout << "이전 위치: " << nowLocation.x << ", " << nowLocation.y << ", " << nowLocation.z << endl;

	nowLocation = nowLocation + move;
	posInfo->set_x(nowLocation.x);
	posInfo->set_y(nowLocation.y);
	posInfo->set_z(nowLocation.z);
	
	//cout << "새로운 위치: " << nowLocation.x << ", " << nowLocation.y << ", " << nowLocation.z << endl;

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
