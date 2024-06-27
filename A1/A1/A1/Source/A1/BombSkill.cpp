// Fill out your copyright notice in the Description page of Project Settings.


#include "BombSkill.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Characters/PlayerChar.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/Monster.h"
#include "Objects/A1PlayerController.h"
#include "Network/A1NetworkManager.h"


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

    BombMesh->OnComponentHit.AddDynamic(this, &ABombSkill::OnBombImpact);
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

   

   
}

void ABombSkill::ThrowBomb(FVector LaunchVelocity)
{
  
    ProjectileMovement->Velocity = LaunchVelocity;
    ProjectileMovement->Activate();
}

void ABombSkill::OnBombImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ImpactLocation = Hit.ImpactPoint;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Damage, [this]()
        {
            PerformDamage(ImpactLocation);
        }, 3.0f, false);
}

void ABombSkill::PerformDamage(const FVector& DamageCenter)
{
    

    if (!Caster) {
       
        return;
    }

    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, DamageCenter);
    }

    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, DamageCenter);
    }

    Super::PerformDamage(DamageCenter);

    EndSkill();
}