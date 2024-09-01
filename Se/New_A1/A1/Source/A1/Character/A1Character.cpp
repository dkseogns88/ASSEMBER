


#include "Character/A1Character.h"
#include "Character/A1MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/A1PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "A1WeaponComponent.h"
#include "Components/ChildActorComponent.h"
#include "A1Weapon.h"
#include "Network/A1NetworkManager.h"

AA1Character::AA1Character()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 310.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	DestInfo = new Protocol::PosInfo();
	StatInfo = new Protocol::StatInfo();
	StateInfo = new Protocol::StateInfo();

	// Crouch
	CrouchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchTimeline"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/Etc/CV_AimCrouch.CV_AimCrouch"));
	if (curve.Succeeded())
	{
		CrouchCurve = curve.Object;
	}
	// WeaponComponent
	static ConstructorHelpers::FClassFinder<UA1WeaponComponent> WeaponBP(TEXT("/Game/WeaponSystem/AC_Weapon.AC_Weapon_C"));
	if (WeaponBP.Succeeded())
	{
		ObjectWeaponComponent = WeaponBP.Class;
	}


	// ChildActor
	HandObject = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorComponent"));
	HandObject->SetupAttachment(GetMesh());
}

AA1Character::~AA1Character()
{
	delete ObjectInfo;
	delete PosInfo;
	delete DestInfo;
	delete StatInfo;
	delete StateInfo;
	ObjectInfo = nullptr;
	PosInfo = nullptr;
	DestInfo = nullptr;
	StatInfo = nullptr;
	StateInfo = nullptr;
}

void AA1Character::BeginPlay()
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
	
	{
		if (CrouchCurve)
		{
			FOnTimelineFloat ProgressFunction;
			ProgressFunction.BindUFunction(this, FName("CrouchTimelineProgress"));

			CrouchTimeline->AddInterpFloat(CrouchCurve, ProgressFunction);
			CrouchTimeline->SetLooping(false);
		}
	}

	{
		WeaponComponent = NewObject<UA1WeaponComponent>(this, ObjectWeaponComponent);
		//WeaponComponent = Cast<UA1WeaponComponent>(ObjectWeaponComponent.GetDefaultObject());
	}
}

void AA1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMyPlayer() == false)
	{
		// 좌표에서 좌표로
		// CurrentToDest(DeltaTime);
		// 단순 방향벡터
		CurrentToDirection(DeltaTime);
	}

}

float AA1Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AA1Weapon* AttackerWeapon = Cast<AA1Weapon>(DamageCauser);
	AA1Character* Attacker = Cast<AA1Character>(AttackerWeapon->GetOwner());
	if (Attacker == this) return -1.f;

	Protocol::C_ATTACK AttackPkt;
	Protocol::AttackInfo* Info = AttackPkt.mutable_info();
	Info->set_attack_object_id(Attacker->GetStatInfo()->object_id());
	Info->set_hit_object_id(ObjectInfo->object_id());
	Info->set_world_location_x(AttackerWeapon->WorldLocationToServer.X);
	Info->set_world_location_y(AttackerWeapon->WorldLocationToServer.Y);
	Info->set_world_location_z(AttackerWeapon->WorldLocationToServer.Z);
	Info->set_forward_vector_x(AttackerWeapon->ForwardVectorToServer.X);
	Info->set_forward_vector_y(AttackerWeapon->ForwardVectorToServer.Y);
	Info->set_forward_vector_z(AttackerWeapon->ForwardVectorToServer.Z);

	UA1NetworkManager* NetworkManager = GetGameInstance()->GetSubsystem<UA1NetworkManager>();
	if (NetworkManager) NetworkManager->SendPacket(AttackPkt);

	return 0.0f;
}

void AA1Character::PickupWeapon(FName WeaponName, bool& Success)
{
	if (WeaponComponent)
	{
		WeaponComponent->Give(WeaponName);
		FA1Weapons Weapon = WeaponComponent->CycleUp();
		AttachToHand(Weapon.Overlay, Weapon.BP_Actor, Weapon.SocketTransform);
		Success = true;
		return;
	}
}

FName AA1Character::GetWeaponName()
{
	return WeaponComponent->ActiveWeapon;
}

void AA1Character::GetCamera(FVector& WorldLocation, FVector& ForwardVector)
{}

void AA1Character::AttachToHand(EOverlayStates Overlay, TSoftClassPtr<AActor> BP_Actor, FTransform SocketTransform)
{
	bool IsValidActor = UKismetSystemLibrary::IsValidSoftClassReference(BP_Actor);
	if (IsValidActor)
	{
		UClass* ActorClass =  UKismetSystemLibrary::LoadClassAsset_Blocking(BP_Actor);
		if (ActorClass)
		{
			HandObject->SetChildActorClass(ActorClass);

			FAttachmentTransformRules TransfromRule(EAttachmentRule::SnapToTarget, true);
			HandObject->AttachToComponent(GetMesh(), TransfromRule, FName("hand_r"));

			//FHitResult* OutSweepHitResult;
			//HandObject->SetRelativeTransform(SocketTransform, true, OutSweepHitResult, ETeleportType::TeleportPhysics);
			HandObject->SetRelativeTransform(SocketTransform);
			
			if (AA1Weapon* WeaponActor = Cast<AA1Weapon>(HandObject->GetChildActor()))
			{
				WeaponActor->SetOwner(this);
			}

			OverlayState = Overlay;
			Protocol::OverlayState State;
			switch (OverlayState)
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
			SetOverlayState(State);
		}
		else
		{
			HandObject->SetChildActorClass(nullptr);
		}
	}
}

void AA1Character::CrouchTimelineProgress(float Value)
{
	float HalfHeight = FMath::Lerp(90.f, 70.f, Value);
	GetCapsuleComponent()->SetCapsuleHalfHeight(HalfHeight, true);
}


bool AA1Character::IsMyPlayer()
{
	return Cast<AA1MyPlayer>(this) != nullptr;

}

void AA1Character::SetMoveState(Protocol::MoveState State)
{
	if (PosInfo->move_state() == State)
		return;

	PosInfo->set_move_state(State);
}

void AA1Character::SetLocomotionState(Protocol::LocomotionState State)
{
	if (StateInfo->locomotion_state() == State)
		return;

	StateInfo->set_locomotion_state(State);
}

void AA1Character::SetOverlayState(Protocol::OverlayState State)
{
	if (StateInfo->overlay_state() == State)
		return;

	StateInfo->set_overlay_state(State);
}

void AA1Character::SetObjectInfo(const Protocol::ObjectInfo& Info)
{
	if (ObjectInfo->object_id() != 0)
	{
		assert(ObjectInfo->object_id() == Info.object_id());
	}

	ObjectInfo->CopyFrom(Info);
}

void AA1Character::SetPosInfo(const Protocol::PosInfo& Info)
{
	if (PosInfo->object_id() != 0)
	{
		assert(PosInfo->object_id() == Info.object_id());
	}

	PosInfo->CopyFrom(Info);
	
	AimPitch = PosInfo->aim_pitch();
	AimYaw = PosInfo->aim_yaw();
}

void AA1Character::SetDestInfo(const Protocol::PosInfo& Info)
{
	if (PosInfo->object_id() != 0)
	{
		assert(PosInfo->object_id() == Info.object_id());
	}

	DestInfo->CopyFrom(Info);

	SetMoveState(Info.move_state());

}

void AA1Character::SetStatInfo(const Protocol::StatInfo& Info)
{
	if (StatInfo->object_id() != 0)
	{
		assert(StatInfo->object_id() == Info.object_id());
	}

	StatInfo->CopyFrom(Info);



}

void AA1Character::SetStateInfo(const Protocol::StateInfo& Info)
{
	if (StateInfo->object_id() != 0)
	{
		assert(StateInfo->object_id() == Info.object_id());
	}

	Protocol::OverlayState Before_Overlay_State = StateInfo->overlay_state();
	StateInfo->CopyFrom(Info);

	if (IsMyPlayer() == false)
	{
		const Protocol::LocomotionState Locomotion_State = StateInfo->locomotion_state();
		if (Locomotion_State == Protocol::LOCOMOTION_STATE_CROUCH)
		{
			bCrouch = true;
			CrouchTimeline->Play();
		}
		else if (Locomotion_State == Protocol::LOCOMOTION_STATE_IDLE 
			&& BeforeLocomotion_State == Protocol::LOCOMOTION_STATE_CROUCH)
		{
			bCrouch = false;
			CrouchTimeline->ReverseFromEnd();
		}
		BeforeLocomotion_State = Locomotion_State;



		const Protocol::OverlayState Overlay_State = StateInfo->overlay_state();
		switch (Overlay_State)
		{
		case Protocol::OverlayState::OVERLAY_STATE_PISTOL:
			OverlayState = EOverlayStates::Pistol;
			break;
		case Protocol::OverlayState::OVERLAY_STATE_RIFLE:
			OverlayState = EOverlayStates::Rifle;
			break;
		default:
			OverlayState = EOverlayStates::Default;
			break;
		}
		bAiming = StateInfo->aim();

		if (Before_Overlay_State != Overlay_State)
		{
			FA1Weapons Outputs = WeaponComponent->CycleUp();
			AttachToHand(Outputs.Overlay, Outputs.BP_Actor, Outputs.SocketTransform);
		}
	}
}

void AA1Character::CurrentToDest(float DeltaTime)
{
	const Protocol::MoveState Move_State = DestInfo->move_state();
	FRotator NowRotation = GetActorRotation();
	FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);

	FVector CurrentLocation = GetActorLocation();
	FVector TargetDestination = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());
	FVector Direction = (TargetDestination - CurrentLocation).GetSafeNormal();

	float YawInterpValue = 30.f;
	if (Move_State == Protocol::MOVE_STATE_IDLE)
	{
		FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
		SetActorRotation(NewRotation);
	}
	else if (Move_State == Protocol::MOVE_STATE_WALK)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
		SetActorRotation(NewRotation);

		if (FVector::Dist(CurrentLocation, TargetDestination) > 10.0f)  // 어느 정도의 오차 허용
		{
			AddMovementInput(Direction);
		}
	}
	else if (Move_State == Protocol::MOVE_STATE_RUN)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
		SetActorRotation(NewRotation);

		if (FVector::Dist(CurrentLocation, TargetDestination) > 10.0f)  // 어느 정도의 오차 허용
		{
			AddMovementInput(Direction);
		}
	}

	float Distance = FVector::Dist(GetActorLocation(), FVector(DestInfo->x(), DestInfo->y(), DestInfo->z()));
	if (Distance >= 200.f)
	{
		FRotator NewRotation = FRotator(0, DestInfo->yaw(), 0);

		SetActorLocation(FVector(DestInfo->x(), DestInfo->y(), DestInfo->z()));
		SetActorRotation(NewRotation);
	}
}

void AA1Character::CurrentToDirection(float DeltaTime)
{
	const Protocol::MoveState Move_State = DestInfo->move_state();
	FRotator NowRotation = GetActorRotation();
	FRotator TargetRotation = FRotator(0, DestInfo->yaw(), 0);


	float YawInterpValue = 30.f;
	if (Move_State == Protocol::MOVE_STATE_IDLE)
	{
		FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
		if (NewRotation.Equals(TargetRotation, 1.0f) == false)
		{
			SetActorRotation(NewRotation);
		}
		else
		{
			SetActorRotation(TargetRotation);

		}
	}
	else if (Move_State == Protocol::MOVE_STATE_WALK)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
		if (NewRotation.Equals(TargetRotation, 1.0f) == false)
		{
			SetActorRotation(NewRotation);
		}
		else
		{
			SetActorRotation(TargetRotation);

		}
		FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
		AddMovementInput(ForwardDirection);

	}
	else if (Move_State == Protocol::MOVE_STATE_RUN)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
		
		if (NewRotation.Equals(TargetRotation, 1.0f) == false) 
		{
			SetActorRotation(NewRotation);
		}
		else
		{
			SetActorRotation(TargetRotation);

		}

		FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
		AddMovementInput(ForwardDirection);
	}
	else if (Move_State == Protocol::MoveState::MOVE_STATE_JUMP)
	{
		Jump();
		FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, YawInterpValue);
		if (NewRotation.Equals(TargetRotation, 1.0f) == false)
		{
			SetActorRotation(NewRotation);
		}
		else
		{
			SetActorRotation(TargetRotation);

		}

		FVector ForwardDirection = FVector(DestInfo->d_x(), DestInfo->d_y(), DestInfo->d_z());
		AddMovementInput(ForwardDirection);
	}

	float Distance = FVector::Dist(GetActorLocation(), FVector(DestInfo->x(), DestInfo->y(), DestInfo->z()));
	if (Distance >= 200.f)
	{

		SetActorLocation(FVector(DestInfo->x(), DestInfo->y(), DestInfo->z()));
		SetActorRotation(TargetRotation);
	}
}
