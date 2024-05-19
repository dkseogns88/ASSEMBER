#include "Animation/A1AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/A1Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/A1PlayerController.h"

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
	
	//bIsZoom = Controller->GetRightClick();
}