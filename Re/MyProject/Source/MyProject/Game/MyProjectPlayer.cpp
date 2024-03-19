// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MyProjectPlayer.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Game/MyProjectMyPlayer.h"
#include "MyProjectGameInstance.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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

void AMyProjectPlayer::BeginPlay()
{
	Super::BeginPlay();

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

		if (State == Protocol::MOVE_STATE_RUN)
		{
			SetActorRotation(FRotator(0, DestInfo->yaw(), 0));
			AddMovementInput(GetActorForwardVector());  // 바라보는 방향으로 가게 된다. 그러면 안된다.
			                                            // 도착 목적지에서 현재 위치를 빼서 노말라이즈한 것을 이동 벡터로 해야 한다.
		}
		else
		{

		}
	}
}

bool AMyProjectPlayer::IsMyPlayer()
{
	bool IsMyPlayer = false;;
	if (Cast<AMyProjectMyPlayer>(this) != nullptr)
		IsMyPlayer = true;


	return IsMyPlayer;
}

void AMyProjectPlayer::SetMoveState(Protocol::MoveState State)
{
	if (PlayerInfo->state() == State)
		return;

	PlayerInfo->set_state(State);

	// TODO
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

	// Dest에 최종 상태 복사.
	DestInfo->CopyFrom(Info);

	// 상태만 바로 적용하자.
	SetMoveState(Info.state());
}
