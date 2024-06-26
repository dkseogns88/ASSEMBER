// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OtherPlayerChar.h"
#include "../Objects/AnimInstanceCustom.h"
#include "../Objects/A1PlayerController.h"
#include "Kismet/GameplayStatics.h"

AOtherPlayerChar::AOtherPlayerChar()
{

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
		const Protocol::MoveState State = PosInfo->state();

		float YawInterpValue = 10.f;
		if (State == Protocol::MOVE_STATE_IDLE)
		{
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);


			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
			SetActorRotation(NewRotation);
		}
		
		if (State == Protocol::MOVE_STATE_RUN)
		{
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		
		}
		if (State == Protocol::MoveState::MOVE_STATE_JUMP)
		{
			Jump();

			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		}


		FVector NowLocation = GetActorLocation();
		FVector NextLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NowLocation: %s"), *NowLocation.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NextLocation: %s"), *NextLocation.ToString()));

		float Distance = FVector::Dist(NowLocation, NextLocation);
		if (Distance >= 200.f)
		{
			SetActorLocation(NextLocation);
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
