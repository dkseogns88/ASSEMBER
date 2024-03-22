#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "MyProject.h"
#include "MyProjectGameInstance.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];


bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	for (int32 i = 0; i < pkt.players_size(); ++i)
	{
		const Protocol::ObjectInfo& player = pkt.players(i);
	}

	Protocol::C_ENTER_GAME EnterGamePkt;
	EnterGamePkt.set_playerindex(0);
	SEND_PACKET(EnterGamePkt);

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (auto* GameInstance = Cast<UMyProjectGameInstance>(GWorld->GetGameInstance())) {
		GameInstance->HandleSpawn(pkt);
	}
	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	if (auto* GameInstance = Cast<UMyProjectGameInstance>(GWorld->GetGameInstance()))
	{
		// TODO : 게임 종료? 로비로?
	}

	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMyProjectGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMyProjectGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDespawn(pkt);
	}
	return true;
}

bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	if (auto* GameInstance = Cast<UMyProjectGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleMove(pkt);
	}

	return true;
}

bool Handle_S_SELECT(PacketSessionRef& session, Protocol::S_SELECT& pkt)
{
	if (pkt.success())
	{
		if (auto* GameInstance = Cast<UMyProjectGameInstance>(GWorld->GetGameInstance()))
		{
			GameInstance->HandleChange();
		}
	}

	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	return true;
}