#include "A1NetworkManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "System/A1GameInstance.h"
#include "Character/A1MyPlayer.h"
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

	AA1Character** FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return;
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
	// TODO: 내꺼 나가기 (DB 관련?)

}

void UA1NetworkManager::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	const uint64 ObjectId = MovePkt.info().object_id();

	if (AA1Character* Player = ValidationPlayer(ObjectId))
	{
		const Protocol::PosInfo& Info = MovePkt.info();
		Player->SetPosInfo(Info);
		Player->SetDestInfo(Info);
	}
}

void UA1NetworkManager::HandleState(const Protocol::S_STATE& StatePkt)
{
	const uint64 ObjectId = StatePkt.state().object_id();

	if (AA1Character* Player = ValidationPlayer(ObjectId))
	{
		const Protocol::StateInfo& Info = StatePkt.state();
		Player->SetStateInfo(Info);
	}
}

AA1Character* UA1NetworkManager::ValidationPlayer(int ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return nullptr;

	auto* World = GetWorld();
	if (World == nullptr)
		return nullptr;

	AA1Character** FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return nullptr;

	AA1Character* Player = (*FindActor);
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
		AA1Character* Player = Cast<AA1Character>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetObjectInfo(ObjectInfo);
		Player->SetPosInfo(ObjectInfo.pos_info());
		Player->SetStateInfo(ObjectInfo.state_info());
		
		FVector Location(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());
		Player->SetActorLocation(Location);

		MyPlayer = Player;
		Objects.Add(ObjectInfo.object_id(), Player);
	}
	else
	{
		AA1Character* Player = Cast<AA1Character>(World->SpawnActor(Cast<UA1GameInstance>(GetGameInstance())->OtherPlayerClass, &SpawnLocation));
		if (Player != nullptr) {

			Player->SetObjectInfo(ObjectInfo);
			Player->SetPosInfo(ObjectInfo.pos_info());
			Player->SetStateInfo(ObjectInfo.state_info());


			/*FVector Location(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());
			Player->SetActorLocation(Location);*/

			Objects.Add(ObjectInfo.object_id(), Player);
		}
	}
}