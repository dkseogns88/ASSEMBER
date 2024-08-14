

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
#include "A1PlayerController.generated.h"

struct FInputActionValue;

UCLASS()
class A1_API AA1PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AA1PlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	void StateTick();
	void SendTick(float DeltaTime);
	void SendMoveState();
	void SendAllState();
	

private:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Turn(const FInputActionValue& InputValue);
	void Input_Jump(const FInputActionValue& InputValue);
	void Input_Sprint(const FInputActionValue& InputValue);
	void Input_Crouch(const FInputActionValue& InputValue);
	void Input_Aim(const FInputActionValue& InputValue);
	

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AA1MyPlayer> A1MyPlayer;

public:
	UPROPERTY()
	UTimelineComponent* AimTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* AimCurve;

	UFUNCTION()
	void AimTimelineProgress(float Value);

public:
	UPROPERTY()
	UUserWidget* CreatedWidget;

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.04f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	// Cache
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	// Dirty Flag Test
	FVector2D LastDesiredInput;
	bool bLastInputJump = false;
};
