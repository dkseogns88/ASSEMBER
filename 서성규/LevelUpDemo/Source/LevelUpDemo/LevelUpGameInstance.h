// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LevelUpGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FActorInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool active = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 player_id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float x;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float z;

};

class ClientSocket;
class ADemoActor;

UCLASS()
class LEVELUPDEMO_API ULevelUpGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ULevelUpGameInstance();
	virtual ~ULevelUpGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void SpawnCharacter();

	ClientSocket* SocketInstance;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
	TArray<FActorInfo> ACTOR_LIST;


	// CreatedActors 배열에 생성된 Actor들을 저장
	UPROPERTY(BlueprintReadWrite, Category = "Game Info")
	TArray<ADemoActor*> CreatedActors;

	// 게임 종료 시 호출되는 함수
	UFUNCTION(BlueprintCallable, Category = "Game Info")
	void Cleanup();

	virtual void Shutdown() override;

public:
	bool isNewPlayer{ false };
	FActorInfo isNewPlayerInfo;
};