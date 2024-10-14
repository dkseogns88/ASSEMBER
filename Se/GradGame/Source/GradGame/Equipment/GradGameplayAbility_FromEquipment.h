// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GradGame/AbilitySystem/Abilities/GradGameplayAbility.h"
#include "GradGameplayAbility_FromEquipment.generated.h"

class UGradInventoryItemInstance;
class UGradEquipmentInstance;
/**
 * 
 */
UCLASS()
class GRADGAME_API UGradGameplayAbility_FromEquipment : public UGradGameplayAbility
{
	GENERATED_BODY()
public:
	UGradEquipmentInstance* GetAssociatedEquipment() const;
	//UGradInventoryItemInstance* GetAssociatedItem() const;
};
