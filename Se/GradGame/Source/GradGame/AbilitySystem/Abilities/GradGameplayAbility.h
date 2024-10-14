// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GradGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EGradAbilityActivationPolicy : uint8
{
	/** Input�� Trigger �Ǿ��� ��� (Presssed/Released) */
	OnInputTriggered,
	/** Input�� Held�Ǿ� ���� ��� */
	WhileInputActive,
	/** avatar�� �����Ǿ��� ���, �ٷ� �Ҵ� */
	OnSpawn,
};

/** forward declarations */
class UGradAbilityCost;

/**
 * 
 */
UCLASS()
class GRADGAME_API UGradGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGradGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* UGameplayAbility interfaces
	*/
	//virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;


	/** ���� GA�� Ȱ��ȭ���� ��å */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grad|AbilityActivation")
	EGradAbilityActivationPolicy ActivationPolicy;

	/** ability costs to apply GradGameplayAbility separately */
	//UPROPERTY(EditDefaultsOnly, Instanced, Category = "Grad|Costs")
	//TArray<TObjectPtr<UGradAbilityCost>> AdditionalCosts;
};
