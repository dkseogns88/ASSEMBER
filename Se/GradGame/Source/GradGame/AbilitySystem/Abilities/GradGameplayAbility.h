// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GradGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EGradAbilityActivationPolicy : uint8
{
	/** Input이 Trigger 되었을 경우 (Presssed/Released) */
	OnInputTriggered,
	/** Input이 Held되어 있을 경우 */
	WhileInputActive,
	/** avatar가 생성되었을 경우, 바로 할당 */
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


	/** 언제 GA가 활성화될지 정책 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grad|AbilityActivation")
	EGradAbilityActivationPolicy ActivationPolicy;

	/** ability costs to apply GradGameplayAbility separately */
	//UPROPERTY(EditDefaultsOnly, Instanced, Category = "Grad|Costs")
	//TArray<TObjectPtr<UGradAbilityCost>> AdditionalCosts;
};
