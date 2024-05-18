

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
	void Send_Idle_Move();
	void Send_Jump();

private:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Turn(const FInputActionValue& InputValue);
	void Input_Jump(const FInputActionValue& InputValue);
	void Input_Mouse(const FInputActionValue& InputValue);

public:
	UAnimMontage* GetZoomMontage() const { return ZoomMontage.Get(); }

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AA1MyPlayer> A1MyPlayer;
	

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	// Cache
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	// Dirty Flag Test
	FVector2D LastDesiredInput;

	// Jump Cache
	bool IsJumping = false;
	bool bLastInputJump = false;

	// Turn Cache
	bool bIsTurn = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> ZoomMontage;

};
