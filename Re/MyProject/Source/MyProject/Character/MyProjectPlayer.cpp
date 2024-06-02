// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectPlayer.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyProjectMyPlayer.h"
#include "MyProjectGameInstance.h"
#include "..\..\Source\MyProject\AnimInstanceCustom.h"
#include "MyProjectPlayerController.h"


AMyProjectPlayer::AMyProjectPlayer()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;

	Speed = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	PlayerInfo = new Protocol::PosInfo();
	DestInfo = new Protocol::PosInfo();
}

AMyProjectPlayer::~AMyProjectPlayer() 
{
	delete PlayerInfo;
	delete DestInfo;
	PlayerInfo = nullptr;
	DestInfo = nullptr;
}

void AMyProjectPlayer::SetMovementSpeed(float NewSpeed)
{
	Speed = NewSpeed;
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AMyProjectPlayer::BeginPlay()
{
	Super::BeginPlay();

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 1000);
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		SetActorLocation(HitResult.Location);
	}

	{
		FVector Location = GetActorLocation();
		DestInfo->set_x(Location.X);
		DestInfo->set_y(Location.Y);
		DestInfo->set_z(Location.Z);
		DestInfo->set_yaw(GetControlRotation().Yaw);
		SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
}


void AMyProjectPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	{
		FVector Location = GetActorLocation();
		PlayerInfo->set_x(Location.X);
		PlayerInfo->set_y(Location.Y);
		PlayerInfo->set_z(Location.Z);
		PlayerInfo->set_yaw(GetControlRotation().Yaw);
	}

	bool MyPlayer;
	auto* GameInstance = Cast<UMyProjectGameInstance>(GWorld->GetGameInstance());
	if (GameInstance->MyPlayer != this)
		MyPlayer = false;
	else
		MyPlayer = true;

	if (MyPlayer == false)
	{
		const Protocol::MoveState State = PlayerInfo->state();

		if (State == Protocol::MOVE_STATE_RUN || State == Protocol::MOVE_STATE_JUMP)
		{
			//회전 보간
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);
			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaSeconds, 5.0f); // 보간 속도는 5.0f로 설정
			SetActorRotation(NewRotation);

			// 위치 보간
			FVector NowLocation = GetActorLocation();
			FVector TargetLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());
			FVector NewLocation = FMath::VInterpTo(NowLocation, TargetLocation, DeltaSeconds, 5.0f);
			SetActorLocation(NewLocation);

			//이동방향 처리
			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);

			// 점프 상태 처리
			if (State == Protocol::MOVE_STATE_JUMP && PreviousState != Protocol::MOVE_STATE_JUMP)
			{
				Jump();
			}

		}
		
		PreviousState = State;
	}
	

}

bool AMyProjectPlayer::IsMyPlayer() 
{
	if (Cast<AMyProjectMyPlayer>(this) != nullptr)
		return true;


	return false;
}


void AMyProjectPlayer::SetMoveState(Protocol::MoveState State)
{
	if (PlayerInfo->state() == State)
		return;

	PlayerInfo->set_state(State);
}

void AMyProjectPlayer::SetPlayerInfo(const Protocol::PosInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	PlayerInfo->CopyFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void AMyProjectPlayer::SetDestInfo(const Protocol::PosInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	DestInfo->CopyFrom(Info);

	SetMoveState(Info.state());
}