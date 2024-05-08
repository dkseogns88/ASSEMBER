// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\Engine\Classes\GameFramework\Actor.h"
#include "MyProject.h"
#include "Character/Enemy1.h"
#include "Character/Enemy2.h"
#include "MyProjectGameInstance.generated.h"


class AMyProjectPlayer;

UCLASS()
class MYPROJECT_API UMyProjectGameInstance : public UGameInstance
{

	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);	

public:
	void HandleSpawn(const Protocol::ObjectInfo& objectInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);
	void HandleJump(const Protocol::S_JUMP& JumpPkt);
	void HandleZoom(const Protocol::S_ZOOM& ZommPkt);

	void HandleMonsterSpawn(const Protocol::S_SPAWN_MONSTER& SpawnPkt);
	void HandleMonsterSpawn(const Protocol::ObjectInfo& MonsterInfo);
	
	void HandleChange(const FString& CharacterName);
	// 캐릭터 클래스 찾기 함수

	TSubclassOf<APawn> FindCharacterClassByName(const FString& CharacterName);


	

	// 초기화 함수
	virtual void Init() override;
	UMyProjectGameInstance();
	void SpawnMonsterAtLocation(UClass* MonsterClass, const FVector& Location);

private:
	// 캐릭터 이름과 클래스를 매핑하는 맵
	TMap<FString, FString> CharacterBlueprintPaths;

	FTimerHandle SpawnTimerHandle;
	void SpawnNPC();
	
public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyProjectPlayer> OtherPlayerClass;

	// 몬스터 클래스 참조
	UPROPERTY()
	UClass* MonsterClass1;

	UPROPERTY()
	UClass* MonsterClass2;

	// 스폰된 몬스터를 관리하기 위한 컨테이너
	UPROPERTY()
	TArray<AActor*> SpawnedMonsters;

	AMyProjectPlayer* MyPlayer;
	TMap<uint64, AMyProjectPlayer*> Players;

};
