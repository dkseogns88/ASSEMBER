// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceCustom.h"
#include "../Characters/PlayerChar.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimInstanceCustom::UAnimInstanceCustom(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) 
{

}

void UAnimInstanceCustom::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Character = Cast<ABaseChar>(GetOwningActor());

    if (Character)
        MovementComponent = Character->GetCharacterMovement();
}


void UAnimInstanceCustom::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
		return;

	if (MovementComponent == nullptr)
		return;

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = (GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);
	bIsFalling = MovementComponent->IsFalling();

	

}