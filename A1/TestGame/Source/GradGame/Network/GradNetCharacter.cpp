
#include "GradNetCharacter.h"
#include "GradNetworkComponent.h"

AGradNetCharacter::AGradNetCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGradNetCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

PRAGMA_DISABLE_OPTIMIZATION
void AGradNetCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGradNetworkComponent* PawnNetComp = FindComponentByClass<UGradNetworkComponent>())
	{
		const Protocol::MoveState Move_State = PawnNetComp->PosInfo->move_state();
		FRotator NowRotation = GetActorRotation();

		// TODO: Pitch에 대한 값도 설정해야함..
		//FRotator TargetRotation = FRotator(PawnNetComp->PosInfo->pitch(), PawnNetComp->PosInfo->yaw(), PawnNetComp->PosInfo->roll());
		FRotator TargetRotation = FRotator(0, PawnNetComp->PosInfo->yaw(), 0);

		float YawInterpValue = 10.f;

		if (Move_State == Protocol::MOVE_STATE_IDLE)
		{
			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
			if (NewRotation.Equals(TargetRotation, 1.0f) == false)
			{
 				SetActorRotation(NewRotation);
			}
			else
			{
				SetActorRotation(TargetRotation);

			}
		}
		else if (Move_State == Protocol::MOVE_STATE_RUN)
		{
			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
			if (NewRotation.Equals(TargetRotation, 1.0f) == false)
			{
				SetActorRotation(NewRotation);
			
			}
			else
			{
				SetActorRotation(TargetRotation);

			}
			FVector ForwardDirection = FVector(PawnNetComp->PosInfo->d_x(), PawnNetComp->PosInfo->d_y(), PawnNetComp->PosInfo->d_z());
			AddMovementInput(ForwardDirection);
		}


		float Distance = FVector::Dist(GetActorLocation(), FVector(PawnNetComp->PosInfo->x(), PawnNetComp->PosInfo->y(), PawnNetComp->PosInfo->z()));
		if (Distance >= 200.f)
		{

			SetActorLocation(FVector(PawnNetComp->PosInfo->x(), PawnNetComp->PosInfo->y(), PawnNetComp->PosInfo->z()));
			SetActorRotation(TargetRotation);
		}
	}



}
PRAGMA_ENABLE_OPTIMIZATION