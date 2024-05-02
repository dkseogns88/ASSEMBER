// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequence.h"
#include "Components/CapsuleComponent.h"
#include "Enemy1.generated.h"

UCLASS()
class MYPROJECT_API AEnemy1 : public ACharacter
{

	GENERATED_BODY()


private:
	

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




	
	void CheckMeshSetup();


};
