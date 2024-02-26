#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

RoomRef GRoom = make_shared<Room>();

Room::Room()
{

}

Room::~Room()
{

}

bool Room::HandleEnterPlayerLocked(PlayerRef player)
{
	WRITE_LOCK;
	bool success = EnterPlayer(player);

	// 랜덤 위치
	player->playerInfo->set_x(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_y(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_z(Utils::GetRandom(100.f, 100.f));
	player->playerInfo->set_yaw(Utils::GetRandom(0.f, 500.f));

	// 입장 사실을 들어온 플레이어에게 알린다.
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->playerInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장 사실을 다른 플레이어에게 알린다.
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
		playerInfo->CopyFrom(*player->playerInfo);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, player->playerInfo->object_id());
	}

	// 입장한 플레이어들을 새로운 플레이어에게 알려줘야 한다.
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _players) {
			Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->playerInfo);
		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
}

bool Room::HandleLeavePlayerLocked(PlayerRef player)
{
	if (player == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = player->playerInfo->object_id();
	bool success = LeavePlayer(objectId);

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다.
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 모두에게 알린다.
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return true;
}

void Room::HandleMoveLocked(Protocol::C_MOVE& pkt)
{
	WRITE_LOCK;

	const uint64 objectId = pkt.info().object_id();
	if (_players.find(objectId) == _players.end())
		return;

	PlayerRef& player = _players[objectId];
	player->playerInfo->CopyFrom(pkt.info());

	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PlayerInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
		}
		
		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);

	}

}

bool Room::EnterPlayer(PlayerRef player)
{
	// 있는지 확인
	if(_players.find(player->playerInfo->object_id()) != _players.end())
		return false;

	_players.insert(make_pair(player->playerInfo->object_id(), player));

	player->room.store(shared_from_this());

	return true;
}

bool Room::LeavePlayer(uint64 objectId)
{
	if (_players.find(objectId) == _players.end())
		return false;

	PlayerRef player = _players[objectId];
	player->room.store(weak_ptr<Room>());

	_players.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _players)
	{
		PlayerRef player = item.second;
		if (player->playerInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}