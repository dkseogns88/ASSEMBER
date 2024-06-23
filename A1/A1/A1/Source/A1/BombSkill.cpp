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
  
    ProjectileMovement->Velocity = LaunchVelocity; //메쉬는 제대로 날아가지만 스킬은 다른곳에서터짐
    ProjectileMovement->Activate();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Damage, this, &ABombSkill::ApplyDamage, 3.0f, false);
}

void ABombSkill::ApplyDamage()
{
    

    if (!Caster) {
       
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
    
    //서버충돌처리
    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != Caster && (Actor->GetActorLocation() - ImpactLocation).Size() <= Radius)
        {
            AMonster* HitMonster = Cast<AMonster>(Actor);
            if (HitMonster)
            {
                HitMonster->TakeDMG(Damage);

                
                APlayerChar* MyCharacter = Cast<APlayerChar>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

                Protocol::C_ATTACK AttackPkt;
                Protocol::AttackInfo* Info = AttackPkt.mutable_info();
                Info->set_attack_object_id(MyCharacter->GetPosInfo()->object_id());
                Info->set_hit_object_id(HitMonster->GetPosInfo()->object_id());
                Info->set_attack_type(Protocol::AttackType::ATTACK_TYPE_SKILL);
                Info->set_skill_type(Protocol::SkillType::SKILL_TYPE_BOMB);
                
                MyCharacter->GetNetworkManager()->SendPacket(AttackPkt);
            }
        }
    }

    EndSkill();
}