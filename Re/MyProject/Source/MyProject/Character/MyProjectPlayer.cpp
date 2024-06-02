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
#include "Net/UnrealNetwork.h"
#include "MyProjectGameInstance.h"
#include "AnimInstanceCustom.h"
#include "..\..\Source\MyProject\AnimInstanceCustom.h"
#include "MyProjectPlayerController.h"


AMyProjectPlayer::AMyProjectPlayer()
{
	PrimaryActorTick.bCanEverTick = true;


	bIsRolling = false;
	bReplicates = true;
	RollDuration = 0.75f; 

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

	StoredForwardInput = 0.0f;
	StoredRightInput = 0.0f;
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

void AMyProjectPlayer::SetRolling(bool bNewRolling)
{
	if (bIsRolling != bNewRolling)
	{
		bIsRolling = bNewRolling;
		OnRep_RollingChanged();
	}
}

void AMyProjectPlayer::StartRoll(float ForwardInput, float RightInput)
{
	if (bIsRolling)
	{
		return;
	}
	bIsRolling = true;
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;

	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	if (ForwardInput > 0.1f)
	{
		if (RightInput > 0.1f)
		{
			RollDirection = (ForwardVector + RightVector).GetSafeNormal(); // 앞-오른쪽
		}
		else if (RightInput < -0.1f)
		{
			RollDirection = (ForwardVector - RightVector).GetSafeNormal(); // 앞-왼쪽
		}
		else
		{
			RollDirection = ForwardVector; // 앞
		}
	}
	else if (ForwardInput < -0.1f)
	{
		if (RightInput > 0.1f)
		{
			RollDirection = (-ForwardVector + RightVector).GetSafeNormal(); // 뒤-오른쪽
		}
		else if (RightInput < -0.1f)
		{
			RollDirection = (-ForwardVector - RightVector).GetSafeNormal(); // 뒤-왼쪽
		}
		else
		{
			RollDirection = -ForwardVector; // 뒤
		}
	}
	else
	{
		if (RightInput > 0.1f)
		{
			RollDirection = RightVector; // 오른쪽
		}
		else if (RightInput < -0.1f)
		{
			RollDirection = -RightVector; // 왼쪽
		}
		else
		{
			RollDirection = ForwardVector; // 기본 앞 방향
		}
	}
	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->bIsRolling = true;
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Roll, this, &AMyProjectPlayer::EndRoll, RollDuration, false);
}

void AMyProjectPlayer::EndRoll()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Roll);

	GetCharacterMovement()->StopMovementImmediately();
	bIsRolling = false;

	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->bIsRolling = false;
	}
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
	if (bIsRolling)
	{
		AddMovementInput(RollDirection, 1.0f);
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
			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaSeconds, 5.0f); // 보간 속도는 5.0f로 설정
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		}
		else if (State == Protocol::MOVE_STATE_JUMP)
		{
			// 이전 상태가 점프가 아닌 경우에만 점프를 실행합니다.
			if (PreviousState != Protocol::MOVE_STATE_JUMP)
			{
				Jump();
			}

			FRotator NowRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

			FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaSeconds, 5.0f); // 보간 속도는 5.0f로 설정
			SetActorRotation(NewRotation);

			FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
			AddMovementInput(ForwardDirection);
		}

		// 현재 상태를 이전 상태로 업데이트합니다.
		PreviousState = State;
	}

}

void AMyProjectPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyProjectPlayer, bIsRolling);
}

void AMyProjectPlayer::OnRep_RollingChanged()
{
	UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->bIsRolling = bIsRolling;
		UE_LOG(LogTemp, Log, TEXT("Rolling state updated in animation blueprint to: %s"), bIsRolling ? TEXT("True") : TEXT("False"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UAnimInstanceCustom"));
	}

	if (bIsRolling)
	{
		StartRoll(StoredForwardInput, StoredRightInput);
	}
	else
	{
		EndRoll();
	}

}

bool AMyProjectPlayer::IsMyPlayer()
{
	if (Cast<AMyProjectMyPlayer>(this) != nullptr)
		return true;


	return false;
}

void AMyProjectPlayer::MoveForward(float Value)
{
	StoredForwardInput = Value;
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMyProjectPlayer::MoveRight(float Value)
{
	StoredRightInput = Value;
	AddMovementInput(GetActorRightVector() * Value);
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
