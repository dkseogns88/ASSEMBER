// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectMyPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyProject.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyProjectCharacter.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceCustom.h"



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


// APawn interface

inline void AMyProjectMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyProjectMyPlayer::Input_Jump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectMyPlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyProjectMyPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProjectMyPlayer::Look);
	}
}

void AMyProjectMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 1000);
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		SetActorLocation(HitResult.Location);
	}

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
	
	bool ForceSendPacket = false;
	
	if ((LastDesiredInput != DesiredInput) || (bLastInputJump))
	{
		ForceSendPacket = true;
		bLastInputJump = false;
		
		LastDesiredInput = DesiredInput;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		IsJumping = true;
	}
	else {
		IsJumping = false;
	}

	if (DesiredInput == FVector2D::Zero() && (IsJumping == false))
	{
		SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
	else if ((DesiredInput != FVector2D::Zero()) && (IsJumping == false))
	{
		SetMoveState(Protocol::MOVE_STATE_RUN);
	}
	else if (IsJumping == true)
	{
		SetMoveState(Protocol::MOVE_STATE_JUMP);
	}

	MovePacketSendTimer -= DeltaTime;
	
	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		if (GetMoveState() == Protocol::MOVE_STATE_RUN
			|| GetMoveState() == Protocol::MOVE_STATE_IDLE)
		{
			Protocol::C_MOVE MovePkt;

			Protocol::PosInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
			Info->set_yaw(DesiredYaw);
			Info->set_state(GetMoveState());
			Info->set_d_x(DesiredMoveDirection.X);
			Info->set_d_y(DesiredMoveDirection.Y);
			Info->set_d_z(DesiredMoveDirection.Z);

			SEND_PACKET(MovePkt);
		}
		else if (GetMoveState() == Protocol::MOVE_STATE_JUMP)
		{
			Protocol::C_JUMP JumpPkt;

			Protocol::PosInfo* Info = JumpPkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
			Info->set_yaw(DesiredYaw);
			Info->set_state(GetMoveState());
			Info->set_d_x(DesiredMoveDirection.X);
			Info->set_d_y(DesiredMoveDirection.Y);
			Info->set_d_z(DesiredMoveDirection.Z);

			SEND_PACKET(JumpPkt);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

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

void AMyProjectMyPlayer::Input_Jump(const FInputActionValue& Value)
{
	bLastInputJump = Value.Get<bool>();
	Jump();
}

void AMyProjectMyPlayer::SetAiming(bool bNewAiming)
{
	bIsAiming = bNewAiming; // Update internal aiming state
	UE_LOG(LogTemp, Log, TEXT("Aiming state set to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));

	// Get the animation instance from the skeletal mesh component
	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		// Update the animation blueprint variable
		AnimInstance->bIsAiming = bIsAiming;
		UE_LOG(LogTemp, Log, TEXT("Aiming state updated in animation blueprint to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UAnimInstanceCustom"));
	}
}