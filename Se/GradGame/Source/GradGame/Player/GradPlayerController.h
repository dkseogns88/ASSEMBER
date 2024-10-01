// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "GradPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GRADGAME_API AGradPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	AGradPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
