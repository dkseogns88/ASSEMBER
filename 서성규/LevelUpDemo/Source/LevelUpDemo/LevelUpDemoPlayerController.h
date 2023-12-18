// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LevelUpDemoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEVELUPDEMO_API ALevelUpDemoPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;
};
