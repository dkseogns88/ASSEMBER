// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GradInventoryItemDefinition.h"
#include "GradInventoryFragment_SetStats.generated.h"

UCLASS()
class GRADGAME_API UGradInventoryFragment_SetStats : public UGradInventoryItemFragment
{
	GENERATED_BODY()
public:

	virtual void OnInstanceCreated(UGradInventoryItemInstance* Instance) const override;

	/** InitialItemStats gives constructor's parameters for GradGameplayTagStackContainer */
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;
};
