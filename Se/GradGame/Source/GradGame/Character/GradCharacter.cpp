// Fill out your copyright notice in the Description page of Project Settings.


#include "GradCharacter.h"
#include "GradPawnExtensionComponent.h"
#include "GradGame/Camera/GradCameraComponent.h"
#include "GradGame/AbilitySystem/GradAbilitySystemComponent.h"
#include "GradGame/AbilitySystem/Attributes/GradCombatSet.h"
#include "GradGame/AbilitySystem/Attributes/GradHealthSet.h"
#include "GradHealthComponent.h"

// Sets default values
AGradCharacter::AGradCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// Tick을 비활성화 -> 델리게이트 방식사용
	//PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent 생성
	PawnExtComponent = CreateDefaultSubobject<UGradPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	{
		PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
		PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
	}

	// CameraComponent 생성
	{
		CameraComponent = CreateDefaultSubobject<UGradCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}

	// AbilitySystemComponent 생성
	AbilitySystemComponent = CreateDefaultSubobject<UGradAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CreateDefaultSubobject<UGradHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UGradCombatSet>(TEXT("CombatSet"));

	// HealthComponent 생성
	{
		HealthComponent = CreateDefaultSubobject<UGradHealthComponent>(TEXT("HealthComponent"));
	}
}

void AGradCharacter::OnAbilitySystemInitialized()
{
	check(AbilitySystemComponent);

	// HealthComponent의 ASC를 통한 초기화
	HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void AGradCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeWithAbilitySystem();
}

// Called when the game starts or when spawned
void AGradCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGradCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGradCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었음:
	// - SetupPlayerInputComponent 확인
	PawnExtComponent->SetupPlayerInputComponent();
}

