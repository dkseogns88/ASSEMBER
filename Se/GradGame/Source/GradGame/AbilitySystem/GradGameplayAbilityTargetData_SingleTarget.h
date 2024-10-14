#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GradGameplayAbilityTargetData_SingleTarget.generated.h"

USTRUCT()
struct FGradGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:
	FGradGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGradGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	/** 탄약 ID (카트리지) */
	UPROPERTY()
	int32 CartridgeID;
};