// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/DelegateCombinations.h"
#include "BaseSkill.generated.h"

UCLASS()
class A1_API ABaseSkill : public AActor
{
    GENERATED_BODY()

public:
    ABaseSkill();

    virtual void InitializeSkill(AActor* SkillCaster, FVector SkillLocation, float SkillRadius, float DamageAmount);
    virtual void PerformDamage(const FVector& DamageLocation);
    virtual void EndSkill();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    AActor* Caster;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Damage;

public:

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEnd);
    FOnSkillEnd OnSkillEnd;

};