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

UCLASS()
class A1_API UA1GameInstance : public UGameInstance
{

	GENERATED_BODY()


	

public:
	

	

	// �ʱ�ȭ �Լ�
	virtual void Init() override;

	UA1GameInstance();

	//void HandleSelectType(const Protocol::S_SELECT& SelectPkt);

	UClass* GetCharacterClass(const FString& CharacterName) const;
	void LogCharacterChange(int32 PlayerIndex, const FString& NewCharacterName);
private:
	// ĳ���� �̸��� Ŭ������ �����ϴ� ��
	TMap<FString, FString> CharacterBlueprintPaths;
	TMap<int32, FString> PlayerCharacterChangeLog;


	FTimerHandle SpawnTimerHandle;
	void SpawnMonster();

private:
	APlayerChar* ValidationPlayer(int ObjectId);
	AMonster* ValidationMonster(int ObjectId);

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;

	UPROPERTY(BlueprintReadOnly)
	bool Connected = false;

public:
	
	// ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AMonster> MonsterClass;

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


};
