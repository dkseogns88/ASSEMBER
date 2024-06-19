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
    UAnimInstanceCustom(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetAiming(bool bNewAiming) { bIsAiming = bNewAiming; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ABaseChar> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;


protected:
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming = false;

};