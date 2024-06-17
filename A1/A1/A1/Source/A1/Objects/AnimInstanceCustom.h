// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceCustom.generated.h"

UCLASS()
class UAnimInstanceCustom : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsAiming;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
    bool bIsMoving;

   
  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsDamaged;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsUsingSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsJumping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsMovingBackward;
   


    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovementInput(FVector2D NewMovementInput);

    void SetIsMovingBackward(bool bIsMovingBackward);

    void NativeInitializeAnimation() override;

   

    void NativeUpdateAnimation(float DeltaSeconds) override;

    void SetAiming(bool bAiming);

   

    void SetRolling(bool bRolling);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    FVector2D MovementInput;
};

