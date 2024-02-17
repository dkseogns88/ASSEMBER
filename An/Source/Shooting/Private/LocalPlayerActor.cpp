// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalPlayerActor.h"

ALocalPlayerActor::ALocalPlayerActor()
{
    // ī�޶� �� ����
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // ī�޶� ������Ʈ ����
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void ALocalPlayerActor::BeginPlay()
{
    Super::BeginPlay();
}

void ALocalPlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // ���⿡ �Է� ���ε� ����
}