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
	
	if (CameraBoom)
	{
		CameraBoom->DestroyComponent();
	}

	if (FollowCamera)
	{
		FollowCamera->DestroyComponent();
	}

	// Create a first-person camera component
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a first-person mesh component for the character (arms only)
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// Set up the character mesh (body) to be invisible in first-person
	GetMesh()->SetOwnerNoSee(true);
	
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

void AMyProjectMyPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	
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

	StateTick();
	SendTick(DeltaTime);

}

void AMyProjectMyPlayer::StateTick()
{

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
}

void AMyProjectMyPlayer::SendTick(float DeltaTime)
{
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

		if (GetMoveState() == Protocol::MOVE_STATE_RUN
			|| GetMoveState() == Protocol::MOVE_STATE_IDLE)
		{
			Send_Idle_Move();
		}
		else if (GetMoveState() == Protocol::MOVE_STATE_JUMP)
		{
			Send_Jump();
		}
	}
}

void AMyProjectMyPlayer::Send_Idle_Move()
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

void AMyProjectMyPlayer::Send_Jump()
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
	

	if (GetMoveState() != Protocol::MOVE_STATE_IDLE)
	{
		bIsTurn = true;
	}

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMyProjectMyPlayer::Input_Jump(const FInputActionValue& Value)
{
	bLastInputJump = Value.Get<bool>();
	Jump();
}

void AMyProjectMyPlayer::SetAiming(bool bNewAiming)
{
	bIsAiming = bNewAiming; 
	UE_LOG(LogTemp, Log, TEXT("Aiming state set to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));

	
	
	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		
		AnimInstance->bIsAiming = bIsAiming;
		UE_LOG(LogTemp, Log, TEXT("Aiming state updated in animation blueprint to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UAnimInstanceCustom"));
	}
}

void AMyProjectMyPlayer::OnRep_Aimingchanged()
{
	SetAiming(bIsAiming);
}