// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSkill.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Characters/PlayerChar.h"
#include "Characters/Monster.h"
#include "Network/A1NetworkManager.h"
// Sets default values
ABaseSkill::ABaseSkill()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();

	
}

void ABaseSkill::InitializeSkill(AActor* SkillCaster, FVector SkillLocation, float SkillRadius, float DamageAmount)
{
    Caster = SkillCaster;
    Radius = SkillRadius;
    Damage = DamageAmount;

    PerformDamage(Caster->GetActorLocation());
    EndSkill();
}

void ABaseSkill::PerformDamage(const FVector& DamageCenter)
{
    if (!Caster) {
        return;
    }

    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != Caster && (Actor->GetActorLocation() - DamageCenter).Size() <= Radius)
        {
            AMonster* HitMonster = Cast<AMonster>(Actor);
            if (HitMonster)
            {
                HitMonster->TakeDMG(Damage);

                APlayerChar* MyCharacter = Cast<APlayerChar>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

                //서버처리
            }
        }
    }
}

void ABaseSkill::EndSkill()
{
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