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

	virtual void InitializeSkill(AActor* SkillCaster, float SkillRadius, float DamageAmount);
	virtual void ApplyDamage();
	

	
	

protected:
	virtual void BeginPlay() override;
	void EndSkill();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	AActor* Caster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Damage;

	
	

public:
	// Delegate to notify the controller when the skill ends
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEnd);
	FOnSkillEnd OnSkillEnd;
};