


#include "Player/A1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/A1AssetManager.h"
#include "Data/A1InputData.h"
#include "A1GameplayTags.h"
#include "Character/A1MyPlayer.h"
#include "Network/A1NetworkManager.h"
#include "GameFramework/CharacterMovementComponent.h"


AA1PlayerController::AA1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AA1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (const UA1InputData* InputData = UA1AssetManager::GetAssetByName<UA1InputData>("InputData"))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputData->InputMappingContext, 0);
		}
	}

	A1MyPlayer = Cast<AA1MyPlayer>(GetCharacter());
}

void AA1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (const UA1InputData* InputData = UA1AssetManager::GetAssetByName<UA1InputData>("InputData"))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);


		auto Action1 = InputData->FindInputActionByTag(A1GameplayTags::Input_Action_Move);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Completed, this, &ThisClass::Input_Move);

		auto Action2 = InputData->FindInputActionByTag(A1GameplayTags::Input_Action_Turn);
		EnhancedInputComponent->BindAction(Action2, ETriggerEvent::Triggered, this, &ThisClass::Input_Turn);

		auto Action3 = InputData->FindInputActionByTag(A1GameplayTags::Input_Action_Jump);
		EnhancedInputComponent->BindAction(Action3, ETriggerEvent::Started, this, &ThisClass::Input_Jump);

		auto Action4 = InputData->FindInputActionByTag(A1GameplayTags::Input_Action_Mouse);
		EnhancedInputComponent->BindAction(Action4, ETriggerEvent::Started, this, &ThisClass::Input_Mouse);
		EnhancedInputComponent->BindAction(Action4, ETriggerEvent::Completed, this, &ThisClass::Input_Mouse);
	}
}

void AA1PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	StateTick();
	SendTick(DeltaTime);
	
}

void AA1PlayerController::StateTick()
{
	if (A1MyPlayer->bPressedJump)
	{
		IsJumping = true;
	}
	else {
		IsJumping = false;

	}

	if (DesiredInput == FVector2D::Zero() && (IsJumping == false))
	{
		A1MyPlayer->SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
	else if ((DesiredInput != FVector2D::Zero()) && (IsJumping == false))
	{
		A1MyPlayer->SetMoveState(Protocol::MOVE_STATE_RUN);
	}
	else if (IsJumping == true)
	{
		A1MyPlayer->SetMoveState(Protocol::MOVE_STATE_JUMP);
	}
}

void AA1PlayerController::SendTick(float DeltaTime)
{
	// Send ÆÇÁ¤
	bool ForceSendPacket = false;

	if ((LastDesiredInput != DesiredInput) || (bLastInputJump) || (bIsTurn))
	{
		ForceSendPacket = true;

		bLastInputJump = false;
		bIsTurn = false;
		LastDesiredInput = DesiredInput;
	}

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		if (A1MyPlayer->GetMoveState() == Protocol::MOVE_STATE_RUN
			|| A1MyPlayer->GetMoveState() == Protocol::MOVE_STATE_IDLE)
		{
			Send_Idle_Move();
		}
		else if (A1MyPlayer->GetMoveState() == Protocol::MOVE_STATE_JUMP)
		{
			Send_Jump();
		}
	}
}

void AA1PlayerController::Send_Idle_Move()
{
	Protocol::C_MOVE MovePkt;

	Protocol::PosInfo* Info = MovePkt.mutable_info();
	Info->CopyFrom(*(A1MyPlayer->PlayerInfo));
	Info->set_yaw(DesiredYaw);
	Info->set_state(A1MyPlayer->GetMoveState());
	Info->set_d_x(DesiredMoveDirection.X);
	Info->set_d_y(DesiredMoveDirection.Y);
	Info->set_d_z(DesiredMoveDirection.Z);

	A1MyPlayer->GetNetworkManager()->SendPacket(MovePkt);
}

void AA1PlayerController::Send_Jump()
{
	Protocol::C_JUMP JumpPkt;

	Protocol::PosInfo* Info = JumpPkt.mutable_info();
	Info->CopyFrom(*(A1MyPlayer->PlayerInfo));
	Info->set_yaw(DesiredYaw);
	Info->set_state(A1MyPlayer->GetMoveState());
	Info->set_d_x(DesiredMoveDirection.X);
	Info->set_d_y(DesiredMoveDirection.Y);
	Info->set_d_z(DesiredMoveDirection.Z);

	A1MyPlayer->GetNetworkManager()->SendPacket(JumpPkt);
}

void AA1PlayerController::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MovementVector = InputValue.Get<FVector2D>();

	FVector ForwardDirection;
	FVector RightDirection;

	if (MovementVector.X != 0)
	{
		FRotator Rotator = GetControlRotation();
		ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0, Rotator.Yaw, 0));
		A1MyPlayer->AddMovementInput(ForwardDirection, MovementVector.X);
	}

	if (MovementVector.Y != 0)
	{
		FRotator Rotator = GetControlRotation();
		RightDirection = UKismetMathLibrary::GetRightVector(FRotator(0, Rotator.Yaw, 0));
		A1MyPlayer->AddMovementInput(RightDirection, MovementVector.Y);
	}

	// Cache
	{
		DesiredInput = MovementVector;

		DesiredMoveDirection = FVector::ZeroVector;
		DesiredMoveDirection += ForwardDirection * MovementVector.X;
		DesiredMoveDirection += RightDirection * MovementVector.Y;
		DesiredMoveDirection.Normalize();
		
		const FVector Location = A1MyPlayer->GetActorLocation();
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
		DesiredYaw = Rotator.Yaw;
	}

}

void AA1PlayerController::Input_Turn(const FInputActionValue& InputValue)
{
	FVector2D LookAxisVector = InputValue.Get<FVector2D>();

	if (A1MyPlayer->GetMoveState() != Protocol::MOVE_STATE_IDLE)
	{
		bIsTurn = true;
	}

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AA1PlayerController::Input_Jump(const FInputActionValue& InputValue)
{
	bLastInputJump = InputValue.Get<bool>();
	A1MyPlayer->Jump();
}

void AA1PlayerController::Input_Mouse(const FInputActionValue& InputValue)
{
	FVector2D MouseValue = InputValue.Get<FVector2D>();
	
	bool IsZoom = static_cast<bool>(MouseValue.Y);

	if (MouseValue.Y != 0)
	{
		if (ZoomMontage)
		{
			A1MyPlayer->PlayAnimMontage(ZoomMontage);

			Protocol::C_ZOOM ZoomPkt;

			Protocol::ZoomInfo* Info = ZoomPkt.mutable_info();
			Info->set_object_id(A1MyPlayer->PlayerInfo->object_id());
			Info->set_b_zoom(true);

			A1MyPlayer->GetNetworkManager()->SendPacket(ZoomPkt);
		}
	}

	if (MouseValue.Y == 0)
	{
		if (ZoomMontage)
		{
			A1MyPlayer->StopAnimMontage(ZoomMontage);

			Protocol::C_ZOOM ZoomPkt;

			Protocol::ZoomInfo* Info = ZoomPkt.mutable_info();
			Info->set_object_id(A1MyPlayer->PlayerInfo->object_id());
			Info->set_b_zoom(false);

			A1MyPlayer->GetNetworkManager()->SendPacket(ZoomPkt);
		}
	}

}
