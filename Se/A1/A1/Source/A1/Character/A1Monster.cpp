#include "Character/A1Monster.h"
#include "Player/A1PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AA1Monster::AA1Monster()
{

}

void AA1Monster::BeginPlay()
{
	Super::BeginPlay();

}

void AA1Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsMyPlayer() == false) {
		const Protocol::MoveState State = DestInfo->state();

		if (State == Protocol::MOVE_STATE_RUN)
		{
			FVector NowLocation = GetActorLocation();
			FVector NewLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());
			
			FVector ForwardDirection = (NewLocation - NowLocation);
			ForwardDirection.Normalize();

			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(NowLocation, NowLocation + ForwardDirection);

			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, Rotator.Yaw, 0);
			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 5.f); // 보간 속도는 5.0f로 설정
			SetActorRotation(NewRotation);

			AddMovementInput(ForwardDirection);
		}
	}
}
