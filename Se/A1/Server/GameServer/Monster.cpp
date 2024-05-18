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

	UpdateMonsterRandomPosition();
}

void Monster::UpdateMonsterRandomPosition()
{
	posInfo->set_x(Utils::GetRandom(0.f, 1000.f));
	posInfo->set_y(Utils::GetRandom(0.f, 1000.f));
	posInfo->set_z(Utils::GetRandom(100.f, 100.f));
	posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));


	Protocol::S_MOVE movePkt;
	{
		Protocol::PosInfo* info = movePkt.mutable_info();
		info->set_state(Protocol::MOVE_STATE_RUN);
		info->CopyFrom(*posInfo);
	}
	{
		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
		if (RoomRef _room = room.load().lock()) {
			_room->Broadcast(sendBuffer);
			cout << "BroadCast!!" << endl;
		}
	}
}
