// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpGameInstance.h"
#include "ClientSocket.h"
#include "DemoActor.h"

ULevelUpGameInstance::ULevelUpGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance"));
	SocketInstance = nullptr;
}

ULevelUpGameInstance::~ULevelUpGameInstance()
{
}

void ULevelUpGameInstance::Init()
{
	Super::Init();
	SocketInstance = new ClientSocket(this);
}

void ULevelUpGameInstance::SpawnCharacter()
{
	FVector SpawnLocation = FVector::ZeroVector;

	FRotator SpawnRotation = FRotator::ZeroRotator;

	// Spawn the actor
	ADemoActor* NewActor = GetWorld()->SpawnActor<ADemoActor>(ADemoActor::StaticClass(), SpawnLocation, SpawnRotation);
	if (NewActor) {
		NewActor->SetId(isNewPlayerInfo.player_id);
		NewActor->SetGameInstance(this);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to create actor!"));
	}

}

void ULevelUpGameInstance::Cleanup()
{}

void ULevelUpGameInstance::Shutdown()
{
	// 게임 인스턴스가 종료될 때 호출되는 메서드
	Super::Shutdown();

	// ACTOR_LIST를 초기화하여 메모리에서 해제
	ACTOR_LIST.Empty();
}
