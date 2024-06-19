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

void AOtherPlayerChar::PlayZoom(bool IsZoom)
{
	if (IsZoom)
	{
		auto* PC = Cast<AA1PlayerController>(UGameplayStatics::GetPlayerController(this, 0));

		PlayAnimMontage(PC->GetZoomMontage());
	}

	else
	{
		auto* PC = Cast<AA1PlayerController>(UGameplayStatics::GetPlayerController(this, 0));

		StopAnimMontage(PC->GetZoomMontage());
	}
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
		const Protocol::MoveState State = PlayerInfo->state();

		if (State == Protocol::MOVE_STATE_IDLE)
		{
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 20.f);
			SetActorRotation(NewRotation);
		}
		
		if (State == Protocol::MOVE_STATE_RUN)
		{
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 20.f);
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		}
		if (State == Protocol::MoveState::MOVE_STATE_JUMP)
		{
			Jump();

			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 10.f); // 보간 속도는 5.0f로 설정
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		}


		/*FVector NowLocation = GetActorLocation();
		FVector TargetLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		float Distance = FVector::Dist(NowLocation, TargetLocation);
		if (Distance >= 500)
		{
			SetActorLocation(TargetLocation);
		}*/
	}
}
