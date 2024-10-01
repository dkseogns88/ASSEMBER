#include "A1NetworkManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "System/A1GameInstance.h"
#include "Character/A1MyPlayer.h"
#include "Character/A1OtherPlayer.h"
#include "Character/A1Monster.h"
#include "Kismet/GameplayStatics.h"
#include "A1LogChannels.h"

void UA1NetworkManager::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();
		
		// TEMP : Lobby에서 캐릭터 선택창 등
		{
			Protocol::C_LOGIN Pkt;
			SendBufferRef SendBuffer = ServerPacketHandler::MakeSendBuffer(Pkt);
			SendPacket(SendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UA1NetworkManager::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SendPacket(LeavePkt);

}

void UA1NetworkManager::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UA1NetworkManager::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UA1NetworkManager::HandleSpawn(const Protocol::ObjectInfo& PlayerInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = PlayerInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(PlayerInfo.pos_info().x(), PlayerInfo.pos_info().y(), PlayerInfo.pos_info().z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		AA1MyPlayer* Player = Cast<AA1MyPlayer>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetPlayerInfo(PlayerInfo.pos_info());
		MyPlayer = Player;
		Players.Add(PlayerInfo.object_id(), Player);
	}
	else
	{
		AA1OtherPlayer* Player = Cast<AA1OtherPlayer>(World->SpawnActor(Cast<UA1GameInstance>(GetGameInstance())->OtherPlayerClass, &SpawnLocation));
		Player->SetPlayerInfo(PlayerInfo.pos_info());
		Players.Add(PlayerInfo.object_id(), Player);
	}
}

void UA1NetworkManager::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UA1NetworkManager::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
		for (auto& Player : SpawnPkt.players())
	{
		HandleSpawn(Player, false);
	}
}

void UA1NetworkManager::HandleMonsterSpawn(const Protocol::S_SPAWN_MONSTER& SpawnPkt)
{
	for (auto& monster : SpawnPkt.monsters())
	{
		HandleMonsterSpawn(monster);
	}

}

void UA1NetworkManager::HandleMonsterSpawn(const Protocol::ObjectInfo& MonsterInfo)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MonsterInfo.object_id();
	if (Monsters.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(MonsterInfo.pos_info().x(), MonsterInfo.pos_info().y(), MonsterInfo.pos_info().z());
	FRotator SpawnRotator(0.0, (MonsterInfo.pos_info().yaw()), 0.0);

	{
		AA1Monster* Monster = Cast<AA1Monster>(World->SpawnActor(Cast<UA1GameInstance>(GetGameInstance())->MonsterClass, &SpawnLocation, &SpawnRotator));
		Monster->SetPlayerInfo(MonsterInfo.pos_info());
		Monsters.Add(MonsterInfo.object_id(), Monster);
	}

}

void UA1NetworkManager::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AA1Character** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
}

void UA1NetworkManager::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void UA1NetworkManager::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	const uint64 ObjectId = MovePkt.info().object_id();

	if (AA1Character* Player = ValidationPlayer(ObjectId))
	{
		const Protocol::PosInfo& Info = MovePkt.info();
		Player->SetDestInfo(Info);
	}
	else if (AA1Character* monster = ValidationMonster(ObjectId))
	{
		const Protocol::PosInfo& Info = MovePkt.info();
		monster->SetDestInfo(Info);
	}
}

void UA1NetworkManager::HandleJump(const Protocol::S_JUMP& JumpPkt)
{
	const uint64 ObjectId = JumpPkt.info().object_id();
	
	if (AA1Character* Player = ValidationPlayer(ObjectId))
	{
		const Protocol::PosInfo& Info = JumpPkt.info();
		Player->SetDestInfo(Info);
	}

}

void UA1NetworkManager::HandleZoom(const Protocol::S_ZOOM& ZoomPkt)
{
	const uint64 ObjectId = ZoomPkt.info().object_id();

	if (AA1Character* Player = ValidationPlayer(ObjectId))
	{
		Cast<AA1OtherPlayer>(Player)->PlayZoom(ZoomPkt.info().b_zoom());
	}

}

void UA1NetworkManager::HandleNpcMove(const Protocol::S_NPCMOVE& NpcMovePkt)
{
	const uint64 ObjectId = NpcMovePkt.info().object_id();

	if (AA1Character* monster = ValidationMonster(ObjectId))
	{
		const Protocol::PosInfo& Info = NpcMovePkt.info();
		UE_LOG(LogTemp, Warning, TEXT("DestInfo : (%f,%f,%f)"), Info.x(), Info.y(), Info.z());

		monster->SetDestInfo(Info);
	}
	
}

void UA1NetworkManager::HandlePathFinding(const Protocol::S_PATHFINDING& pathPkt)
{
	UE_LOG(LogA1, Warning, TEXT("HandlePathFinding"));

	for (auto path : pathPkt.info())
	{
		UE_LOG(LogA1, Warning, TEXT("(%f,%f,%f)"), path.x(), path.y(), path.z());
	}
}

void UA1NetworkManager::TestFunc()
{
	Protocol::C_PATHFINDING pkt;
	Protocol::PosInfo* startInfo = pkt.add_info();
	startInfo->set_x(-2000.f);
	startInfo->set_y(-2000.f);
	startInfo->set_z(0.f);
	startInfo->set_state(Protocol::MoveState::MOVE_STATE_RUN);
	startInfo->set_object_id(0);
	startInfo->set_yaw(0.f);

	Protocol::PosInfo* goalInfo = pkt.add_info();
	goalInfo->set_x(-900.f);
	goalInfo->set_y(-900.f);
	goalInfo->set_z(0.f);
	goalInfo->set_state(Protocol::MoveState::MOVE_STATE_RUN);
	goalInfo->set_object_id(1);
	goalInfo->set_yaw(0.f);

	SendPacket(pkt);
}

AA1Character* UA1NetworkManager::ValidationPlayer(int ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return nullptr;

	auto* World = GetWorld();
	if (World == nullptr)
		return nullptr;

	AA1Character** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return nullptr;

	AA1Character* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return nullptr;

	return Player;
}

AA1Character* UA1NetworkManager::ValidationMonster(int ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return nullptr;

	auto* World = GetWorld();
	if (World == nullptr)
		return nullptr;

	AA1Character** FindActor = Monsters.Find(ObjectId);
	if (FindActor == nullptr)
		return nullptr;

	return *FindActor;
}
