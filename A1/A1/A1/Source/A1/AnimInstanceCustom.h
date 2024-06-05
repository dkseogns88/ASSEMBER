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

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovementInput(FVector2D NewMovementInput);

    void NativeInitializeAnimation() override;

    void NativeUpdateAnimation(float DeltaSeconds) override;

    void SetAiming(bool bAiming);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsRolling;

    void SetRolling(bool bRolling);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    FVector2D MovementInput;
};

