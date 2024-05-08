// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequence.h"
#include "Components/CapsuleComponent.h"
#include "Enemy2.generated.h"

UCLASS()
class MYPROJECT_API AEnemy2 : public ACharacter
{

	GENERATED_BODY()


private:


public:
	// Sets default values for this character's properties
	AEnemy2();
	void UpdateAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void CheckAndTeleport();


	void CheckMeshSetup();


};
