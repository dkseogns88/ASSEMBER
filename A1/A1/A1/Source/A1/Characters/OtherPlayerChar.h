 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerChar.h"
#include "OtherPlayerChar.generated.h"

/**
 * 
 */
UCLASS()
class A1_API AOtherPlayerChar : public ABaseChar
{
	GENERATED_BODY()

public:
	AOtherPlayerChar();
	virtual ~AOtherPlayerChar();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetAiming(bool bNewAiming);

	void SpawnMuzzleFlash();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* MuzzleFlashEffect;

	
private:
	bool bIsAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FirstPersonMesh;

};
