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
	// ���� �ν��Ͻ��� ����� �� ȣ��Ǵ� �޼���
	Super::Shutdown();

	// ACTOR_LIST�� �ʱ�ȭ�Ͽ� �޸𸮿��� ����
	ACTOR_LIST.Empty();
}
