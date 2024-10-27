// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GradGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "GradAbilityCost.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class GRADGAME_API UGradAbilityCost : public UObject
{
	GENERATED_BODY()
public:
	UGradAbilityCost();

	/**
	 * CheckCost and ApplyCost function signature come from UGameAbility's CheckCost and ApplyCost
	 * - You can think GradAbilityCost as managable-unit to check/apply cost for GameplayAbility
	 */
	virtual bool CheckCost(const UGradGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	virtual void ApplyCost(const UGradGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}
};
