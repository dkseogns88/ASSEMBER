// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSkill.h"

AGunSkill::AGunSkill()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGunSkill::BeginPlay()
{
    Super::BeginPlay();
}

void AGunSkill::InitializeSkill(AActor* SkillCaster, FVector SkillLocation, float SkillRadius, float DamageAmount)
{
    Super::InitializeSkill(SkillCaster,SkillLocation, SkillRadius, DamageAmount);
}

void AGunSkill::PerformDamage(const FVector& DamageCenter)
{
    Super::PerformDamage(Caster->GetActorLocation());
}