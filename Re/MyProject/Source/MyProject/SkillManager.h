// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillManager.generated.h"



class USkill;
/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API USkillManager : public UObject
{

    GENERATED_BODY()

public:
    USkillManager();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skills")
    TMap<FName, USkill*> Skills;

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void AddSkill(USkill* NewSkill);

    UFUNCTION(BlueprintCallable, Category = "Skills")
    USkill* GetSkillByName(FName SkillName) const;
};

