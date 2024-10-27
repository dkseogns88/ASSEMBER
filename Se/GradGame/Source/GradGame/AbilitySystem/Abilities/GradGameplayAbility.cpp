// Fill out your copyright notice in the Description page of Project Settings.


#include "GradGameplayAbility.h"
#include "GradAbilityCost.h"

UGradGameplayAbility::UGradGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EGradAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


bool UGradGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) || !ActorInfo)
	{
		return false;
	}

	// verify AdditionalCosts defined in GradGameplayAbility to activate GameplayAbility:
	for (TObjectPtr<UGradAbilityCost> AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			if (!AdditionalCost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
			{
				return false;
			}
		}
	}

	// all cost requipements are meet! ready to activate!
	return true;
}

void UGradGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
	check(ActorInfo);

	// pay any additional cost
	for (TObjectPtr<UGradAbilityCost> AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			AdditionalCost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);
		}
	}
}