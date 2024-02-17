// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayerActor.generated.h"

UCLASS()
class SHOOTING_API ABasePlayerActor : public ACharacter
{
	GENERATED_BODY()


public:
	ABasePlayerActor();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
