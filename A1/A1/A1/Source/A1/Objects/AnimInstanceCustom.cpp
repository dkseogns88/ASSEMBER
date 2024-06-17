// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceCustom.h"
#include "../Characters/PlayerChar.h"
#include "../Characters/PlayerChar.h"
void UAnimInstanceCustom::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
}


void UAnimInstanceCustom::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APlayerChar* OwningCharacter = Cast<APlayerChar>(TryGetPawnOwner());
    if (OwningCharacter)
    {
        bIsMoving = OwningCharacter->IsMoving();
        MovementInput = OwningCharacter->GetMovementInput();
        bIsAiming = OwningCharacter->IsAiming();
       
        bIsMovingBackward = OwningCharacter->bIsMovingBackward;
        bIsDamaged = OwningCharacter->bIsDamaged;
        bIsUsingSkill = OwningCharacter->bIsUsingSkill;
        bIsJumping = OwningCharacter->bIsJumping;
    }
}

void UAnimInstanceCustom::SetAiming(bool bAiming)
{
    bIsAiming = bAiming;
}


void UAnimInstanceCustom::SetMovementInput(FVector2D NewMovementInput)
{
    MovementInput = NewMovementInput;
}
void UAnimInstanceCustom::SetIsMovingBackward(bool bMovingBackward)
{
    bIsMovingBackward = bMovingBackward;
}