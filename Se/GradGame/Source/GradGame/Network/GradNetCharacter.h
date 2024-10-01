// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.h"
#include "GradNetCharacter.generated.h"

UCLASS()
class GRADGAME_API AGradNetCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	AGradNetCharacter();
	/**
	* Blueprint ���Ǹ� ���� Equip/Unequip �Լ�
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "NetEquipped"))
	void K2_NetOnEquipped();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
