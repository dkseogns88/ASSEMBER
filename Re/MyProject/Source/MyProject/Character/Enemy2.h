// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "NPC.h"
#include "Enemy2.generated.h"

UCLASS()
class MYPROJECT_API AEnemy2 : public ANPC
{

	GENERATED_BODY()


public:
	AEnemy2();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
