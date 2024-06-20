// Fill out your copyright notice in the Description page of Project Settings.


#include "BombSkill.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Characters/PlayerChar.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/Monster.h"


ABombSkill::ABombSkill()
{
    PrimaryActorTick.bCanEverTick = true;

    BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
    RootComponent = BombMesh;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bAutoActivate = false;
    ProjectileMovement->ProjectileGravityScale = 1.0f;
    ProjectileMovement->UpdatedComponent = BombMesh;
    BombMesh->SetSimulatePhysics(false);
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

   
}

void ABombSkill::ThrowBomb(FVector LaunchVelocity)
{
  
    ProjectileMovement->Velocity = LaunchVelocity; //메쉬는 제대로 날아가지만 스킬은 다른곳에서터짐
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

    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, ImpactLocation);
    }

    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, ImpactLocation);
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