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

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void StartRoll();

	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void EndRoll();

	UFUNCTION(BlueprintCallable, Category = "Character Actions")
	void UseSkill(int32 SkillIndex);

	
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;

	
	virtual void TurnLeft(float Value) override;
	virtual void TurnRight(float Value) override;


	
	virtual void Jump() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void Roll();
};
