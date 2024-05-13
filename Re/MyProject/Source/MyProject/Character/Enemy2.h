// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequence.h"
#include "Components/BoxComponent.h"
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

	// Function to get the box component's corner points
	TArray<FVector> GetBoxCornerPoints() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Info")
	FString EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Info")
	float Health;

	// Box Component for collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;


	void CheckAndTeleport();


	void CheckMeshSetup();


};
