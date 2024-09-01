#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"
#include "ObjectUtils.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	Protocol::S_LOGIN loginPkt;

	loginPkt.set_success(true);
	SEND_PACKET(loginPkt);
	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
	
	// TODO: 플레이어 타입 정의
	player->objectInfo->set_player_type(Protocol::PLAYER_TYPE_NONE);

	// TODO: 플레이어 스탯 정의
	player->statInfo->set_object_id(player->objectInfo->object_id());
	player->statInfo->set_hp(100);
	player->statInfo->set_max_hp(100);
	player->statInfo->set_damage(10);

	GRoom->DoAsync(&Room::HandleEnterPlayer, player);

	return true;

}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	GRoom->DoAsync(&Room::HandleLeavePlayer, player); // JobQueue 방식
	return true;

}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleMove, pkt);

	return true;

}

bool Handle_C_STATE(PacketSessionRef& session, Protocol::C_STATE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleSate, pkt);
	return true;
}

bool Handle_C_ATTACK(PacketSessionRef& session, Protocol::C_ATTACK& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleAttack, pkt);
	return true;
}