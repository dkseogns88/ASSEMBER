#include "NetworkManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GradGame/GradLogChannels.h"
#include "GradGame/Character/GradCharacter.h"
#include "GradGame/Network/GradNetCharacter.h"
#include "GradNetworkComponent.h"
#include "GradGame/GameModes/GradGameState.h"
#include "GradGame/GameModes/GradExperienceManagerComponent.h"
#include "GradGame/GameModes/GradExperienceDefinition.h"
#include "GradGame/Character/GradPawnData.h"
#include "GradGame/Player/GradPlayerController.h"
#include "GradGame/Cosmetics/GradControllerComponent_CharacterParts.h"
#include "GradGame/Cosmetics/GradPawnComponent_CharacterParts.h"
#include "GradGame/Network/GradNetworkControllerComponent.h"

void UNetworkManager::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);


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

void UNetworkManager::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SendPacket(LeavePkt);
}

void UNetworkManager::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UNetworkManager::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UNetworkManager::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
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

void UNetworkManager::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UNetworkManager::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& object : SpawnPkt.objects())
	{
		HandleSpawn(object, false);
	}
}

void UNetworkManager::HandleDespawn(uint64 ObjectId)
{
	// TODO: 다른 플레이어가 삭제 될 때, UGradControllerComponent_CharacterParts에서 삭제를 시켜줘야 함

}

void UNetworkManager::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
}

void UNetworkManager::HandleLeave(const Protocol::S_LEAVE_GAME& LeavePkt)
{
}

void UNetworkManager::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	const uint64 ObjectId = MovePkt.info().object_id();
	
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	TObjectPtr<ACharacter>* FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	if (MyPlayer == (*FindActor)) return;

	const Protocol::PosInfo& Info = MovePkt.info();
	if (UGradNetworkComponent* PawnNetComp = (*FindActor)->FindComponentByClass<UGradNetworkComponent>())
	{
		PawnNetComp->SetPosInfo(Info);
	}
}

PRAGMA_DISABLE_OPTIMIZATION
void UNetworkManager::SpawnPlayer(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	auto* World = GetWorld();
	if (World == nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		check(PC);
		TObjectPtr<AGradCharacter> Player = Cast<AGradCharacter>(PC->GetPawn());
		if (Player == nullptr)
			return;

		if (UGradNetworkComponent* PawnNetComp = Player->FindComponentByClass<UGradNetworkComponent>())
		{
			PawnNetComp->SetObjectInfo(ObjectInfo);
			PawnNetComp->SetPosInfo(ObjectInfo.pos_info());

			FVector Location(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());
			Player->SetActorLocation(Location);
		}

		MyPlayer = Player;
		Objects.Add(ObjectInfo.object_id(), Player);
	}
	else
	{
		// 내가 플레이하는 컨트롤러 가지고 오기
		APlayerController* PlayerController = Cast<AGradPlayerController>(GetWorld()->GetFirstPlayerController());
		UGradNetworkControllerComponent* NetControllCmp = PlayerController->FindComponentByClass<UGradNetworkControllerComponent>();
		UGradControllerComponent_CharacterParts* ControllCmp = PlayerController->FindComponentByClass<UGradControllerComponent_CharacterParts>();

		check(NetControllCmp);
		check(ControllCmp);

		// 캐릭터 생성
		TObjectPtr<AGradNetCharacter> Player = Cast<AGradNetCharacter>(World->SpawnActor(NetControllCmp->PawnClass, &SpawnLocation));

		UGradPawnComponent_CharacterParts* PawnCustom = Player->FindComponentByClass<UGradPawnComponent_CharacterParts>();
		check(PawnCustom);

		// 캐릭터 메쉬 부착
		ControllCmp->K2_AddPartsNetwork(PawnCustom);

		// 캐릭터 무기와 애니메이션 설정
		Player->K2_NetOnEquipped();

		// 네트워크 변수들 생성
		if (UGradNetworkComponent* PawnNetComp = Player->FindComponentByClass<UGradNetworkComponent>())
		{
			PawnNetComp->CreateVariables();
			PawnNetComp->SetObjectInfo(ObjectInfo);
			PawnNetComp->SetPosInfo(ObjectInfo.pos_info());
		}

		// 오브젝트 추가
		Objects.Add(ObjectInfo.object_id(), Player);
	}
}
PRAGMA_ENABLE_OPTIMIZATION