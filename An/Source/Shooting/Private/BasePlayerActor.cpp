// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerActor.h"
#include "Net/UnrealNetwork.h"

ABasePlayerActor::ABasePlayerActor()
{
    // ��Ʈ��ũ Ȱ��ȭ
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

    // ���⿡ ���ø����̼��� ���� ������Ƽ �߰�
}







