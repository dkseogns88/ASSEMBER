// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalPlayerActor.h"

ALocalPlayerActor::ALocalPlayerActor()
{
    // 카메라 붐 설정
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // 카메라 컴포넌트 설정
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

    // 여기에 입력 바인딩 설정
}