// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GradPawnData.generated.h"

class UGradAbilitySet;
class UGradInputConfig;
class UGradCameraMode;


UCLASS()
class GRADGAME_API UGradPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UGradPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Pawn의 Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grad|Pawn")
	TSubclassOf<APawn> PawnClass;

	/** Camera Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grad|Camera")
	TSubclassOf<UGradCameraMode> DefaultCameraMode;

	/** input configuration used by player controlled pawns to create input mappings and bind input actions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grad|InputConfig")
	TObjectPtr<UGradInputConfig> InputConfig;

	/** Pawn의 NetClass */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grad|NetPawn")
	TSubclassOf<APawn> NetPawnClass;

	/** 해당 Pawn의 Ability System에 허용할 AbilitySet */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grad|Abilities")
	TArray<TObjectPtr<UGradAbilitySet>> AbilitySets;
};
