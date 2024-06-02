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
	cout << "Delete Monster" << endl;
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
		FVector playerLocation{ target->posInfo->x(), target->posInfo->y(), target->posInfo->z() };

		if ((playerLocation - nowLocation).length() < 500.f) {
			Protocol::S_ATTACK attackPkt;
			Protocol::PosInfo* info = attackPkt.mutable_info();
			info->CopyFrom(*posInfo);
			info->set_state(Protocol::MOVE_STATE_ATTACK);
			SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(attackPkt);
			if (RoomRef _room = room.load().lock()) {
				_room->Broadcast(sendBuffer);
			}
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
		_newLocation = FVector(Utils::GetRandom(-3000.f, -2500.f), Utils::GetRandom(0.f, 500.f), posInfo->z());
	}

	// 현재 위치와 캐릭터 위치 비교!
	{
		FVector player1Location{ GRoom->_player1->posInfo->x(), GRoom->_player1->posInfo->y(),GRoom->_player1->posInfo->z() };

		if ((player1Location - nowLocation).length() < 1000.f) {
			cout << "Target!" << endl;
			_target = GRoom->_player1;
			return;
		}

		
		/*FVector player2Location{ GRoom->_player2->posInfo->x(), GRoom->_player2->posInfo->y(),GRoom->_player2->posInfo->z() };
		if ((player2Location - nowLocation).lengthXY() < 1000.f) {
			cout << "Target!" << endl;
			_target = GRoom->_player2;
			return;
		}*/
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
