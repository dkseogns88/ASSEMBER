#include "Animation/A1AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/A1PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

UA1AnimInstance::UA1AnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UA1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();


	Character = Cast<AA1Character>(GetOwningActor());

	if (Character)
		MovementComponent = Character->GetCharacterMovement();

	Controller = Cast<AA1PlayerController>(UGameplayStatics::GetPlayerController(this, 0));

}

void UA1AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
		return;

	if (MovementComponent == nullptr)
		return;

	if (Controller == nullptr)
		return;

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = (GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);

	bIsFalling = MovementComponent->IsFalling();

	// Direction
	float DirectionAngle = CalculateDirection(FVector(Velocity.X, Velocity.Y, 0.f), Character->GetActorRotation());
	Direction = UKismetMathLibrary::NormalizeAxis(DirectionAngle);

	// Crouch
	bCrouch = Character->bCrouch;

	// Overlay State
	OverlayState = Character->OverlayState;

	// Aiming
	bAiming = Character->bAiming;

	if (Character->IsMyPlayer() == false)
	{
		// 다른 플레이어
		float InterpSpeed = 5.0f;
		FRotator BeforeAimOffset = AimOffset;

		/*AimOffset.Roll = Character->AimPitch;
		AimOffset.Pitch = 0.f;
		AimOffset.Yaw = -1 * Character->AimYaw;*/
	
		float YawInterpValue = 10.f;
		AimOffset.Roll = FMath::FInterpTo(BeforeAimOffset.Roll, Character->AimPitch, DeltaSeconds, InterpSpeed);
		AimOffset.Pitch = 0.f;
		AimOffset.Yaw = FMath::FInterpTo(BeforeAimOffset.Yaw, -1 * Character->AimYaw, DeltaSeconds, InterpSpeed);
	}
	else
	{
		// 나의 캐릭터
		FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetActorRotation(), Character->GetControlRotation());
		AimOffset.Roll = DeltaRotator.Pitch;
		AimOffset.Pitch = 0.f;
		AimOffset.Yaw = -1 * DeltaRotator.Yaw;
	}

}