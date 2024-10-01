

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "A1GameplayTags.h"
#include "Character/A1MyPlayer.h"
#include "A1AnimInstance.generated.h"

UCLASS()
class A1_API UA1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UA1AnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly) // TODO: A1MyPlayer로 바꾸어야 하나?
	TObjectPtr<class AA1Character> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AA1PlayerController> Controller;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float GroundSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bShouldMove = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bIsFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float Direction = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bCrouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	FRotator AimOffset = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	EOverlayStates OverlayState = EOverlayStates::Default;

};