// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\Engine\Classes\GameFramework\Actor.h"
#include "MyProject.h"
#include "Character/NPC.h"
#include "MyProjectGameInstance.generated.h"


//캐릭터 변경정보 저장하는 구조체 플레이어번호, 캐릭터이름으로 저장함
USTRUCT(BlueprintType)
struct FPlayerCharacterChangeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerIndex;

	UPROPERTY(BlueprintReadOnly)
	FString NewCharacterName;

	FPlayerCharacterChangeInfo() : PlayerIndex(-1), NewCharacterName(TEXT("")) {}
	FPlayerCharacterChangeInfo(int32 InPlayerIndex, const FString& InNewCharacterName)
		: PlayerIndex(InPlayerIndex), NewCharacterName(InNewCharacterName) {}
};

class AMyProjectPlayer;
class AMyProjectMyPlayer;

class ANPC;

UCLASS()
class MYPROJECT_API UMyProjectGameInstance : public UGameInstance
{

	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	bool ConnectToGameServer();

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

	void HandleSelectType(const Protocol::S_SELECT& SelectPkt);
	void HandleTelePort(const Protocol::S_TELEPORT& TelePortPkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);
	void HandleJump(const Protocol::S_JUMP& JumpPkt);
	void HandleZoom(const Protocol::S_ZOOM& ZommPkt);
	void HandleRoll();//구르기함수

	void HandleMonsterSpawn(const Protocol::S_SPAWN_MONSTER& SpawnPkt);
	void HandleMonsterSpawn(const Protocol::ObjectInfo& MonsterInfo);

	void HandleHIT(const Protocol::S_HIT& pkt);
	void HandleAttack(const Protocol::S_ATTACK& pkt);

	// 초기화 함수
	virtual void Init() override;

	UMyProjectGameInstance();
	void SpawnMonsterAtLocation(const Protocol::PosInfo& Info);

	UClass* GetCharacterClass(const FString& CharacterName) const;
	void LogCharacterChange(int32 PlayerIndex, const FString& NewCharacterName);
private:
	// 캐릭터 이름과 클래스를 매핑하는 맵
	TMap<FString, FString> CharacterBlueprintPaths;
	TMap<int32, FString> PlayerCharacterChangeLog;
	

	FTimerHandle SpawnTimerHandle;
	void SpawnNPC();
	
private:
	AMyProjectPlayer* ValidationPlayer(int ObjectId);
	ANPC* ValidationMonster(int ObjectId);

public:
	class FSocket* Socket;
	FString IpAddress;
	int16 Port = 7777;
	
	TSharedPtr<class PacketSession> GameServerSession;

	UPROPERTY(BlueprintReadOnly)
	bool Connected = false;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyProjectPlayer> OtherPlayerClassRinty;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyProjectPlayer> OtherPlayerClassSida;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyProjectMyPlayer> BPClassRinty;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyProjectMyPlayer> BPClassSida;


	// 몬스터 클래스 참조
	UPROPERTY(EditAnywhere)
	TSubclassOf<ANPC> MonsterClass;


	// 스폰된 몬스터를 관리하기 위한 컨테이너
	UPROPERTY()
	TMap<uint64, ANPC*> Monsters;

	AMyProjectPlayer* MyPlayer;
	TMap<uint64, AMyProjectPlayer*> Players;


};
