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
	Info->CopyFrom(*MyPlayer->GetPlayerInfo());
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
		ABaseChar* Player = Cast<APlayerChar>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetPlayerInfo(PlayerInfo.pos_info());
		MyPlayer = Player;
		Players.Add(PlayerInfo.object_id(), Player);
	}
	else
	{
		AOtherPlayerChar* Player = Cast<AOtherPlayerChar>(World->SpawnActor(Cast<UA1GameInstance>(GetGameInstance())->OtherPlayerClass, &SpawnLocation));
		if (Player != nullptr) {

			Player->SetPlayerInfo(PlayerInfo.pos_info());
			Players.Add(PlayerInfo.object_id(), Player);
		}
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

void UA1NetworkManager::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	ABaseChar** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
	Players.Remove(ObjectId);
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
		Cast<AOtherPlayerChar>(Player)->PlayZoom(ZoomPkt.info().b_zoom());
	}
}

ABaseChar* UA1NetworkManager::ValidationPlayer(int ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return nullptr;

	auto* World = GetWorld();
	if (World == nullptr)
		return nullptr;

	ABaseChar** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return nullptr;

	ABaseChar* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return nullptr;

	return Player;
}


