// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USkill : public UObject
{
    GENERATED_BODY()

public:
    USkill();

    void Initialize(FName InSkillName, float InSkillRadius, float InSkillDamage);

    void ExecuteSkill(AActor* Instigator);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
    FName SkillName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
    float SkillRadius;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
    float SkillDamage;

   
};