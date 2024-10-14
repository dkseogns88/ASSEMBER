// Fill out your copyright notice in the Description page of Project Settings.


#include "GradCharacter.h"
#include "GradPawnExtensionComponent.h"
#include "GradGame/Camera/GradCameraComponent.h"
#include "GradGame/AbilitySystem/GradAbilitySystemComponent.h"

// Sets default values
AGradCharacter::AGradCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// Tick�� ��Ȱ��ȭ -> ��������Ʈ ��Ļ��
	//PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent ����
	PawnExtComponent = CreateDefaultSubobject<UGradPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent ����
	{
		CameraComponent = CreateDefaultSubobject<UGradCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}

	// AbilitySystemComponent ����
	AbilitySystemComponent = CreateDefaultSubobject<UGradAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Ability System Component�� �����ڰ� ������ �ƹ�Ÿ�� ���������� ������ ����� �Ѵ�.
	//AbilitySystemComponent->InitAbilityActorInfo(this, this);

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

	// Pawn�� Possess�μ�, Controller�� PlayerState ���� ������ ������ ���°� �Ǿ���:
	// - SetupPlayerInputComponent Ȯ��
	PawnExtComponent->SetupPlayerInputComponent();
}

