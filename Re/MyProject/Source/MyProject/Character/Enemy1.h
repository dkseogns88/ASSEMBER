// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequence.h"
#include "Enemy1.generated.h"

UCLASS()
class MYPROJECT_API AEnemy1 : public ACharacter
{

	GENERATED_BODY()


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* WalkingAnimation; // Additional animation

public:
	// Sets default values for this character's properties
	AEnemy1();
	void UpdateAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAnimSequence* IdleAnimation;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayIdleAnimation();
	void CheckMeshSetup();


};
