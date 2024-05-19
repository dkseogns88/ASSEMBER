// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProjectPlayer.h"
#include "InputActionValue.h"
#include "MyProjectMyPlayer.generated.h"


UCLASS()
class MYPROJECT_API AMyProjectMyPlayer : public AMyProjectPlayer
{
	GENERATED_BODY()

public:
	
	AMyProjectMyPlayer();

	bool IsAiming() const { return bIsAiming; }
	void SetAiming(bool bNewAiming);

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** First person mesh (arms only) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FirstPersonMesh;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Input_Jump(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// To add mapping context
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void StateTick();
	void SendTick(float DeltaTime);
	void Send_Idle_Move();
	void Send_Jump();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


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

	// 캐릭터의 조준 상태
	UPROPERTY(ReplicatedUsing = OnRep_Aimingchanged)
	bool bIsAiming = false;

	// 조준 상태가 변경될 때 호출될 함수
	UFUNCTION()
	void OnRep_Aimingchanged();

	
};
