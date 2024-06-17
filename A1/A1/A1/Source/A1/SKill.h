// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/DelegateCombinations.h"
#include "SKill.generated.h"

UCLASS()
class A1_API ASKill : public AActor
{
	GENERATED_BODY()
	
public:
	ASKill();

	void InitializeSkill(AActor* SkillCaster, float SkillRadius, float DamageAmount);

	

protected:
	virtual void BeginPlay() override;



private:
	void ApplyDamage();
	void EndSkill();

	AActor* Caster;
	float Radius;
	float Damage;

public:
	// Delegate to notify the controller when the skill ends
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEnd);
	FOnSkillEnd OnSkillEnd;
};