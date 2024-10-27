// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "GradGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class GRADGAME_API UGradGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
public:
	static UGradGameplayCueManager* Get();

	UGradGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void RefreshGameplayCuePrimaryAsset();
};
