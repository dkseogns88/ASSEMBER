// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNetworkActor.h"
#include "Net/UnrealNetwork.h"
// Sets default values
APlayerNetworkActor::APlayerNetworkActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 네트워크 활성화
	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerNetworkActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerNetworkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		NetLocation = GetActorLocation();
		NetRotation = GetActorRotation();
	}

}

void APlayerNetworkActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerNetworkActor, NetLocation);
	DOREPLIFETIME(APlayerNetworkActor, NetRotation);

}

// Called to bind functionality to input
void APlayerNetworkActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

