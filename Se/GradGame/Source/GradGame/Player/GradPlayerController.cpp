// Fill out your copyright notice in the Description page of Project Settings.


#include "GradPlayerController.h"
#include "GradGame/Camera/GradPlayerCameraManager.h"
#include "EnhancedInputSubsystems.h"
#include "GradGame/Network/GradNetworkComponent.h"
#include "GradGame/Character/GradCharacter.h"
#include "GradGame/Network/NetworkManager.h"
#include "GradGame/AbilitySystem/GradAbilitySystemComponent.h"

AGradPlayerController::AGradPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AGradPlayerCameraManager::StaticClass();
}

void AGradPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AGradPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TObjectPtr<AGradCharacter> MyPlayer = Cast<AGradCharacter>(GetCharacter());

	if (MyPlayer == nullptr) return;

	UGradNetworkComponent* NetComponent = MyPlayer->FindComponentByClass<UGradNetworkComponent>();
	if (NetComponent == nullptr) return;

	if (DesiredInput == FVector2D::Zero())
	{
		NetComponent->SetMoveState(Protocol::MOVE_STATE_IDLE);
		DesiredYaw = GetControlRotation().Yaw;
	}
	else if ((DesiredInput != FVector2D::Zero()))
	{
		NetComponent->SetMoveState(Protocol::MOVE_STATE_RUN);
	}

	// Send 판정
	bool ForceSendPacket = false;

	if ((LastDesiredInput != DesiredInput))
	{
		ForceSendPacket = true;
		LastDesiredInput = DesiredInput;
	}

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;

		Protocol::PosInfo* Info = MovePkt.mutable_info();
		Info->CopyFrom(*(NetComponent->GetPosInfo()));
		Info->set_yaw(DesiredYaw);
		Info->set_move_state(NetComponent->GetMoveState());
		Info->set_d_x(DesiredMoveDirection.X);
		Info->set_d_y(DesiredMoveDirection.Y);
		Info->set_d_z(DesiredMoveDirection.Z);

		// 이건 내 실제 좌표
		FVector MyLocation = MyPlayer->GetActorLocation();
		Info->set_x(MyLocation.X);
		Info->set_y(MyLocation.Y);
		Info->set_z(MyLocation.Z);
		Info->set_pitch(DesiredPitch);
		Info->set_roll(DesiredRoll);

		GetGameInstance()->GetSubsystem<UNetworkManager>()->SendPacket(MovePkt);

	}
}

void AGradPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UGradAbilitySystemComponent* GradASC = GetGradAbilitySystemComponent())
	{
		GradASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UGradAbilitySystemComponent* AGradPlayerController::GetGradAbilitySystemComponent() const
{
	AGradCharacter* GradPawn = Cast<AGradCharacter>(GetPawn());
	return (GradPawn ? GradPawn->GetGradAbilitySystemComponent() : nullptr);
}
