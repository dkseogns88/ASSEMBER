// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MyProjectMyPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyProject.h"
#include "Kismet/KismetMathLibrary.h"


AMyProjectMyPlayer::AMyProjectMyPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

void AMyProjectMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyProjectMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Send ����
	bool ForceSendPacket = false;

	if (LastDesiredInput != DesiredInput)
	{
		ForceSendPacket = true;
		LastDesiredInput = DesiredInput;
	}

	// State ����
	if (DesiredInput == FVector2D::Zero())
		SetMoveState(Protocol::MOVE_STATE_IDLE);
	else
		SetMoveState(Protocol::MOVE_STATE_RUN);

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;

		// ���� ��ġ ����
		{
			Protocol::PlayerInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
			Info->set_yaw(DesiredYaw);
			Info->set_state(GetMoveState());
		}

		SEND_PACKET(MovePkt);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
	
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectMyPlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyProjectMyPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProjectMyPlayer::Look);
	}
}

void AMyProjectMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// Cache
		{
			DesiredInput = MovementVector;

			DesiredMoveDirection = FVector::ZeroVector;
			DesiredMoveDirection += ForwardDirection * MovementVector.Y;
			DesiredMoveDirection += RightDirection * MovementVector.X;
			DesiredMoveDirection.Normalize();

			const FVector Location = GetActorLocation();
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
			DesiredYaw = Rotator.Yaw;
		}
	}
}

void AMyProjectMyPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}