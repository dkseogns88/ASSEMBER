// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A1.h"
#include "Characters/Monster.h"
#include "Engine/GameInstance.h"
#include "A1GameInstance.generated.h"

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

class ABaseChar;
class APlayerChar;
class AMonster;


/////////////////////////////////////
////////////	서버    /////////////
/////////////////////////////////////

UCLASS()
class A1_API UA1GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	virtual void Init() override;

	UA1GameInstance();

	//void HandleSelectType(const Protocol::S_SELECT& SelectPkt);

	UClass* GetCharacterClass(const FString& CharacterName) const;
	void LogCharacterChange(int32 PlayerIndex, const FString& NewCharacterName);

	void LogNavMeshPolygons();

	UFUNCTION(BlueprintCallable, Category = "CharacterSelect")
	void CharacterSelect(FString CharacterName);
	
	// 캐릭터 이름과 클래스를 매핑하는 맵
	TMap<FString, FString> CharacterBlueprintPaths;
	TMap<int32, FString> PlayerCharacterChangeLog;


	FTimerHandle SpawnTimerHandle;
	FVector NextSpawnLocation;
	void SpawnMonster(TSubclassOf<class AMonster> MonsterClass);
	bool SpawnLocationValid(FVector Location, FRotator Rotation, FVector BoxExtent);

public:
	
	// 몬스터 클래스 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AMonster> BPClassMonk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AMonster> BPClassFanatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<APlayerChar> OtherPlayerClassRinty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<APlayerChar> BPClassRinty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<APlayerChar> BPClassSida;


	// 스폰된 몬스터를 관리하기 위한 컨테이너
	UPROPERTY()
	TMap<uint64, AMonster*> Monsters;

	ABaseChar* MyPlayer;
	TMap<uint64, ABaseChar*> Players;

	UFUNCTION(BlueprintCallable)
	void SpawnMonsters();

public:
	/////////////////////////////////////
	////////////	서버    /////////////
	/////////////////////////////////////
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AOtherPlayerChar> OtherPlayerClass;

};
