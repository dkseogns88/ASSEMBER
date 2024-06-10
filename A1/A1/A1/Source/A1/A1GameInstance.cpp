// Fill out your copyright notice in the Description page of Project Settings.


#include "A1GameInstance.h"
#include "Sockets.h"
#include "Engine/World.h"
#include "Characters/Monster.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Characters/PlayerChar.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Characters/BaseChar.h"
#include "Objects/A1PlayerController.h"
#include "Objects/AnimInstanceCustom.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/StaticMeshActor.h"



UA1GameInstance::UA1GameInstance()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

}


//
//bool UA1GameInstance::ConnectToGameServer()
//{
//
//	FIPv4Address Ip;
//	FIPv4Address::Parse(IpAddress, Ip);
//
//	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
//	InternetAddr->SetIp(Ip.Value);
//	InternetAddr->SetPort(Port);
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));
//
//	Connected = Socket->Connect(*InternetAddr);
//
//	
//	if (Connected)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));
//
//		GameServerSession = MakeShared<PacketSession>(Socket);
//		GameServerSession->Run();
//
//		Protocol::C_LOGIN Pkt;
//		SendBufferRef SendBuffer = ServerPacketHandler::MakeSendBuffer(Pkt);
//		SendPacket(SendBuffer);
//	}
//
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
//
//	}
//	return Connected;
//	
//}

//void UA1GameInstance::DisconnectToGameServer()
//{
//
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//	
//	Protocol::C_LEAVE_GAME LeavePkt;
//	Protocol::PosInfo* Info = LeavePkt.mutable_info();
//	Info->CopyFrom(*MyPlayer->GetPlayerInfo());
//	SEND_PACKET(LeavePkt);
//	
//}

//void UA1GameInstance::HandleRecvPackets()
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	GameServerSession->HandleRecvPackets();
//}

//void UA1GameInstance::SendPacket(SendBufferRef SendBuffer)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	GameServerSession->SendPacket(SendBuffer);
//}

//void UA1GameInstance::HandleSpawn(const Protocol::ObjectInfo& objectInfo, bool IsMine)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return;
//
//	const uint64 ObjectId = objectInfo.object_id();
//	if (Players.Find(ObjectId) != nullptr)
//		return;
//
//	FVector SpawnLocation(objectInfo.pos_info().x(), objectInfo.pos_info().y(), objectInfo.pos_info().z());
//
//	if (IsMine)
//	{
//		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
//		APlayerChar* Player = Cast<APlayerChar>(PC->GetPawn());
//		if (Player == nullptr)
//			return;
//
//		Player->SetPlayerInfo(objectInfo.pos_info());
//		MyPlayer = Player;
//		Players.Add(objectInfo.object_id(), Player);
//	}
//	else
//	{
//		APlayerChar* Player = Cast<APlayerChar>(World->SpawnActor(OtherPlayerClassRinty, &SpawnLocation));
//		Player->SetPlayerInfo(objectInfo.pos_info());
//		Players.Add(objectInfo.object_id(), Player);
//	}
//
//}


//void UA1GameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
//{
//	HandleSpawn(EnterGamePkt.player(), true);
//}


//
//void UA1GameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
//{
//	for (auto& Player : SpawnPkt.players())
//	{
//		HandleSpawn(Player, false);
//	}
//}


//
//void UA1GameInstance::HandleDespawn(uint64 ObjectId)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return;
//
//	APlayerChar** FindActor = Players.Find(ObjectId);
//	if (FindActor == nullptr)
//		return;
//
//	World->DestroyActor(*FindActor);
//	Players.Remove(ObjectId);
//}


//void UA1GameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
//{
//	for (auto& ObjectId : DespawnPkt.object_ids())
//	{
//		HandleDespawn(ObjectId);
//	}
//}

//void UA1GameInstance::HandleLeave(const Protocol::S_LEAVE_GAME& LeavePkt)
//{
//	// 
//
//
//}

//void UA1GameInstance::HandleSelectType(const Protocol::S_SELECT& SelectPkt)
//{
//	if (SelectPkt.success() == true) {
//		const uint64 ObjectId = SelectPkt.info().object_id();
//
//		APlayerChar** FindActor = Players.Find(ObjectId);
//		if (FindActor == nullptr)
//			return;
//
//		APlayerChar* Player = (*FindActor);
//
//		 //내 플레이어
//		if (Player->IsMyPlayer())
//		{
//
//			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
//			if (PlayerController)
//			{
//				APlayerChar* CurrentPawn = Cast<APlayerChar>(PlayerController->GetPawn());
//				FVector Location = FVector(SelectPkt.info().x(), SelectPkt.info().y(), SelectPkt.info().z());
//				FRotator Rotation = FRotator(0.f, SelectPkt.info().yaw(), 0.f);
//
//				if (SelectPkt.player_type() == Protocol::PLAYER_TYPE_RINTY) {
//
//					APlayerChar* NewCharacter = GetWorld()->SpawnActor<APlayerChar>(BPClassRinty, Location, Rotation);
//					if (NewCharacter) {
//						PlayerController->Possess(NewCharacter);
//						CurrentPawn->Destroy();
//						Players.Remove(SelectPkt.info().object_id());
//
//						NewCharacter->SetPlayerInfo(SelectPkt.info());
//						Players.Add(SelectPkt.info().object_id(), NewCharacter);
//
//					}
//				}
//				else if (SelectPkt.player_type() == Protocol::PLAYER_TYPE_SIDA) {
//
//					APlayerChar* NewCharacter = GetWorld()->SpawnActor<APlayerChar>(BPClassSida, Location, Rotation);
//					if (NewCharacter) {
//						PlayerController->Possess(NewCharacter);
//						CurrentPawn->Destroy();
//						Players.Remove(SelectPkt.info().object_id());
//
//						NewCharacter->SetPlayerInfo(SelectPkt.info());
//						Players.Add(SelectPkt.info().object_id(), NewCharacter);
//					}
//				}
//			}
//			return;
//		}
//
//		if (!Player->IsMyPlayer()) {
//
//			APlayerChar* CurrentPawn = Player;
//			FVector Location = FVector(SelectPkt.info().x(), SelectPkt.info().y(), SelectPkt.info().z());
//			FRotator Rotation = FRotator(0.f, SelectPkt.info().yaw(), 0.f);
//
//			if (SelectPkt.player_type() == Protocol::PLAYER_TYPE_RINTY) {
//
//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("RINTY")));
//				APlayerChar* NewCharacter = GetWorld()->SpawnActor<APlayerChar>(OtherPlayerClassRinty, Location, Rotation);
//				if (NewCharacter) {
//					CurrentPawn->Destroy();
//					Players.Remove(SelectPkt.info().object_id());
//
//					NewCharacter->SetPlayerInfo(SelectPkt.info());
//					Players.Add(SelectPkt.info().object_id(), NewCharacter);
//				}
//			}
//
//			else if (SelectPkt.player_type() == Protocol::PLAYER_TYPE_SIDA) {
//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SIDA")));
//
//				APlayerChar* NewCharacter = GetWorld()->SpawnActor<APlayerChar>(OtherPlayerClassSida, Location, Rotation);
//				if (NewCharacter) {
//					CurrentPawn->Destroy();
//					Players.Remove(SelectPkt.info().object_id());
//
//					NewCharacter->SetPlayerInfo(SelectPkt.info());
//					Players.Add(SelectPkt.info().object_id(), NewCharacter);
//				}
//			}
//		}
//	}
//}

//void UA1GameInstance::HandleTelePort(const Protocol::S_TELEPORT& TelePortPkt)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return;
//
//	const uint64 ObjectId = TelePortPkt.info().object_id();
//	APlayerChar** FindActor = Players.Find(ObjectId);
//	if (FindActor == nullptr)
//		return;
//
//	APlayerChar* Player = (*FindActor);
//
//	const Protocol::PosInfo& Info = TelePortPkt.info();
//	Player->SetDestInfo(Info);
//	Player->SetActorLocation(
//		FVector(TelePortPkt.info().x(), TelePortPkt.info().y(), TelePortPkt.info().z())
//	);
//
//
//}

//void UA1GameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
//{
//	const uint64 ObjectId = MovePkt.info().object_id();
//
//	if (APlayerChar* Player = ValidationPlayer(ObjectId))
//	{
//		const Protocol::PosInfo& Info = MovePkt.info();
//		Player->SetDestInfo(Info);
//	}
//	else if (AMonster* monster = ValidationMonster(ObjectId))
//	{
//		const Protocol::PosInfo& Info = MovePkt.info();
//		monster->SetDestInfo(Info);
//	}
//}

//void UA1GameInstance::HandleJump(const Protocol::S_JUMP& JumpPkt)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return;
//
//	const uint64 ObjectId = JumpPkt.info().object_id();
//	APlayerChar** FindActor = Players.Find(ObjectId);
//	if (FindActor == nullptr)
//		return;
//
//	APlayerChar* Player = (*FindActor);
//	if (Player->IsMyPlayer())
//		return;
//
//	const Protocol::PosInfo& Info = JumpPkt.info();
//	Player->SetDestInfo(Info);
//}

//void UA1GameInstance::HandleZoom(const Protocol::S_ZOOM& ZoomPkt)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return;
//
//	const uint64 ObjectId = ZoomPkt.info().object_id();
//	APlayerChar** FindActor = Players.Find(ObjectId);
//	if (FindActor == nullptr)
//		return;
//
//	APlayerChar* Player = (*FindActor);
//	if (Player->IsMyPlayer())
//		return;
//
//
//	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(Player->GetMesh()->GetAnimInstance());
//	if (AnimInstance)
//	{
//		// 조준 상태를 업데이트 합니다.
//		bool bIsAiming = ZoomPkt.info().b_zoom();  // 패킷에서 조준 정보를 가져옵니다.
//		AnimInstance->SetAiming(bIsAiming);
//
//
//		UE_LOG(LogTemp, Log, TEXT("Zoom animation updated for player %llu, aiming: %s"), ObjectId, bIsAiming ? TEXT("True") : TEXT("False"));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UAnimInstanceCustom for player %llu"), ObjectId);
//	}
//}

//void UA1GameInstance::HandleRoll(const Protocol::S_ROLL& RollPkt)
//{
//
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return;
//
//	const uint64 ObjectId = RollPkt.info().object_id();
//	APlayerChar** FindActor = Players.Find(ObjectId);
//	if (FindActor == nullptr)
//		return;
//
//	APlayerChar* Player = (*FindActor);
//	if (Player->IsMyPlayer())
//		return;
//
//	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(Player->GetMesh()->GetAnimInstance());
//	if (AnimInstance)
//	{
//		if (Protocol::MoveState::MOVE_STATE_ROOL == RollPkt.info().state()) {
//			Player->StartRoll(RollPkt.forwardinput(), RollPkt.rightinput());
//		}
//	}
//
//}


//void UA1GameInstance::HandleMonsterSpawn(const Protocol::S_SPAWN_MONSTER& SpawnPkt)
//{
//	for (const auto& MonsterInfo : SpawnPkt.monsters())
//	{
//		HandleMonsterSpawn(MonsterInfo);
//	}
//
//}

//void UA1GameInstance::HandleMonsterSpawn(const Protocol::ObjectInfo& MonsterInfo)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	UWorld* World = GetWorld();
//	if (World == nullptr)
//		return;
//
//	FVector Location(MonsterInfo.pos_info().x(), MonsterInfo.pos_info().y(), MonsterInfo.pos_info().z());
//	FRotator Rotation(0.0f, 0.0f, 0.0f);
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	// MonsterClass를 사용하여 몬스터 스폰
//	AMonster* SpawnedMonster = World->SpawnActor<AMonster>(MonsterClass, Location, Rotation, SpawnParams);
//	if (SpawnedMonster)
//	{
//		// 필요에 따라 MonsterInfo의 속성으로 설정
//		SpawnedMonster->SetActorEnableCollision(true);
//		// 스폰된 몬스터 정보를 monsters 맵에 추가
//
//		SpawnedMonster->SetMonsterInfo(MonsterInfo.pos_info());
//
//		Monsters.Add(MonsterInfo.object_id(), SpawnedMonster);
//		UE_LOG(LogTemp, Log, TEXT("Monster spawned successfully at %s"), *Location.ToString());
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Failed to spawn monster at %s"), *Location.ToString());
//	}
//}
//
//void UA1GameInstance::HandleHIT(const Protocol::S_HIT& pkt)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return;
//	bool OnHit = pkt.on_hit();
//	int HitId = pkt.object_id();
//	if (OnHit)
//	{
//		AMonster** FindActor = Monsters.Find(HitId);
//		if (FindActor == nullptr) return;
//
//		if (AMonster* Enemy = Cast<AMonster>(*FindActor))
//		{
//
//			// Handle AEnemy1-specific logic
//			Enemy->TakeDamaged();
//			Enemy->Health -= 20;
//			if (Enemy->Health <= 0)
//			{
//				Enemy->Die();
//			}
//		}
//	}
//
//}
//
//void UA1GameInstance::HandleAttack(const Protocol::S_ATTACK& pkt)
//{
//
//	return;
//}
//
//void UA1GameInstance::SpawnMonsterAtLocation(const Protocol::PosInfo& Info)
//{
//	UWorld* World = GetWorld();
//	if (World == nullptr)
//	{
//		UE_LOG(LogTemp, Error, TEXT("World is null"));
//		return;
//	}
//
//
//	FRotator Rotation(0.0f, 0.0f, 0.0f);
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//
//
//	FVector Location = FVector(Info.x(), Info.y(), Info.z());
//
//
//	// 스폰할 몬스터의 클래스를 지정
//	AMonster* SpawnedMonster = World->SpawnActor<AMonster>(MonsterClass, Location, Rotation, SpawnParams);
//	if (SpawnedMonster)
//	{
//		UE_LOG(LogTemp, Log, TEXT("Monster spawned successfully at %s"), *Location.ToString());
//
//		// 초기화 여부 확인
//		if (SpawnedMonster->MonsterInfo == nullptr)
//		{
//			SpawnedMonster->MonsterInfo = new Protocol::PosInfo();
//			UE_LOG(LogTemp, Warning, TEXT("MonsterInfo was null, created new instance"));
//		}
//
//		if (SpawnedMonster->DestInfo == nullptr)
//		{
//			SpawnedMonster->DestInfo = new Protocol::PosInfo();
//			UE_LOG(LogTemp, Warning, TEXT("DestInfo was null, created new instance"));
//		}
//
//		SpawnedMonster->SetActorEnableCollision(true);
//		Monsters.Add(Info.object_id(), SpawnedMonster);
//
//		if (Info.object_id() != 0)
//		{
//			assert(PlayerInfo->object_id() == Info.object_id());
//		}
//
//		// 안전한 메모리 접근
//		if (SpawnedMonster->MonsterInfo)
//		{
//			SpawnedMonster->MonsterInfo->CopyFrom(Info);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("MonsterInfo is still null"));
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Failed to spawn monster at %s"), *Location.ToString());
//	}
//}

void UA1GameInstance::Init()
{
	Super::Init();


	//MonsterClass = AEnemy1::StaticClass();


	// 캐릭터 클래스 매핑 초기화
	CharacterBlueprintPaths.Add("Rinty", "Blueprint'/Game/BP/BP_Rinty.BP_Rinty_C'");
	

	//스폰안정화를위해 월드 완전히생성후 텀을두어 몬스터소환
	//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UMyProjectGameInstance::SpawnNPC, 1.0f, false);


}



UClass* UA1GameInstance::GetCharacterClass(const FString& CharacterName) const
{
	const FString* FoundPath = CharacterBlueprintPaths.Find(CharacterName);
	if (FoundPath)
	{
		FString AssetPath = *FoundPath + _TEXT("_C"); // Add "_C" to the path to get the UClass path
		UE_LOG(LogTemp, Log, TEXT("Attempting to load class at path: %s"), *AssetPath);
		UClass* BlueprintClass = StaticLoadClass(UObject::StaticClass(), nullptr, *AssetPath);
		if (BlueprintClass)
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully loaded class for: %s"), *CharacterName);
			return BlueprintClass;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load class at path: %s"), *AssetPath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No path found for character name: %s"), *CharacterName);
	}
	return nullptr;
}

void UA1GameInstance::LogCharacterChange(int32 PlayerIndex, const FString& NewCharacterName)
{
	FPlayerCharacterChangeInfo ChangeInfo(PlayerIndex, NewCharacterName);
	PlayerCharacterChangeLog.Add(PlayerIndex, NewCharacterName);


	//캐릭터 변경에대한 정보를 로그로 출력
	UE_LOG(LogTemp, Log, TEXT("Player %d changed to character: %s"), PlayerIndex, *NewCharacterName);
	for (const TPair<int32, FString>& Info : PlayerCharacterChangeLog)
	{
		UE_LOG(LogTemp, Log, TEXT("Log - Player %d: %s"), Info.Key, *Info.Value);
	}
}

//APlayerChar* UA1GameInstance::ValidationPlayer(int ObjectId)
//{
//
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return nullptr;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return nullptr;
//
//	APlayerChar** FindActor = Players.Find(ObjectId);
//	if (FindActor == nullptr)
//		return nullptr;
//
//	APlayerChar* Player = (*FindActor);
//	if (Player->IsMyPlayer())
//		return nullptr;
//
//	return Player;
//}
//
//AMonster* UA1GameInstance::ValidationMonster(int ObjectId)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return nullptr;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return nullptr;
//
//	AMonster** FindActor = Monsters.Find(ObjectId);
//	if (FindActor == nullptr)
//		return nullptr;
//
//	return *FindActor;
//
//}



