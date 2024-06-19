// Fill out your copyright notice in the Description page of Project Settings.


#include "BombSkill.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Characters/PlayerChar.h"
#include "Characters/Monster.h"


ABombSkill::ABombSkill()
{
    PrimaryActorTick.bCanEverTick = true;

    BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
    RootComponent = BombMesh;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bAutoActivate = false;
}

void ABombSkill::BeginPlay()
{
    Super::BeginPlay();
}

void ABombSkill::InitializeSkill(AActor* SkillCaster, FVector TargetLocation, float SkillRadius, float DamageAmount)
{
    Caster = SkillCaster;
    Radius = SkillRadius;
    Damage = DamageAmount;
    ImpactLocation = TargetLocation;

    UE_LOG(LogTemp, Log, TEXT("Bomb skill initialized with Radius: %f, Damage: %f at Location: %s"), Radius, Damage, *ImpactLocation.ToString());

    // 폭탄이 떨어지면 데미지를 적용하도록 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Damage, this, &ABombSkill::ApplyDamage, 0.5f, false);
}

void ABombSkill::ThrowBomb(FVector LaunchVelocity)
{
    ProjectileMovement->SetVelocityInLocalSpace(LaunchVelocity);
    ProjectileMovement->Activate();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Damage, this, &ABombSkill::ApplyDamage, 3.0f, false);
}

void ABombSkill::ApplyDamage()
{
    UE_LOG(LogTemp, Log, TEXT("Applying bomb damage"));

    if (!Caster) {
        UE_LOG(LogTemp, Error, TEXT("Invalid Caster reference"));
        return;
    }

    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != Caster && (Actor->GetActorLocation() - ImpactLocation).Size() <= Radius)
        {
            AMonster* HitMonster = Cast<AMonster>(Actor);
            if (HitMonster)
            {
                HitMonster->TakeDMG(Damage);
                UE_LOG(LogTemp, Log, TEXT("Damage applied to %s"), *HitMonster->GetName());
            }
        }
    }

    EndSkill();
}