// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectGameInstance.h"
#include "Sockets.h"
#include "Engine/World.h"
#include "Enemy1.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "MyProjectPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "MyProjectMyPlayer.h"
#include "MyProjectMyPlayerSida.h"
#include "MyProjectPlayerController.h"
#include "AnimInstanceCustom.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/StaticMeshActor.h"

UMyProjectGameInstance::UMyProjectGameInstance()
{
	


}


void UMyProjectGameInstance::ConnectToGameServer()
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
	
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		Protocol::C_LOGIN Pkt;
		SendBufferRef SendBuffer = ServerPacketHandler::MakeSendBuffer(Pkt);
		SendPacket(SendBuffer);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UMyProjectGameInstance::DisconnectToGameServer()
{

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);

}

void UMyProjectGameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UMyProjectGameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UMyProjectGameInstance::HandleSpawn(const Protocol::ObjectInfo& objectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = objectInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(objectInfo.pos_info().x(), objectInfo.pos_info().y(), objectInfo.pos_info().z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		AMyProjectPlayer* Player = Cast<AMyProjectPlayer>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetPlayerInfo(objectInfo.pos_info());
		MyPlayer = Player;
		Players.Add(objectInfo.object_id(), Player);
	}
	else
	{
		AMyProjectPlayer* Player = Cast<AMyProjectPlayer>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
		Player->SetPlayerInfo(objectInfo.pos_info());
		Players.Add(objectInfo.object_id(), Player);
	}

}

void UMyProjectGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UMyProjectGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& Player : SpawnPkt.players())
	{
		HandleSpawn(Player, false);
	}
}

void UMyProjectGameInstance::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AMyProjectPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
}

void UMyProjectGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void UMyProjectGameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	const uint64 ObjectId = MovePkt.info().object_id();

	if (AMyProjectPlayer* Player = ValidationPlayer(ObjectId))
	{
		const Protocol::PosInfo& Info = MovePkt.info();
		Player->SetDestInfo(Info);
	}
	else if (ANPC* monster = ValidationMonster(ObjectId))
	{
		const Protocol::PosInfo& Info = MovePkt.info();
		monster->SetDestInfo(Info);
	}
}

void UMyProjectGameInstance::HandleJump(const Protocol::S_JUMP& JumpPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = JumpPkt.info().object_id();
	AMyProjectPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AMyProjectPlayer* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return;

	const Protocol::PosInfo& Info = JumpPkt.info();
	Player->SetDestInfo(Info);
}

void UMyProjectGameInstance::HandleZoom(const Protocol::S_ZOOM& ZoomPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = ZoomPkt.info().object_id();
	AMyProjectPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AMyProjectPlayer* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return;

	
	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(Player->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		// 조준 상태를 업데이트 합니다.
		bool bIsAiming = ZoomPkt.info().b_zoom();  // 패킷에서 조준 정보를 가져옵니다.
		AnimInstance->SetAiming(bIsAiming);
		

		UE_LOG(LogTemp, Log, TEXT("Zoom animation updated for player %llu, aiming: %s"), ObjectId, bIsAiming ? TEXT("True") : TEXT("False"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UAnimInstanceCustom for player %llu"), ObjectId);
	}
}

void UMyProjectGameInstance::HandleMonsterSpawn(const Protocol::S_SPAWN_MONSTER& SpawnPkt)
{
	for (const auto& MonsterInfo : SpawnPkt.monsters())
	{
		HandleMonsterSpawn(MonsterInfo);
	}

}

void UMyProjectGameInstance::HandleMonsterSpawn(const Protocol::ObjectInfo& MonsterInfo)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	UWorld* World = GetWorld();
	if (World == nullptr)
		return;

	FVector Location(MonsterInfo.pos_info().x(), MonsterInfo.pos_info().y(), MonsterInfo.pos_info().z());
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// MonsterClass를 사용하여 몬스터 스폰
	AEnemy1* SpawnedMonster = World->SpawnActor<AEnemy1>(MonsterClass, Location, Rotation, SpawnParams);
	if (SpawnedMonster)
	{
		// 필요에 따라 MonsterInfo의 속성으로 설정
		SpawnedMonster->SetActorEnableCollision(true);
		// 스폰된 몬스터 정보를 monsters 맵에 추가

		SpawnedMonster->SetMonsterInfo(MonsterInfo.pos_info());

		Monsters.Add(MonsterInfo.object_id(), SpawnedMonster);
		UE_LOG(LogTemp, Log, TEXT("Monster spawned successfully at %s"), *Location.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn monster at %s"), *Location.ToString());
	}

	/*if (MonsterInfo.monster_type() == Protocol::MONSTER_TYPE_TEST) {
		SpawnMonsterAtLocation(MonsterInfo.pos_info());
	}*/
}
// 이부분 서버처리 수정해야함

void UMyProjectGameInstance::HandleHIT(const Protocol::S_HIT& pkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	bool OnHit = pkt.on_hit();
	int HitId = pkt.object_id();

	if (OnHit)
	{
		ANPC** FindActor = Monsters.Find(HitId);
		if (FindActor == nullptr) return;
		
		if (ANPC* Enemy = Cast<ANPC>(*FindActor))
		{
			
			// Handle AEnemy1-specific logic
			Enemy->TakeDamage();
			Enemy->Health -= 20;
			if (Enemy->Health <= 0)
			{
				Enemy->Die();
			}
		}
	}

}

void UMyProjectGameInstance::HandleAttack(const Protocol::S_ATTACK& pkt)
{
	GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Attack!!")));
	
	return ;
}

void UMyProjectGameInstance::SpawnMonsterAtLocation (const Protocol::PosInfo& Info)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null"));
		return;
	}


	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	

	FVector Location = FVector(Info.x(), Info.y(), Info.z());


	// 스폰할 몬스터의 클래스를 지정
	ANPC* SpawnedMonster = World->SpawnActor<ANPC>(MonsterClass, Location, Rotation, SpawnParams);
	if (SpawnedMonster)
	{
		UE_LOG(LogTemp, Log, TEXT("Monster spawned successfully at %s"), *Location.ToString());

		// 초기화 여부 확인
		if (SpawnedMonster->MonsterInfo == nullptr)
		{
			SpawnedMonster->MonsterInfo = new Protocol::PosInfo();
			UE_LOG(LogTemp, Warning, TEXT("MonsterInfo was null, created new instance"));
		}

		if (SpawnedMonster->DestInfo == nullptr)
		{
			SpawnedMonster->DestInfo = new Protocol::PosInfo();
			UE_LOG(LogTemp, Warning, TEXT("DestInfo was null, created new instance"));
		}

		SpawnedMonster->SetActorEnableCollision(true);
		Monsters.Add(Info.object_id(), SpawnedMonster);

		if (Info.object_id() != 0)
		{
			assert(PlayerInfo->object_id() == Info.object_id());
		}

		// 안전한 메모리 접근
		if (SpawnedMonster->MonsterInfo)
		{
			SpawnedMonster->MonsterInfo->CopyFrom(Info);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MonsterInfo is still null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn monster at %s"), *Location.ToString());
	}
}

void UMyProjectGameInstance::Init()
{
	Super::Init();

	
	//MonsterClass = AEnemy1::StaticClass();
	
	
	// 캐릭터 클래스 매핑 초기화
	CharacterBlueprintPaths.Add("Rinty", "Blueprint'/Game/MyBP/BP_Class/BP_MyPlayer.BP_MyPlayer_C'");
	CharacterBlueprintPaths.Add("Sida", "Blueprint'/Game/MyBP/BP_Class/BP_MyPlayer_sida.BP_MyPlayer_sida_C'");

	//스폰안정화를위해 월드 완전히생성후 텀을두어 몬스터소환
	//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UMyProjectGameInstance::SpawnNPC, 1.0f, false);


}

void UMyProjectGameInstance::SpawnNPC()
{
	// AEnemy1 스폰 위치 설정
	//FVector MonsterSpawnLocation1 = FVector(0.0f, 0.0f, 150.0f);
	//SpawnMonsterAtLocation(MonsterClass1, MonsterSpawnLocation1);
	//UE_LOG(LogTemp, Log, TEXT("AEnemy1 Spawned at %s"), *MonsterSpawnLocation1.ToString());

	// AEnemy2 스폰 위치 설정
	//FVector MonsterSpawnLocation2 = FVector(200.0f, 200.0f, 150.0f);
	//SpawnMonsterAtLocation(MonsterClass2, MonsterSpawnLocation2);
	//UE_LOG(LogTemp, Log, TEXT("AEnemy2 Spawned at %s"), *MonsterSpawnLocation2.ToString());

}

UClass* UMyProjectGameInstance::GetCharacterClass(const FString& CharacterName) const
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


void UMyProjectGameInstance::LogCharacterChange(int32 PlayerIndex, const FString& NewCharacterName)
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

AMyProjectPlayer* UMyProjectGameInstance::ValidationPlayer(int ObjectId)
{

	if (Socket == nullptr || GameServerSession == nullptr)
		return nullptr;

	auto* World = GetWorld();
	if (World == nullptr)
		return nullptr;

	AMyProjectPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return nullptr;

	AMyProjectPlayer* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return nullptr;

	return Player;
}

ANPC* UMyProjectGameInstance::ValidationMonster(int ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return nullptr;

	auto* World = GetWorld();
	if (World == nullptr)
		return nullptr;

	ANPC** FindActor = Monsters.Find(ObjectId);
	if (FindActor == nullptr)
		return nullptr;

	return *FindActor;

}



