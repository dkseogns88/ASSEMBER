


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
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"


AA1PlayerController::AA1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimTimeline"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/Etc/CV_AimCrouch.CV_AimCrouch"));
	if (curve.Succeeded())
	{
		AimCurve = curve.Object;
	}

    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("/Game/UI/UI_Dot.UI_Dot_C"));
    if (WidgetAsset.Succeeded())
    {
		CreatedWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetAsset.Class);
    }

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

	if (AimCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("AimTimelineProgress"));

		AimTimeline->AddInterpFloat(AimCurve, ProgressFunction);

		AimTimeline->SetLooping(false);
	}
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
		EnhancedInputComponent->BindAction(Action3, ETriggerEvent::Completed, this, &ThisClass::Input_Jump);

		auto Action4 = InputData->FindInputActionByTag(A1GameplayTags::Input_Action_Sprint);
		EnhancedInputComponent->BindAction(Action4, ETriggerEvent::Started, this, &ThisClass::Input_Sprint);
		EnhancedInputComponent->BindAction(Action4, ETriggerEvent::Completed, this, &ThisClass::Input_Sprint);

		auto Action5 = InputData->FindInputActionByTag(A1GameplayTags::Input_Action_Crouch);
		EnhancedInputComponent->BindAction(Action5, ETriggerEvent::Started, this, &ThisClass::Input_Crouch);

		auto Action6 = InputData->FindInputActionByTag(A1GameplayTags::Input_Action_Aim);
		EnhancedInputComponent->BindAction(Action6, ETriggerEvent::Triggered, this, &ThisClass::Input_Aim);
		EnhancedInputComponent->BindAction(Action6, ETriggerEvent::Completed, this, &ThisClass::Input_Aim);
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
	if (DesiredInput == FVector2D::Zero())
	{
		A1MyPlayer->SetMoveState(Protocol::MOVE_STATE_IDLE);
		DesiredYaw = GetControlRotation().Yaw;
	}
	else if ((DesiredInput != FVector2D::Zero()))
	{
		if(A1MyPlayer->PlayerWalkSpeed == A1MyPlayer->WalkSpeed){ A1MyPlayer->SetMoveState(Protocol::MOVE_STATE_WALK); }
		else if(A1MyPlayer->PlayerWalkSpeed == A1MyPlayer->RunSpeed){ A1MyPlayer->SetMoveState(Protocol::MOVE_STATE_RUN); }
	}

	if (bLastInputJump) {
		// TODO: 점프도 걷는 속도 해야하나?
		A1MyPlayer->SetMoveState(Protocol::MOVE_STATE_JUMP);
	}

}

void AA1PlayerController::SendTick(float DeltaTime)
{
	// Send 판정
	bool ForceSendPacket = false;

	if ((LastDesiredInput != DesiredInput))
	{
		ForceSendPacket = true;
		LastDesiredInput = DesiredInput;
	}

	if (bLastInputJump) {
		ForceSendPacket = true;
		bLastInputJump = false;
	}

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		SendMoveState();
	}
}

void AA1PlayerController::SendMoveState()
{
	Protocol::C_MOVE MovePkt;

	Protocol::PosInfo* Info = MovePkt.mutable_info();
	Info->CopyFrom(*(A1MyPlayer->GetPosInfo()));
	Info->set_yaw(DesiredYaw);
	Info->set_move_state(A1MyPlayer->GetMoveState());
	Info->set_d_x(DesiredMoveDirection.X);
	Info->set_d_y(DesiredMoveDirection.Y);
	Info->set_d_z(DesiredMoveDirection.Z);

	// 이건 내 실제 좌표
	FVector MyLocation = A1MyPlayer->GetActorLocation();
	Info->set_x(MyLocation.X);
	Info->set_y(MyLocation.Y);
	Info->set_z(MyLocation.Z);

	Info->set_aim_pitch(A1MyPlayer->AimPitch);
	Info->set_aim_yaw(A1MyPlayer->AimYaw);
	
	A1MyPlayer->GetNetworkManager()->SendPacket(MovePkt);
}

void AA1PlayerController::SendAllState()
{
	Protocol::C_STATE StatePkt;

	Protocol::StateInfo* Info = StatePkt.mutable_state();
	Info->CopyFrom(*(A1MyPlayer->GetStateInfo()));

	A1MyPlayer->GetNetworkManager()->SendPacket(StatePkt);
}

void AA1PlayerController::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MovementVector = InputValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


	A1MyPlayer->AddMovementInput(ForwardDirection, MovementVector.X);
	A1MyPlayer->AddMovementInput(RightDirection, MovementVector.Y);


	// Cache
	{
		DesiredInput = MovementVector;

		DesiredMoveDirection = FVector::ZeroVector;
		DesiredMoveDirection += ForwardDirection * MovementVector.X;
		DesiredMoveDirection += RightDirection * MovementVector.Y;
		DesiredMoveDirection.Normalize();
		
		//const FVector Location = A1MyPlayer->GetActorLocation();
		//FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
		//DesiredYaw = Rotator.Yaw;
		
		DesiredYaw = YawRotation.Yaw;


	
	}

}

void AA1PlayerController::Input_Turn(const FInputActionValue& InputValue)
{
	FVector2D LookAxisVector = InputValue.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AA1PlayerController::Input_Jump(const FInputActionValue& InputValue)
{
	bLastInputJump = InputValue.Get<bool>();

	if(InputValue.Get<bool>())
		A1MyPlayer->Jump();
	else
		A1MyPlayer->StopJumping();
}

void AA1PlayerController::Input_Sprint(const FInputActionValue& InputValue)
{
	bool IsSprint = InputValue.Get<bool>();
	A1MyPlayer->bSprint = IsSprint;

	if (IsSprint)
	{
		if (A1MyPlayer->bCrouch)
		{
			A1MyPlayer->bCrouch = false;
			A1MyPlayer->CrouchTimeline->ReverseFromEnd();
		}

		A1MyPlayer->GetCharacterMovement()->MaxWalkSpeed = A1MyPlayer->RunSpeed;
		A1MyPlayer->PlayerWalkSpeed = A1MyPlayer->RunSpeed;
	
		A1MyPlayer->SetLocomotionState(Protocol::LOCOMOTION_STATE_IDLE);
	}
	else
	{
		A1MyPlayer->GetCharacterMovement()->MaxWalkSpeed = A1MyPlayer->WalkSpeed;
		A1MyPlayer->PlayerWalkSpeed = A1MyPlayer->WalkSpeed;
	}

	SendAllState();
}

void AA1PlayerController::Input_Crouch(const FInputActionValue& InputValue)
{
	A1MyPlayer->bCrouch = !(A1MyPlayer->bCrouch);
	if (A1MyPlayer->bCrouch)
	{
		A1MyPlayer->bSprint = false;
		A1MyPlayer->CrouchTimeline->Play();

		A1MyPlayer->SetLocomotionState(Protocol::LOCOMOTION_STATE_CROUCH);

	}
	else
	{
		A1MyPlayer->CrouchTimeline->ReverseFromEnd();
		A1MyPlayer->SetLocomotionState(Protocol::LOCOMOTION_STATE_IDLE);
	}

	SendAllState();
}

void AA1PlayerController::Input_Aim(const FInputActionValue& InputValue)
{
	Protocol::OverlayState State;

	switch (A1MyPlayer->OverlayState)
	{
	case EOverlayStates::Pistol:
		State = Protocol::OVERLAY_STATE_PISTOL;
		break;
	case EOverlayStates::Rifle:
		State = Protocol::OVERLAY_STATE_RIFLE;
		break;
	default:
		State = Protocol::OVERLAY_STATE_NONE;
		break;
	}

	if (InputValue.Get<bool>())
	{
		AimTimeline->Play();
		CreatedWidget->AddToViewport();
		A1MyPlayer->bAiming = true;

		FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(A1MyPlayer->GetActorRotation(), GetControlRotation());
		A1MyPlayer->AimPitch = DeltaRotator.Pitch;
		A1MyPlayer->AimYaw = DeltaRotator.Yaw;

		A1MyPlayer->SetOverlayState(State);
		A1MyPlayer->GetStateInfo()->set_aim(true);
	}
	else
	{
		AimTimeline->ReverseFromEnd();
		CreatedWidget->RemoveFromParent();
		A1MyPlayer->bAiming = false;

		A1MyPlayer->SetOverlayState(State);
		A1MyPlayer->GetStateInfo()->set_aim(false);
	}

	SendAllState();
}

void AA1PlayerController::AimTimelineProgress(float Value)
{
	float ArmLength = FMath::Lerp(300.f, 200.f, Value);
	A1MyPlayer->SpringArm->TargetArmLength = ArmLength;
}