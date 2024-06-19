// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OtherPlayerChar.h"
#include "../Objects/AnimInstanceCustom.h"

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
		const Protocol::MoveState State = PlayerInfo->state();

		if (State == Protocol::MOVE_STATE_IDLE)
		{
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 10.f);
			SetActorRotation(NewRotation);
		}

		if (State == Protocol::MOVE_STATE_RUN)
		{
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 10.f);
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		}

		/*FVector NowLocation = GetActorLocation();
		FVector TargetLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		float Distance = FVector::Dist(NowLocation, TargetLocation);
		if (Distance >= 200)
		{
			SetActorLocation(TargetLocation);
		}*/
	}
}
