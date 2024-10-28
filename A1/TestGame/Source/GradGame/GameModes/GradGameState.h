// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GradGameState.generated.h"

class UGradExperienceManagerComponent;

/**
 * 
 */
UCLASS()
class GRADGAME_API AGradGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AGradGameState();

public:
	UPROPERTY()
	TObjectPtr<UGradExperienceManagerComponent> ExperienceManagerComponent;
};
