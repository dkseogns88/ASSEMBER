#pragma once

#include "CoreMinimal.h"
#include "BaseChar.h"
#include "PlayerChar.generated.h"



class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class A1_API APlayerChar : public ABaseChar
{

	GENERATED_BODY()
	
public:
	APlayerChar();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void StartRoll();

	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void EndRoll();

	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void UseSkillAnim(bool UsingSkill);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsUsingSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsDamaged;


	UPROPERTY(ReplicatedUsing = Aimingchanged)
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsMovingBackward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext1P;

	virtual void Landed(const FHitResult& Hit) override;

	

	

	bool IsAiming() const { return bIsAiming; }
	void SetAiming(bool bNewAiming);
	bool IsMoving() const { return bIsMoving; }
	FVector2D GetMovementInput() const;
	
	void SetMovementSpeed(float NewSpeed);
	virtual void Jump() override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void IsDamaged(bool Damaged);

	FTimerHandle DamageResetTimerHandle;
protected:
	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& Value);

	
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Aimingchanged();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;



	FVector2D MovementInput;
	
	/////////////////////////////////////
	////////////	¼­¹ö    /////////////
	/////////////////////////////////////


public:
	class UA1NetworkManager* GetNetworkManager() const;

private:
	void MoveCache();

	void StateTick();
	void SendTick(float DeltaTime);

	void Send_Idle_Move();

	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	FVector2D LastDesiredInput;

	const float MOVE_PACKET_SEND_DELAY = 0.2f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

};
