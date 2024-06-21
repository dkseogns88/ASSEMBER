// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "../Objects/A1PlayerController.h"
#include "Engine/World.h"
#include "../Characters/PlayerChar.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


AMonsterProjectile::AMonsterProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

   
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComponent->OnComponentHit.AddDynamic(this, &AMonsterProjectile::OnHit);
    RootComponent = CollisionComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    // can edit at blueprint
    InitialSpeed = 500.0f;
    MaxSpeed = 500.0f;
    bRotationFollowsVelocity = true;
    bShouldBounce = false;
    InitialLifeSpan = 2.0f;
    Damage = 10.0f;

}


void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

//
void AMonsterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    
    if (OtherActor && OtherActor != this && OtherComp)
    {
        //서버충돌처리
        if (OtherActor->IsA(APlayerChar::StaticClass()))
        {
            APlayerChar* HitCharacter = Cast<APlayerChar>(OtherActor);
            if (HitCharacter)
            {
                AA1PlayerController* PlayerController = Cast<AA1PlayerController>(HitCharacter->GetController());
                if (PlayerController)
                {
                    PlayerController->ApplyDamage(Damage);

                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Monk dealing %f damage to %s"), Damage, *HitCharacter->GetName()));
                    }
                }
                HitCharacter->IsDamaged(true);
            }
            
        }
        

        DestroyProjectile();
    }
}

void AMonsterProjectile::DestroyProjectile()
{
    Destroy();
}