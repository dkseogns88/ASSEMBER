// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GradGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GRADGAME_API UGradGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	/**
	* UGameInstance's interfaces
	*/
	virtual void Init() override;
	virtual void Shutdown() override;
};
