// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OtherPlayerChar.h"
#include "../Objects/AnimInstanceCustom.h"
#include "../Objects/A1PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AOtherPlayerChar::AOtherPlayerChar()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->bUsePawnControlRotation = true;


	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(CameraBoom);
	FirstPersonCamera->bUsePawnControlRotation = false;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

}

AOtherPlayerChar::~AOtherPlayerChar()
{
}


void AOtherPlayerChar::BeginPlay()
{
	Super::BeginPlay();


}

void AOtherPlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMyPlayer() == false)
	{

		FVector NowLocation;
		NowLocation.X = PosInfo->x();
		NowLocation.Y = PosInfo->y();
		NowLocation.Z = PosInfo->z();
		
		const Protocol::MoveState State = PosInfo->state();
		FRotator NowRotation = GetActorRotation();
		FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

		float YawInterpValue = 10.f;
		if (State == Protocol::MOVE_STATE_IDLE)
		{
			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
			SetActorRotation(NewRotation);
		}
		
		if (State == Protocol::MOVE_STATE_RUN)
		{
			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		
		}
		if (State == Protocol::MoveState::MOVE_STATE_JUMP)
		{
			Jump();

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		}

		FVector NextLocation = GetActorLocation();

		float Distance = FVector::Dist(NowLocation, NextLocation);
		if (Distance >= 200.f)
		{
			FRotator NewRotation = FRotator(0, DestInfo->yaw(), 0);

			SetActorLocation(NowLocation);
			SetActorRotation(NewRotation);

			/*FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);*/
		}
	}
}

void AOtherPlayerChar::SetAiming(bool bNewAiming)
{
	bIsAiming = bNewAiming;
	UE_LOG(LogTemp, Log, TEXT("Aiming state set to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));

	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetAiming(bIsAiming);
		UE_LOG(LogTemp, Log, TEXT("Aiming state updated in animation blueprint to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UAnimInstanceCustom"));
	}
}

void AOtherPlayerChar::SpawnMuzzleFlash()
{
	if (MuzzleFlashEffect)
	{
		// 소켓의 위치와 회전을 가져옴
		FVector MuzzleLocation = FirstPersonMesh->GetSocketLocation(TEXT("Muzzle"));
		FRotator MuzzleRotation = FirstPersonMesh->GetSocketRotation(TEXT("Muzzle"));
		FVector Scale = FVector(3.0f, 3.0f, 3.0f);

		// 나이아가라 이펙트를 소켓에 붙여서 스폰
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleFlashEffect,
			FirstPersonMesh,
			TEXT("Muzzle"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale,
			true,
			true
		);


		if (NiagaraComponent)
		{
			NiagaraComponent->SetWorldScale3D(Scale);
		}
	}
}
