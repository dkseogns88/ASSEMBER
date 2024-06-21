#include "A1NetworkManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "A1GameInstance.h"
#include "Characters/PlayerChar.h"
#include "Characters/OtherPlayerChar.h"
#include "Kismet/GameplayStatics.h"

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
	Protocol::PosInfo* Info = LeavePkt.mutable_info();
	Info->CopyFrom(*MyPlayer->GetPosInfo());
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

void UA1NetworkManager::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const uint64 ObjectId = ObjectInfo.object_id();
	if (Objects.Find(ObjectId) != nullptr)
		return;
	
	switch (ObjectInfo.object_type())
	{
	case Protocol::ObjectType::OBJECT_TYPE_PLAYER:
		SpawnPlayer(ObjectInfo, IsMine);
		break;
	case Protocol::ObjectType::OBJECT_TYPE_MONSTER:
		SpawnMonster(ObjectInfo);
		break;
	default:
		break;
	}

}

void UA1NetworkManager::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UA1NetworkManager::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& object : SpawnPkt.objects())
	{
		HandleSpawn(object, false);
	}
}

void UA1NetworkManager::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	ABaseChar** FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
	Objects.Remove(ObjectId);
}

void UA1NetworkManager::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void UA1NetworkManager::HandleLeave(const Protocol::S_LEAVE_GAME& LeavePkt)
{
	// TODO: 내꺼 나가기
}

void UA1NetworkManager::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	const uint64 ObjectId = MovePkt.info().object_id();

	if (ABaseChar* Player = ValidationPlayer(ObjectId))
	{
		const Protocol::PosInfo& Info = MovePkt.info();
		Player->SetDestInfo(Info);
	}
}

void UA1NetworkManager::HandleZoom(const Protocol::S_ZOOM& ZoomPkt)
{
	const uint64 ObjectId = ZoomPkt.info().object_id();

	if (ABaseChar* Player = ValidationPlayer(ObjectId))
	{
		Cast<AOtherPlayerChar>(Player)->SetAiming(ZoomPkt.info().b_zoom());
	}
}

void UA1NetworkManager::HandleAttack(const Protocol::S_ATTACK& AttackPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	// TODO: 다른 플레이어의 스킬 애니메이션 보여주기
	AttackPkt.attack_info();

	{
		const uint64 ObjectId = AttackPkt.stat_info().object_id();
		ABaseChar** FindActor = Objects.Find(ObjectId);
		if (FindActor == nullptr) return;

		ABaseChar* Object = *FindActor;
		Object->SetStatInfo(AttackPkt.stat_info());
	}
}

ABaseChar* UA1NetworkManager::ValidationPlayer(int ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return nullptr;

	auto* World = GetWorld();
	if (World == nullptr)
		return nullptr;

	ABaseChar** FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return nullptr;

	ABaseChar* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return nullptr;

	return Player;
}

void UA1NetworkManager::SpawnPlayer(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	auto* World = GetWorld();
	if (World == nullptr)
		return; 
	
	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		ABaseChar* Player = Cast<APlayerChar>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetPosInfo(ObjectInfo.pos_info());
		Player->SetStatInfo(ObjectInfo.stat_info());
		MyPlayer = Player;
		Objects.Add(ObjectInfo.object_id(), Player);
	}
	else
	{
		AOtherPlayerChar* Player = Cast<AOtherPlayerChar>(World->SpawnActor(Cast<UA1GameInstance>(GetGameInstance())->OtherPlayerClass, &SpawnLocation));
		if (Player != nullptr) {

			Player->SetPosInfo(ObjectInfo.pos_info());
			Player->SetStatInfo(ObjectInfo.stat_info());
			Objects.Add(ObjectInfo.object_id(), Player);
		}
	}
}

void UA1NetworkManager::SpawnMonster(const Protocol::ObjectInfo& ObjectInfo)
{
	auto* World = GetWorld();
	if (World == nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());
	TSubclassOf<AMonster> MonsterClass;

	if (ObjectInfo.monster_type() == Protocol::MonsterType::MONSTER_TYPE_FANATIC)
		MonsterClass = Cast<UA1GameInstance>(GetGameInstance())->BPClassFanatic;
	else if (ObjectInfo.monster_type() == Protocol::MonsterType::MONSTER_TYPE_MONK)
		MonsterClass = Cast<UA1GameInstance>(GetGameInstance())->BPClassMonk;

	AMonster* Monster = Cast<AMonster>(World->SpawnActor(MonsterClass, &SpawnLocation));
	if (Monster)
	{
		Monster->SetPosInfo(ObjectInfo.pos_info());
		Monster->SetStatInfo(ObjectInfo.stat_info());
		Objects.Add(ObjectInfo.object_id(), Monster);
	}

}