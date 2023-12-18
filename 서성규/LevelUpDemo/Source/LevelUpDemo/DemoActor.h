// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelUpGameInstance.h"
#include "GameFramework/Actor.h"
#include "DemoActor.generated.h"


UCLASS()
class LEVELUPDEMO_API ADemoActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADemoActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetSkeletalMesh(USkeletalMesh* Mesh);

	void SetId(int32 id) { m_id = id; }
	void SetGameInstance(ULevelUpGameInstance* inst) { gameInst = inst; }
private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

public:
	UPROPERTY()
	ULevelUpGameInstance* gameInst;

	int32 m_id;
};
