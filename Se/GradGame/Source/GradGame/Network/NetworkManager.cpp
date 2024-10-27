#include "NetworkManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GradGame/GradLogChannels.h"
#include "GradGame/GameModes/GradExperienceManagerComponent.h"
#include "GradGame/GameModes/GradGameModeBase.h"
#include "GradGame/GameModes/GradExperienceDefinition.h"
#include "GradGame/Character/GradPawnData.h"
#include "GradGame/Character/GradCharacter.h"
#include "GradGame/Network/GradNetCharacter.h"
#include "GradNetworkComponent.h"

void UNetworkManager::Deinitialize()
{
	// DisconnectFromGameServer();
}

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

	if (MyPlayer == nullptr) return;
	UGradNetworkComponent* NetComponent = MyPlayer->FindComponentByClass<UGradNetworkComponent>();
	
	if (NetComponent) {
		Protocol::C_LEAVE_GAME LeavePkt;
		Protocol::PosInfo* Info = LeavePkt.mutable_info();
		Info->CopyFrom(*(NetComponent->GetPosInfo()));
		SendPacket(LeavePkt);
	}
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

	TObjectPtr<APawn>* FindActor = Objects.Find(ObjectId);
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
void UNetworkManager::HandleAttack(const Protocol::S_ATTACK& AttackPkt)
{
	const uint64 ObjectId = AttackPkt.info().object_id();
	// TODO: 어택 타입이 아니라 스킬 타입으로 교체
	Protocol::AttackType AttackType = AttackPkt.info().attack_type();

	float camrot_pitch = AttackPkt.info().camrot_pitch();
	float camrot_yaw = AttackPkt.info().camrot_yaw();
	float camrot_roll = AttackPkt.info().camrot_roll();

	float finalcamloc_x = AttackPkt.info().finalcamloc_x();
	float finalcamloc_y = AttackPkt.info().finalcamloc_y();
	float finalcamloc_z = AttackPkt.info().finalcamloc_z();

	FRotator CamRot(camrot_pitch, camrot_yaw, camrot_roll);
	FVector FinalCamLoc(finalcamloc_x, finalcamloc_y, finalcamloc_z);

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	TObjectPtr<APawn>* FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	if (MyPlayer == (*FindActor)) return;
	

	const FGradGameplayTags& GameplayTags = FGradGameplayTags::Get();
	if (AGradNetCharacter* NetPawn = Cast<AGradNetCharacter>(*FindActor))
	{
		NetPawn->WeaponTransform = FTransform(CamRot, FinalCamLoc);
		
		FVector AimDir = CamRot.Vector().GetSafeNormal();

		//DrawDebugLine(GetWorld(), NetPawn->GetActorLocation(), FinalCamLoc, FColor::Red, false, 60.0f, 0, 2.0f);
		//DrawDebugLine(GetWorld(), FinalCamLoc, FinalCamLoc + (AimDir * 1024.0f), FColor::Blue, false, 60.0f, 0, 2.0f);


		switch (AttackType)
		{
			case Protocol::AttackType::ATTACK_TYPE_RIFLE:
			NetPawn->WeaponFire(GameplayTags.InputTag_Weapon_Fire);
			break;
		}
	}
}

void UNetworkManager::HandleReload(const Protocol::S_RELOAD& ReloadkPkt)
{
	const uint64 ObjectId = ReloadkPkt.object_id();
	const bool Success = ReloadkPkt.success();

	if (Success == false) return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	TObjectPtr<APawn>* FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	if (MyPlayer == (*FindActor)) return;

	const FGradGameplayTags& GameplayTags = FGradGameplayTags::Get();
	if (AGradNetCharacter* NetPawn = Cast<AGradNetCharacter>(*FindActor))
	{
		// TODO: 함수 이름 변경
		NetPawn->WeaponFire(GameplayTags.InputTag_Weapon_Reload);
	}
}

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
		TObjectPtr<APawn> Player = PC->GetPawn();
		if (Player == nullptr)
			return;

		if (UGradNetworkComponent* PawnNetComp = Player->FindComponentByClass<UGradNetworkComponent>())
		{
			PawnNetComp->SetObjectInfo(ObjectInfo);
			PawnNetComp->SetPosInfo(ObjectInfo.pos_info());

			Player->SetActorLocation(SpawnLocation);
		}

		MyPlayer = Player;
		Objects.Add(ObjectInfo.object_id(), Player);
	}
	else
	{
		const AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		UGradExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGradExperienceManagerComponent>();
		check(ExperienceManagerComponent);

		UClass* NetClass = ExperienceManagerComponent->CurrentExperience->DefaultPawnData->NetPawnClass;
		TObjectPtr<AGradNetCharacter> Player = Cast<AGradNetCharacter>(World->SpawnActor(NetClass, &SpawnLocation));

		if (UGradNetworkComponent* PawnNetComp = Player->FindComponentByClass<UGradNetworkComponent>())
		{
			PawnNetComp->SetObjectInfo(ObjectInfo);
			PawnNetComp->SetPosInfo(ObjectInfo.pos_info());

			Player->K2_NetOnEquipped();
		}

		Objects.Add(ObjectInfo.object_id(), Player);
	}
}
PRAGMA_ENABLE_OPTIMIZATION