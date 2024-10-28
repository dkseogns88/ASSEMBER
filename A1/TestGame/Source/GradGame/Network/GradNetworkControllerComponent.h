// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "GradNetworkControllerComponent.generated.h"


UCLASS()
class GRADGAME_API UGradNetworkControllerComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UGradNetworkControllerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Pawn¿« Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grad|Pawn")
	TSubclassOf<APawn> PawnClass;
	
	

};
