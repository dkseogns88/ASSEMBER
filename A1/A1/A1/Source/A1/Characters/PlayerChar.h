#pragma once

#include "CoreMinimal.h"
#include "BaseChar.h"
#include "PlayerChar.generated.h"

/**
 * 
 */
UCLASS()
class A1_API APlayerChar : public ABaseChar
{

	GENERATED_BODY()
	
public:
	APlayerChar();

protected:
	virtual void BeginPlay() override;
	

	

	
	UFUNCTION()
	void Aimingchanged();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void StartRoll();

	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void EndRoll();

	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void UseSkill(bool UsingSkill);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsUsingSkill;

	UPROPERTY(ReplicatedUsing = Aimingchanged)
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsMovingBackward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsJumping;

	virtual void Landed(const FHitResult& Hit) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsDamaged;


	
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;

	
	virtual void TurnLeft(float Value) override;
	virtual void TurnRight(float Value) override;

	bool IsAiming() const { return bIsAiming; }
	void SetAiming(bool bNewAiming);
	bool IsMoving() const { return bIsMoving; }
	FVector2D GetMovementInput() const { return MovementInput; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;
	void SetMovementSpeed(float NewSpeed);
	virtual void Jump() override;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void IsDamaged(bool Damaged);

	FTimerHandle DamageResetTimerHandle;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	
	FVector2D MovementInput;
	
};
