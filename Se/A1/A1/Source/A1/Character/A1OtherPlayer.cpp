


#include "Character/A1OtherPlayer.h"
#include "Player/A1PlayerController.h"
#include "Kismet/GameplayStatics.h"

AA1OtherPlayer::AA1OtherPlayer()
{
}

AA1OtherPlayer::~AA1OtherPlayer()
{
}

void AA1OtherPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void AA1OtherPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMyPlayer() == false)
	{
		const Protocol::MoveState State = PlayerInfo->state();

		if (State == Protocol::MOVE_STATE_RUN)
		{
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 5.f); // 보간 속도는 5.0f로 설정
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);

		}
		else if (State == Protocol::MOVE_STATE_JUMP)
		{
			Jump();

			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 5.0f); // 보간 속도는 5.0f로 설정
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		}

		FVector NowLocation = GetActorLocation();
		FVector TargetLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		float Distance = FVector::Dist(NowLocation, TargetLocation);
		if (Distance >= 200)
		{
			SetActorLocation(TargetLocation);
		}
	}
}

void AA1OtherPlayer::PlayZoom(bool IsZoom)
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
