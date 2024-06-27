// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkill.h"
#include "GunSkill.generated.h"

/**
 * 
 */
UCLASS()
class A1_API AGunSkill : public ABaseSkill
{
    GENERATED_BODY()

public:
    AGunSkill();

    virtual void InitializeSkill(AActor* SkillCaster, FVector SkillLocation, float SkillRadius, float DamageAmount) override;
    virtual void PerformDamage(const FVector& DamageCenter) override;

protected:
    virtual void BeginPlay() override;
};