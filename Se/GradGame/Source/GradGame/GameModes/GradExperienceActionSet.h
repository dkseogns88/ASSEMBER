// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GradExperienceActionSet.generated.h"

class UGameFeatureAction;

UCLASS()
class GRADGAME_API UGradExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UGradExperienceActionSet();

	/**
	 * member variables
	 */
	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};