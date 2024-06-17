// Fill out your copyright notice in the Description page of Project Settings.


#include "SKill.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Characters/PlayerChar.h"
#include "Characters/Monster.h"


ASKill::ASKill()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}

void ASKill::BeginPlay()
{
	Super::BeginPlay();

  

}

void ASKill::InitializeSkill(AActor* SkillCaster, float SkillRadius, float DamageAmount)
{
	Caster = SkillCaster;
	Radius = SkillRadius;
	Damage = DamageAmount;

	UE_LOG(LogTemp, Log, TEXT("Skill initialized with Radius: %f, Damage: %f"), Radius, Damage);
    ApplyDamage();
    EndSkill();
}


void ASKill::ApplyDamage()
{
    UE_LOG(LogTemp, Log, TEXT("Applying damage"));

    if (!Caster) {
        UE_LOG(LogTemp, Error, TEXT("Invalid Caster reference"));
        return;
    }

    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != Caster && (Actor->GetActorLocation() - Caster->GetActorLocation()).Size() <= Radius)
        {
            AMonster* HitMonster = Cast<AMonster>(Actor);
            if (HitMonster)
            {
                HitMonster->TakeDMG(Damage);
                UE_LOG(LogTemp, Log, TEXT("Damage applied to %s"), *HitMonster->GetName());
            }
        }
    }
}
void ASKill::EndSkill()
{
    UE_LOG(LogTemp, Log, TEXT("Ending skill"));
    APlayerChar* PlayerChar = Cast<APlayerChar>(Caster);
    if (PlayerChar)
    {
        PlayerChar->bIsUsingSkill = false;
    }

    if (OnSkillEnd.IsBound())
    {
        OnSkillEnd.Broadcast();
    }

    Destroy();
}