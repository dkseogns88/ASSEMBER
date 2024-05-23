#include "pch.h"
#include "Monster.h"
#include "BehaviorTree.h"
#include "Room.h"

Monster::Monster()
{

}

Monster::~Monster()
{

}

void Monster::executeBehavior()
{
	_behaviorTree->execute();
}

void Monster::ChasePlayer()
{
	float speed = 0.01f;

	while (true) {
		FVector oldVector(posInfo->x(), posInfo->y(), posInfo->z());
		FVector NewVector(_target.lock()->posInfo->x(), _target.lock()->posInfo->y(), _target.lock()->posInfo->z());

		FVector direction = NewVector - oldVector;
		float distance = direction.length();

		if (distance < speed)
		{
			posInfo->set_x(NewVector.x);
			posInfo->set_y(NewVector.y);
			posInfo->set_z(NewVector.z);
			return;
		}
		else {
			FVector move = direction.normalized() * speed;
			oldVector = oldVector + move;
			posInfo->set_x(oldVector.x);
			posInfo->set_y(oldVector.y);
			posInfo->set_z(oldVector.z);

			
			FVector playerLocation{ GRoom->_testPlayer->posInfo->x(), GRoom->_testPlayer->posInfo->y(),GRoom->_testPlayer->posInfo->z() };
			FVector monsterLocation{ oldVector };

			if ((playerLocation - monsterLocation).length() < 500.f) {
				cout << "공격!" << endl;
				break;
			}
		}

		static std::chrono::steady_clock::time_point lastBroadcastTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed = currentTime - lastBroadcastTime;

		if (elapsed.count() >= 0.2f) {
			{

				Protocol::S_MOVE movePkt;
				Protocol::PosInfo* info = movePkt.mutable_info();
				info->CopyFrom(*posInfo);
				info->set_state(Protocol::MOVE_STATE_RUN);
				SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
				if (RoomRef _room = room.load().lock()) {
					_room->Broadcast(sendBuffer);
				}
				lastBroadcastTime = currentTime;
			}
		}
	}

	this_thread::sleep_for(1s);
}

void Monster::MoveToRandom()
{

	FVector oldVector(posInfo->x(), posInfo->y(), posInfo->z());
	FVector NewVector(Utils::GetRandom(0.f, 2000.f), Utils::GetRandom(0.f, 2000.f), Utils::GetRandom(100.f, 100.f));

	float speed = 1.f;

	while (true) {
		FVector direction = NewVector - oldVector;
		float distance = direction.length();

		if (distance < speed)
		{
			posInfo->set_x(NewVector.x);
			posInfo->set_y(NewVector.y);
			posInfo->set_z(NewVector.z);
			break;
		}
		else {
			FVector move = direction.normalized() * speed;
			oldVector = oldVector + move;
			posInfo->set_x(oldVector.x);
			posInfo->set_y(oldVector.y);
			posInfo->set_z(oldVector.z);

			// 여기서 캐릭터가 거리내에 있는 지 확인!
			FVector playerLocation{ GRoom->_testPlayer->posInfo->x(), GRoom->_testPlayer->posInfo->y(),GRoom->_testPlayer->posInfo->z() };
			FVector monsterLocation{ oldVector };

			if ((playerLocation - monsterLocation).length() < 500.f) {
				cout << "Target!" << endl;
				_target = GRoom->_testPlayer;
				return;
			}
			else {
			}

		}
		
		static std::chrono::steady_clock::time_point lastBroadcastTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed = currentTime - lastBroadcastTime;
		
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
				lastBroadcastTime = currentTime;
			}
		}
	}

	this_thread::sleep_for(3s);
}
