// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A1.h"
#include "Characters/Monster.h"
#include "Engine/GameInstance.h"
#include "A1GameInstance.generated.h"

//ĳ���� �������� �����ϴ� ����ü �÷��̾��ȣ, ĳ�����̸����� ������
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
////////////	����    /////////////
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
	
	// ĳ���� �̸��� Ŭ������ �����ϴ� ��
	TMap<FString, FString> CharacterBlueprintPaths;
	TMap<int32, FString> PlayerCharacterChangeLog;


	FTimerHandle SpawnTimerHandle;
	FVector NextSpawnLocation;
	void SpawnMonster(TSubclassOf<class AMonster> MonsterClass);
	bool SpawnLocationValid(FVector Location, FRotator Rotation, FVector BoxExtent);

public:
	
	// ���� Ŭ���� ����
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


	// ������ ���͸� �����ϱ� ���� �����̳�
	UPROPERTY()
	TMap<uint64, AMonster*> Monsters;

	ABaseChar* MyPlayer;
	TMap<uint64, ABaseChar*> Players;

	UFUNCTION(BlueprintCallable)
	void SpawnMonsters();

public:
	/////////////////////////////////////
	////////////	����    /////////////
	/////////////////////////////////////
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AOtherPlayerChar> OtherPlayerClass;

};
