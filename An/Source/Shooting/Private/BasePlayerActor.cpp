// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerActor.h"
#include "Net/UnrealNetwork.h"

ABasePlayerActor::ABasePlayerActor()
{
    // 네트워크 활성화
    SetReplicates(true);
    PrimaryActorTick.bCanEverTick = true;
}

void ABasePlayerActor::BeginPlay()
{
    Super::BeginPlay();
}

void ABasePlayerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABasePlayerActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 여기에 레플리케이션을 위한 프로퍼티 추가
}







