// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillManager.h"
#include "Skill.h"

USkillManager::USkillManager()
{
    // 기본 스킬 초기화 (필요시)
}

void USkillManager::AddSkill(USkill* NewSkill)
{
    if (NewSkill)
    {
        Skills.Add(NewSkill->SkillName, NewSkill);
    }
}

USkill* USkillManager::GetSkillByName(FName SkillName) const
{
    USkill* const* FoundSkill = Skills.Find(SkillName);
    if (FoundSkill)
    {
        UE_LOG(LogTemp, Log, TEXT("Skill found: %s"), *SkillName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Skill not found in SkillManager: %s"), *SkillName.ToString());
    }
    return FoundSkill ? *FoundSkill : nullptr;
}