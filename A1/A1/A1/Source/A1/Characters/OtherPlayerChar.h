 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerChar.h"
#include "OtherPlayerChar.generated.h"

/**
 * 
 */
UCLASS()
class A1_API AOtherPlayerChar : public ABaseChar
{
	GENERATED_BODY()

public:
	AOtherPlayerChar();
	virtual ~AOtherPlayerChar();

	void PlayZoom(bool IsZoom);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
};